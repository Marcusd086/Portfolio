/*
 * scanner.cpp
 *
 *  Created on: May 26, 2020
 *      Author: Marcus DeKnatel
 */


#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cctype>
#include <cmath>

#include "symbol.h"
#include "error_handler.h"
#include "token.h"
#include "scanner.h"
#include "lille_exception.h"

using namespace std;


scanner::scanner()
{
	pos_on_line = -1;	// Nothing read from the input buffer yet.
	line_number = 0;
	eoln_flag = true;	// assume end of line is true before reading anything from the input buffer.
	eof_flag = false;
	input_buffer = "";
	next_char = end_marker;
	current_symbol = new symbol();
	current_token = new token();
	current_line_number = 0;
	current_pos_on_line = 0;
	current_integer_value = 0;
	current_real_value = 0.0;
	current_string_value = "";
	current_identifier_name = "";	
	error = NULL;		// specified by public constructor
	id_tab = NULL;		// specified by public constructor
}


scanner::scanner(string source_filename, id_table* id_t, error_handler* e) : scanner::scanner()
// Open source file. Raise exception if it is not present.
{
	id_tab = id_t;
	error = e;
	if (filesystem::exists(source_filename))		// Check file exists
		source_file.open(source_filename);          // Open default file for reading.
	else
	{
		cerr << "Source code file not found." << endl;
		throw lille_exception("Source code file not found.");
	}
	get_line();
}


int error_message(symbol::symbol_type s)
// Error message associated with symbol s in scanner. This is used so that we have consistency in the error message returned.
{
	switch (s) {
	case symbol::identifier:
		return 0;
		break;
	case symbol::strng:
		return 1;
		break;
	case symbol::real_num:
		return 2;
		break;
	case symbol::integer:
		return 3;
		break;
	case symbol::end_of_program:
		return 4;
		break;
	case symbol::semicolon_sym:
		return 5;
		break;
	case symbol::colon_sym:
		return 6;
		break;
	case symbol::comma_sym:
		return 7;
		break;
	case symbol::equals_sym:
		return 8;
		break;
	case symbol::not_equals_sym:
		return 9;
		break;
	case symbol::less_than_sym:
		return 10;
		break;
	case symbol::greater_than_sym:
		return 11;
		break;
	case symbol::less_or_equal_sym:
		return 12;
		break;
	case symbol::greater_or_equal_sym:
		return 13;
		break;
	case symbol::plus_sym:
		return 14;
		break;
	case symbol::minus_sym:
		return 15;
		break;
	case symbol::slash_sym:
		return 16;
		break;
	case symbol::asterisk_sym:
		return 17;
		break;
	case symbol::power_sym:
		return 18;
		break;
	case symbol::ampersand_sym:
		return 19;
		break;
	case symbol::left_paren_sym:
		return 20;
		break;
	case symbol::right_paren_sym:
		return 21;
		break;
	case symbol::range_sym:
		return 22;
		break;
	case symbol::becomes_sym:
		return 23;
		break;
	case symbol::and_sym:
		return 24;
		break;
	case symbol::begin_sym:
		return 25;
		break;
	case symbol::boolean_sym:
		return 26;
		break;
	case symbol::constant_sym:
		return 27;
		break;
	case symbol::else_sym:
		return 28;
		break;
	case symbol::elsif_sym:
		return 29;
		break;
	case symbol::end_sym:
		return 30;
		break;
	case symbol::eof_sym:
		return 31;
		break;
	case symbol::exit_sym:
		return 32;
		break;
	case symbol::false_sym:
		return 33;
		break;
	case symbol::for_sym:
		return 34;
		break;
	case symbol::function_sym:
		return 35;
		break;
	case symbol::if_sym:
		return 36;
		break;
	case symbol::in_sym:
		return 37;
		break;
	case symbol::integer_sym:
		return 38;
		break;
	case symbol::is_sym:
		return 39;
		break;
	case symbol::loop_sym:
		return 40;
		break;
	case symbol::not_sym:
		return 41;
		break;
	case symbol::null_sym:
		return 42;
		break;
	case symbol::odd_sym:
		return 43;
		break;
	case symbol::or_sym:
		return 44;
		break;
	case symbol::pragma_sym:
		return 45;
		break;
	case symbol::procedure_sym:
		return 46;
		break;
	case symbol::program_sym:
		return 47;
		break;
	case symbol::read_sym:
		return 48;
		break;
	case symbol::real_sym:
		return 49;
		break;
	case symbol::ref_sym:
		return 50;
		break;
	case symbol::return_sym:
		return 51;
		break;
	case symbol::reverse_sym:
		return 52;
		break;
	case symbol::string_sym:
		return 53;
		break;
	case symbol::then_sym:
		return 54;
		break;
	case symbol::true_sym:
		return 55;
		break;
	case symbol::value_sym:
		return 56;
		break;
	case symbol::when_sym:
		return 57;
		break;
	case symbol::while_sym:
		return 58;
		break;
	case symbol::write_sym:
		return 59;
		break;
	case symbol::writeln_sym:
		return 60;
		break;
	default:
		throw lille_exception("Unexpected symbol passed to Error_Message inside Scanner.");
		break;
	}
}


void scanner::get_char()
{
	// gets the next character from the input stream. Checks for end of line and end of file.

	if ((pos_on_line + 1) < input_buffer.length())
	{
		pos_on_line++;
		next_char = input_buffer.at(pos_on_line);
		eoln_flag = false;
	}
	else
		fill_buffer();
}


char scanner::following_char()
{
	// return the character after next_char;
	//if (pos_on_line < (input_buffer.length()-1))
	if ((!eof_flag) and ((pos_on_line + 1) < input_buffer.length()))
		return input_buffer.at(pos_on_line + 1);
	else
		return end_marker;;
}


void scanner::get_line()
{
	if (!source_file.eof())
	{
		getline(source_file, input_buffer);
		line_number++;
	}
	else
	{
		eof_flag = true;
		input_buffer = "";
	}

	if (debugging)
	{
		cout << "In GET_LINE " << line_number << ":  >" << input_buffer << "<" << endl;
	}
}


void scanner::fill_buffer()
// Routine to fill the buffer, i.e., read a line from the source file.
{
	pos_on_line = -1;
	get_line();
	eoln_flag = true;			// indicates end of previous line was encountered. Acts as a token delimiter.
	next_char = end_marker;
}



token* scanner::get_token()
// Get the current token from the input stream. It is held in the private variable current_token.
{
	//skip whitespace and comments to find start of next token.
	while ((!eof_flag) and ((next_char <= ' ') or ((next_char == '-') and (following_char() == '-'))))
	{
		// skip whitespace
		while ((!eof_flag) and (next_char <= ' '))
		{
			get_char();
		}

		// skip comments
		while ((!eof_flag) and ((next_char == '-') and (following_char() == '-')))
			fill_buffer();	// discard the rest of the line
	}

	// initialize variables to record token identified and its current location in the source file;
	current_symbol = new symbol(symbol::end_of_program);	// This is the token returned if at end of file.

	current_line_number = line_number;
	current_pos_on_line = pos_on_line;


	if (!eof_flag)	// If not at end of file
	{
		if (isalpha(next_char))
			scan_alpha();
		else if (isdigit(next_char))
			scan_digit();
		else
			scan_special_symbol();

		switch (current_symbol->get_sym())
		{
		case symbol::identifier:
			current_token = new token(new symbol(symbol::identifier), current_line_number, current_pos_on_line);
			current_token->set_identifier_value(current_identifier_name);
			break;
		case symbol::strng:
			current_token = new token(new symbol(symbol::strng), current_line_number, current_pos_on_line);
			current_token->set_string_value(current_string_value);
			break;
		case symbol::integer:
			current_token = new token(new symbol(symbol::integer), current_line_number, current_pos_on_line);
			current_token->set_integer_value(current_integer_value);
			break;
		case symbol::real_num:
			current_token = new token(new symbol(symbol::real_num), current_line_number, current_pos_on_line);
			current_token->set_real_value(current_real_value);
			break;
		case symbol::pragma_sym:		// pragmas are handled by the scanner not the parser
			parse_pragma();				// pragma can appear anywhere in the code.
			break;
		default:
			current_token = new token(current_symbol, current_line_number, current_pos_on_line);
		}
	}
	else
	{
		// At eof. Set token to end_of_program to indicate end of input.
		current_token = new token(new symbol(symbol::end_of_program), line_number, pos_on_line);
			// This is the token returned if at end of file.
			// The parser needs to process this to make sure that
			// there is no extraneous text after the end of the
			// code is processed.
	}

	if (debugging)
	{
		// display the token about to be returned.
		cout << "GET_TOKEN about to return... ";
		current_token->print_token();
	}

	return current_token;
}


void scanner::scan_string()
{
    	// Process a string. Need to ensure that a string begins and ends with a
    	// double quote and that the string is wholly contained on a single line in
    	// the source file. 2 adjacent double quote marks are interpreted as a single
    	// double quote character within the string.
    
    	// TO BE COMPLETED BEFORE THE SCANNER FUNCTIONS PROPERLY AND THE
    	// PARSER CAN BE STARTED.

	// INSERT CODE HERE.
	string st=""; //string st to hold each of the characters as they are entered
	get_char(); //first character is a " and can be ignored
	bool is_end = false; //boolean to determine if the string is over
	bool eol= false; //boolean to determine if it is the end of line
	while(eol!=true && is_end!=true){ //as long as its not the end of line or string is over it will continue
		if(next_char=='"'){ 
			if(following_char()=='"'){ //condition if there are two " ex:""
				st.push_back(next_char);//only adding one of them to the string
				get_char(); //Then moving over both of them
				get_char();
			}
			else{
				is_end=true; //If only onr quote is detected then the string is over
			}
		}
		else if(eoln_flag==true){ //eoln_flag is part of program already and is used to determine if the line has ended
			eol=true;
			error->flag(current_line_number, current_pos_on_line, 60); //Sending appropriate error					
		}
		else{
			st.push_back(next_char);//Normal condition adds next character to string
			get_char(); //Moving pointer 
		}
	}
	current_string_value = st; //setting our string value equal to the string created in loop
	current_symbol = new symbol(symbol::strng); //Creating string token
}


void scanner::scan_alpha()
{
	// Scan in a token beginning with a letter
	// *** TO BE COMPLETED ***
	// This method takes care of reserved words and identifies in the language. These begin with a alphabetic character
	// and contain an unlimited number of significant characters. Note that lower case letters are converted to upper case
	// before being stored. For each identifier, a check is made to see if the identifier maps onto a reserved word.
	// Make sure that there are no trailing underscores

	bool malformed_ident {false};
	current_identifier_name = "";

	current_identifier_name += char(toupper(next_char)); 	// in case it is an identifier, we need to record what the identifier actually is.
	get_char();
	while (isalpha(next_char) or isdigit(next_char) or (next_char == '_'))
	{
		if ((next_char == '_') and (following_char() == '_'))
			malformed_ident = true;
		current_identifier_name += char(toupper(next_char));
		get_char();
	}
	if (malformed_ident or (current_identifier_name.at(current_identifier_name.length() - 1)) == '_')
		error->flag(current_line_number, current_pos_on_line, 61); 		// Illegal underscore in identifier.
	// check to see if the string matches an a reserved word
	if (current_identifier_name == "AND")
		current_symbol = new symbol(symbol::and_sym);
	else if (current_identifier_name == "BEGIN")
		current_symbol = new symbol(symbol::begin_sym);
	else if (current_identifier_name == "BOOLEAN")
		current_symbol = new symbol(symbol::boolean_sym);
	else if (current_identifier_name == "CONSTANT")
		current_symbol = new symbol(symbol::constant_sym);
	else if (current_identifier_name == "ELSE")
		current_symbol = new symbol(symbol::else_sym);
	else if (current_identifier_name == "ELSIF")
		current_symbol = new symbol(symbol::elsif_sym);
	else if (current_identifier_name == "END")
		current_symbol = new symbol(symbol::end_sym);
	else if (current_identifier_name == "EOF")
		current_symbol = new symbol(symbol::eof_sym);
	else if (current_identifier_name == "EXIT")
		current_symbol = new symbol(symbol::exit_sym);
	else if (current_identifier_name == "FALSE")
		current_symbol = new symbol(symbol::false_sym);
	else if (current_identifier_name == "FOR")
		current_symbol = new symbol(symbol::for_sym);
	else if (current_identifier_name == "FUNCTION")
		current_symbol = new symbol(symbol::function_sym);
	else if (current_identifier_name == "IF")
		current_symbol = new symbol(symbol::if_sym);
	else if (current_identifier_name == "IN")
		current_symbol = new symbol(symbol::in_sym);
	else if (current_identifier_name == "INTEGER")
		current_symbol = new symbol(symbol::integer_sym);
	else if (current_identifier_name == "IS")
		current_symbol = new symbol(symbol::is_sym);
	else if (current_identifier_name == "LOOP")
		current_symbol = new symbol(symbol::loop_sym);
	else if (current_identifier_name == "NOT")
		current_symbol = new symbol(symbol::not_sym);
	else if (current_identifier_name == "NULL")
		current_symbol = new symbol(symbol::null_sym);
	else if (current_identifier_name == "ODD")
		current_symbol = new symbol(symbol::odd_sym);
	else if (current_identifier_name == "OR")
		current_symbol = new symbol(symbol::or_sym);
	else if (current_identifier_name == "PRAGMA")
		current_symbol = new symbol(symbol::pragma_sym);
	else if (current_identifier_name == "PROCEDURE")
		current_symbol = new symbol(symbol::procedure_sym);
	else if (current_identifier_name == "PROGRAM")
		current_symbol = new symbol(symbol::program_sym);
	else if (current_identifier_name == "READ")
		current_symbol = new symbol(symbol::read_sym);
	else if (current_identifier_name == "REAL")
		current_symbol = new symbol(symbol::real_sym);
	else if (current_identifier_name == "REF")
		current_symbol = new symbol(symbol::ref_sym);
	else if (current_identifier_name == "RETURN")
		current_symbol = new symbol(symbol::return_sym);
	else if (current_identifier_name == "REVERSE")
		current_symbol = new symbol(symbol::reverse_sym);
	else if (current_identifier_name == "STRING")
		current_symbol = new symbol(symbol::string_sym);
	else if (current_identifier_name == "THEN")
		current_symbol = new symbol(symbol::then_sym);
	else if (current_identifier_name == "TRUE")
		current_symbol = new symbol(symbol::true_sym);
	else if (current_identifier_name == "VALUE")
		current_symbol = new symbol(symbol::value_sym);
	else if (current_identifier_name == "WHEN")
		current_symbol = new symbol(symbol::when_sym);
	else if (current_identifier_name == "WRITE")
		current_symbol = new symbol(symbol::write_sym);
	else if (current_identifier_name == "WRITELN")
		current_symbol = new symbol(symbol::writeln_sym);
	else if (current_identifier_name == "WHILE")
		current_symbol = new symbol(symbol::while_sym);
	else
		current_symbol = new symbol(symbol::identifier);
}


void scanner::scan_digit()
{
	// scan in a token beginning with a digit
	// Note that a unary operator such as -1 would be detected as a minus sign followed by a digit.

    	// TO BE COMPLETED BEFORE THE SCANNER FUNCTIONS PROPERLY AND THE
    	// PARSER CAN BE STARTED.

	// INSERT CODE HERE.
	current_integer_value = 0;
	current_real_value=0.0;
	string number=""; //Creating string to represent number so that it can be read in a character at a
	//time and then will be converted later
	number.push_back(next_char); //first character was identified as digit and thus can be added
	get_char(); //moving pointer
	bool is_int=true; //boolean to determine if it is an integer
	bool is_error = false; //boolean to determine if there is an error
	bool is_scientific = false; //boolean to determine if it is in scientific notation
	bool is_pos_scientific = true; //boolean to determine if the scientific notation number is positive
	string sci_num=""; //string to represent the number after the E in scientific notation
	//bool one=false; //boolean used for my own discovery of errors within my loop
	while( (isdigit(next_char) || toupper(next_char) == 'E' || next_char == '.') && is_error==false){
	//while loop continues as long as the next character is a digit an E or a decimal, it also ends if 
	//An error is found
		if(is_scientific==true){ //if the number is scientific
			if(next_char == '.' || toupper(next_char)=='E'){ //if it is scientific there should
			//not be aditional decimals or E's 	
				is_error=true;
				error->flag(current_line_number,current_pos_on_line,66); //sending appropriate error
			}
			else{ //normal functionality for scientific notation
				sci_num.push_back(next_char);
				get_char();
				sci_num.push_back(next_char);
			}
		}
		else{
			if(toupper(next_char)=='E'){ //if the next character is E
				is_scientific=true; //turn boolean to true indicating the number is in scientific notation
				if(following_char()=='-'){ //if for when the scientific notation number is negative
					if(is_int==false){
						is_pos_scientific=false; //changing boolean value
						get_char(); //moving pointer
						sci_num.push_back(next_char); //taking - into string for scientific number
						get_char(); //moving pointer
					}
					else{ //if it is an int and the scientific number is negative an error is thrown
						is_error=true;
						error->flag(current_line_number,current_pos_on_line,67);
					}
				}
				else if(following_char()=='+'){//if it is positive then the pointer needs to be shifted 2
					get_char(); //this is because numbers are by default positive so the + is not
					get_char(); //needed
				}				
				else{
					get_char(); //Normal circumstance move the pointer
				}
			}
			else if(next_char=='.'){//if a decimal is encountered the number should no longer be an int 
				if(is_int=false){//if it already is a real an error should be thrown
					error->flag(current_line_number,current_pos_on_line,66);
					is_error=true;
				}
				else{//changing the boolean to represent it is no longer an int
					is_int=false;
					number.push_back(next_char);//taking decimal into string
					get_char();//moving pointer
				}
			}
			else{
				number.push_back(next_char);//normal condition takes in character to string
				get_char();//moves pointer
			}
		}
	}
	if(is_int==false){//if it is a real number
		float num1 = stof(number); //declare num1 as a real and set it equal to converted string
		if((is_scientific==true)&&(sci_num!="")){//if the number is scientific notation and contains values for
//scientific notation number 
			float num2 =stof(sci_num);//converting scientific notation number 
			float num3 = pow(10,num2);//converting from scientific notation to a standard float ->
			num1 = num1 * num3;
		}
		else if(is_scientific==true && sci_num==""){ //throws error for empty scientific notation value
			error->flag(current_line_number,current_pos_on_line,64);
		}
		current_real_value = num1;
		current_symbol = new symbol(symbol::real_num);//creating token
		
	}
	else{ //if number is an int
		int num1 = stoi(number);//declare num1 as an int and set it equal to converted string
		//cout<<num1<<endl;
		if((is_scientific==true)&&(sci_num!="")){//if the number is scientific notation and contains values for
//scientific notation number 
			int num2 = stoi(sci_num);//converting scientific notation number 
			int num3 = pow(10,num2); //converting from scientific notation to a standard int ->
			num1 = num1 * num3;
		}
		else if(is_scientific==true && sci_num==""){ //throws error for empty scientific notation value
			error->flag(current_line_number,current_pos_on_line,64);
		}
			current_integer_value = num1;
			current_symbol = new symbol(symbol::integer); //creating token
	}
}



void scanner::scan_special_symbol()
{
// scan in a token beginning with a special character
	switch (next_char)		// handle multiple character tokens first, the single character tokens
	{
	case ':':	// BECOMES or a COLON
		if (following_char() == '=')
		{
			current_symbol = new symbol(symbol::becomes_sym);
			get_char();
		}
		else
			current_symbol = new symbol(symbol::colon_sym);
		break;
	case '<':	// LESS THAN, LESS OR EQUAL, or NOT EQUAL
		if (following_char() == '=')
		{
			current_symbol = new symbol(symbol::less_or_equal_sym);
			get_char();
		}
		else if (following_char() == '>')
		{
			current_symbol = new symbol(symbol::not_equals_sym);
			get_char();
		}
		else
			current_symbol = new symbol(symbol::less_than_sym);
		break;
	case '>':	// GREATER THAN, or GREATER OR EQUAL
		if (following_char() == '=')
		{
			current_symbol = new symbol(symbol::greater_or_equal_sym);
			get_char();
		}
		else
			current_symbol = new symbol(symbol::greater_than_sym);
		break;
	case '*':	// ASTERISK or POWER symbol
		if (following_char() == '*')
		{
			current_symbol = new symbol(symbol::power_sym);
			get_char();
		}
		else
			current_symbol = new symbol(symbol::asterisk_sym);
		break;
	case '.':	// RANGE symbol
		if (following_char() == '.')
		{
			current_symbol = new symbol(symbol::range_sym);
			get_char();
		}
		else
		{
			// illegal symbol
			current_symbol = new symbol(symbol::nul);
			error->flag(current_line_number, current_pos_on_line, 22);	// Expected a range token.
		}
		break;
	case '"':	// Start of a string
		scan_string();
		break;
	case '&':
		current_symbol = new symbol(symbol::ampersand_sym);
		break;
	case '/':
		current_symbol = new symbol(symbol::slash_sym);
		break;
	case ';':
		current_symbol = new symbol(symbol::semicolon_sym);
		break;
	case '(':
		current_symbol = new symbol(symbol::left_paren_sym);
		break;
	case ')':
		current_symbol = new symbol(symbol::right_paren_sym);
		break;
	case ',':
		current_symbol = new symbol(symbol::comma_sym);
		break;
	case '+':
		current_symbol = new symbol(symbol::plus_sym);
		break;
	case '-':
		current_symbol = new symbol(symbol::minus_sym);
		break;
	case '=':
		current_symbol = new symbol(symbol::equals_sym);
		break;
	default:
		current_symbol = new symbol(symbol::nul);
		error->flag(current_line_number, current_pos_on_line, 74); 	// illegal character.
		break;
	}
	get_char();	// get the next character

}


void scanner::parse_pragma()
{
// parse the pragma identified by the scanner.

    
    // NOTE. YOU WILL NEED TO RETURN TO THIS CODE ONCE THE PARSER AND SYMBOL TABLE
    // ARE WRITTEN IN ORDER TO IMPLEMENT THE FUNCTIONALITY OF PRAGMAS.
    
	string pragma_name = "";

	get_token();	// consume the pragma keyword
	if (current_symbol->get_sym() == symbol::identifier)
	{
		pragma_name = current_identifier_name;;
		if ((pragma_name != "ERROR_LIMIT")
				and (pragma_name != "TRACE")
				and (pragma_name != "UNTRACE")
				and (pragma_name != "DEBUG"))
			error->flag(current_line_number, current_pos_on_line, 70);		// Illegal pragma name
	}
	else
		error->flag(current_line_number, current_pos_on_line, 69);  	// Malformed pragma.
	get_token();	// consume pragma name
	// check to see if arguments are provided to the pragma
	if (current_symbol->get_sym() == symbol::left_paren_sym)
		get_token();	// consume left paren
		// C++ does not support the use of a switch statement on strings.
	else
		error->flag(current_line_number, current_pos_on_line, 20);	// pragmas have arguments so a left paren is expected.

	if (pragma_name == "ERROR_LIMIT")
	{
		if (current_symbol->get_sym() == symbol::integer)
        {
			// INSERT CODE HERE
        }
		else
			error->flag(current_line_number, current_pos_on_line, 71);	// pragma ERROR_LIMIT requires a numeric argument.
	}
	else if (pragma_name == "TRACE")
	{
		if (current_symbol->get_sym() == symbol::identifier)
			// Turn on tracing flag in the symbol table for this identifier.
			// Do not generate an error if the identifier is not present!
		{
            // INSERT CODE HERE
		}
		else
			error->flag(current_line_number, current_pos_on_line, 72);	// pragma TRACE requires a variable name.
	}
	else if (pragma_name == "UNTRACE")
	{
		if (current_symbol->get_sym() == symbol::identifier)
            // Turn off tracing flag in the symbol table for this identifier.
            // Do not generate an error if the identifier is not present!
        {
            // INSERT CODE HERE
        }
		else
			error->flag(current_line_number, current_pos_on_line, 72);	// pragma UNTRACE requires a variable name.
	}
	else if (pragma_name == "DEBUG")
	{
		if (current_symbol->get_sym() == symbol::identifier)
		{
			// INSERT CODE HERE
            // pragma DEBUG requires either ON or OFF as the argument.
		}
		else
			error->flag(current_line_number, current_pos_on_line, 72);	// pragma TRACE requires a variable name.
	}
	else
	{
		// Already generated an error message about an illegal pragma name
	}
	get_token();	// consume the argument
	if (current_symbol->get_sym() == symbol:: right_paren_sym)
		get_token();		// consume right paren
	else
		error->flag(current_line_number, current_pos_on_line, 21);	// Right paren expected
	if (current_symbol->get_sym() == symbol::semicolon_sym)
			get_token();		// consume semicolon
	else
		error->flag(current_line_number, current_pos_on_line, 5);	// semicolon expected

}



bool scanner::have(symbol::symbol_type s)
// Returns true if the current token is an s symbol, false otherwise.
{
	if (current_token->get_sym() == s)
		return true;
	else
		return false;
}


void scanner::must_be(symbol::symbol_type s)
// The current token must be an s symbol otherwise it is a syntax error. If the current token matches
// the symbol s, then the scanner discards the token and advances to the next token in the source file.
{
	if (current_token->get_sym() == s)
		get_token();
	else
		error->flag(current_token, error_message(s));
}


token* scanner::this_token()
// Returns the current token, without advancing to the next token in the input stream.
{
	token* result = new token();
	result = current_token;
	return result;
}
