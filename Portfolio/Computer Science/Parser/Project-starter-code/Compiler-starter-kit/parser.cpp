#include "scanner.h"
#include "symbol.h"
#include "error_handler.h"
#include "token.h"
#include "lille_exception.h"
#include "parser.h"
#include "iostream"
#include "vector"
parser::parser(){
	scan = NULL;
	id_tab = NULL;
	error = NULL;
	simple = false;
	ident_name = "";
}

parser::parser(scanner* s, id_table* t,error_handler* e){
	scan = s;
	id_tab = t;
	error = e;
	simple = false;
	ident_name = "";
	id_tab->init_table();	
}

void parser::PROG(){
	if(debugging)
		cout<<"Entering PROG"<<endl;
	scan->get_token();
	token* current_token;
	if(!(scan->have(symbol::program_sym)))
		error->flag(scan->this_token(),124);
	scan->must_be(symbol::program_sym);
	token* block_name = scan->this_token();
	scan->must_be(symbol::identifier);
	scan->must_be(symbol::is_sym);
	BLOCK(block_name,lille_type::type_prog,lille_kind::unknown,lille_type::type_unknown,1);
	scan->must_be(symbol::semicolon_sym);
	scan->must_be(symbol::end_of_program);
	if(IS_EXPR() || IS_BOOL() || IS_RELOP() || IS_MULTOP() || IS_ADDOP() || IS_PRIMARY() || IS_NUMBER() || IS_DECLARATION() || IS_STATEMENT())
			error->flag(scan->this_token(),77);
	if(debugging)
		cout<<"Exiting PROG"<<endl;	
}
/*
void parser::BLOCK(){
	if(debugging)
		cout<<"Entering BLOCK"<<endl;
	while(IS_DECLARATION())
		DECLARATION();
	scan->must_be(symbol::begin_sym);
	STATEMENT_LIST();
	scan->must_be(symbol::end_sym);
	if(scan->have(symbol::identifier))
		scan->must_be(symbol::identifier);
//	id_tab->exit_scope();
	if(debugging)
		cout<<"Exiting BLOCK"<<endl;	
}
*/

void parser::BLOCK(token* id, lille_type lt, lille_kind kd,lille_type ret,int q){
	if(debugging)
		cout<<"Entering BLOCK"<<endl;
	string block_name;
	if(q!=1){
		block_name = id->get_identifier_value();
		id_table_entry* proc_id = id_tab->enter_id(id,lt,kd,id_tab->scope(),0,ret);
		id_tab->enter_scope();
	}
	else{
		block_name = id->get_identifier_value();
		id_table_entry* proc_id = id_tab->enter_id(id,lt,kd,id_tab->scope(),0,ret);
		id_tab->enter_scope();
	}
	while(IS_DECLARATION())
		DECLARATION();
	scan->must_be(symbol::begin_sym);
	STATEMENT_LIST();
	scan->must_be(symbol::end_sym);
	if(scan->have(symbol::identifier))
	{
		if(block_name.compare(scan->this_token()->get_identifier_value())!=0){
//			throw lille_exception("Error: End Block Identifier Not Same as Block Identifier");
			error->flag(scan->this_token(),75);
		}
		scan->must_be(symbol::identifier);
	}
	id_tab->exit_scope();
	if(debugging)
		cout<<"Exiting BLOCK"<<endl;	
}

void parser::DECLARATION(){ 
	if(debugging)
		cout<<"Entering DECLARATION"<<endl;
	//while(scan->have((symbol::identifier))){
	vector<id_table_entry*> p_1;

	if(scan->have(symbol::procedure_sym)){
		scan->must_be(symbol::procedure_sym);
		token* proc_name = scan->this_token();
		scan->must_be(symbol::identifier);
		if(scan->have(symbol::left_paren_sym)){
			scan->must_be(symbol::left_paren_sym);
			p_1 = PARAM_LIST(lille_type::type_proc); 
			scan->must_be(symbol::right_paren_sym);
		}
		scan->must_be(symbol::is_sym);
		lille_kind kd = lille_kind::unknown;
		if(scan->have(symbol::value_sym)){
			kd = lille_kind::value_param;
		}
		else{
			kd = lille_kind::ref_param;
		}
		BLOCK(proc_name,lille_type::type_proc,kd,lille_type::type_unknown,0);
		scan->must_be(symbol::semicolon_sym);
	}	
	else if(scan->have(symbol::function_sym)){
		scan->must_be(symbol::function_sym);
		token* func_name = scan->this_token();
		scan->must_be(symbol::identifier);
		lille_kind kd = lille_kind::unknown;
		if(scan->have(symbol::left_paren_sym)){
			scan->must_be(symbol::left_paren_sym);
			p_1 = PARAM_LIST(lille_type::type_unknown); 	
			scan->must_be(symbol::right_paren_sym);
		}
		scan->must_be(symbol::return_sym);
		lille_type return_type = TYPE(false);
		scan->must_be(symbol::is_sym);
		if(scan->have(symbol::value_sym)){
			kd = lille_kind::value_param;
		}
		else{
			kd = lille_kind::ref_param;
		}
		BLOCK(func_name,lille_type::type_func,kd,return_type,0);
		scan->must_be(symbol::semicolon_sym);
	}
	else{
		vector<token*> tokens_read = IDENTIFIER_LIST();
		lille_type lt = lille_type::type_unknown;
		lille_kind kd = lille_kind::variable;
		scan->must_be(symbol::colon_sym);
		bool CONST;
		if(scan->have(symbol::constant_sym)){
			kd= lille_kind::constant;
			scan->must_be(symbol::constant_sym);
			CONST=true;
		}
		else
			CONST=false;
		lt = TYPE(CONST);
		int i_val;
		float r_val;
		string s_val; 
		bool b_val;
		id_table_entry* id;
		for(token* &i : tokens_read){
			if(i!=NULL){
				id = id_tab->enter_id(i,lt,kd,id_tab->scope(),0,lille_type::type_unknown);
				if(CONST){
					id->fix_const(i_val,r_val,s_val,b_val);
				}
			}
		}
		if(debugging)
			id_tab->dump_id_table();
		scan->must_be(symbol::semicolon_sym);
//		if(scan->have(symbol::integer);
//			scan->must_be(symbol::integer);
//		else if(scan->have(symbol::strng);
//			scan->must_be(symbol::strng);
//		else
//			scan->must_be(symbol::boolean_sym);
//		}
	}
	if(debugging)
		cout<<"Exiting DECLARATION"<<endl;	
}

lille_type parser::TYPE(bool CONST){
	if(debugging)
		cout<<"Entering TYPE"<<endl;
	lille_type t = lille_type::type_unknown;
	if(scan->have(symbol::integer_sym)){
		scan->must_be(symbol::integer_sym);
		t = lille_type::type_integer;
		if(CONST){
			scan->must_be(symbol::becomes_sym);
			scan->must_be(symbol::integer);
		}
	}
	else if(scan->have(symbol::real_sym)){
		scan->must_be(symbol::real_num);
		t = lille_type::type_real;
		if(CONST){
			scan->must_be(symbol::becomes_sym);
			scan->must_be(symbol::real_num);
		}
	}
	else if(scan->have(symbol::string_sym)){
		scan->must_be(symbol::strng);
		t = lille_type::type_string;
		if(CONST){
			scan->must_be(symbol::becomes_sym);
			scan->must_be(symbol::strng);
		}
	}
	else if(IS_BOOL()){
		scan->must_be(symbol::boolean_sym);
		t = lille_type::type_boolean;
		if(CONST){
			scan->must_be(symbol::becomes_sym);
			BOOL();
		}
	}
	else{
		error->flag(scan->this_token(),96);
	}
	if(debugging)
		cout<<"Exiting TYPE"<<endl;	
	return t;
}

vector<id_table_entry*> parser::PARAM_LIST(lille_type pkind){
	if(debugging)
		cout<<"Entering PARAM_LIST"<<endl;
	vector<id_table_entry*> id_tes;
	vector<token*> tokens_list = IDENTIFIER_LIST();
	lille_kind kd = lille_kind::unknown;
//	tokens_list = IDENTIFIER_LIST();
	kd = PARAM(false);
	lille_type typ = TYPE(false);
	id_table_entry* hold;
	for(int x = 0; x<tokens_list.size();x++){
		id_tes.push_back(id_tab ->enter_id(tokens_list[x],typ,kd));
	}
	while(scan->have(symbol::semicolon_sym)){
		scan->must_be(symbol::semicolon_sym);
		tokens_list = IDENTIFIER_LIST();
		kd = PARAM(false);	
		lille_type typ = TYPE(false);
		for(int x = 0; x<tokens_list.size();x++){
			id_tes.push_back(id_tab ->enter_id(tokens_list[x],typ,kd));
		}
	}
	if(debugging)
		cout<<"Exiting PARAM_LIST"<<endl;	
	return id_tes;
}

lille_kind parser::PARAM(bool b){
	if(debugging)
		cout<<"Entering PARAM"<<endl;
	if(b){
		IDENTIFIER_LIST();
	}	
	scan->must_be(symbol::colon_sym);
	lille_kind kd = lille_kind::unknown;
	kd = PARAM_KIND();
	if(b){
		TYPE(false);
	}
	if(debugging)
		cout<<"Exiting PARAM"<<endl;	
	return kd;
}

/*
vector<token*> parser::IDENTIFIER_LIST(){
	if(debugging)
		cout<<"Entering IDENTIFIER_LIST"<<endl;
	scan->must_be(symbol::identifier);
	while(scan->have(symbol::comma_sym)){
		scan->must_be(symbol::comma_sym);	
		scan->must_be(symbol::identifier);
	}
	if(debugging)
		cout<<"Exiting IDENTIFIER_LIST"<<endl;	
	return tokens_read;
}

*/
vector<token*> parser::IDENTIFIER_LIST(){
	if(debugging)
		cout<<"Entering IDENTIFIER_LIST"<<endl;
	vector<token*> tokens_read;
	tokens_read.push_back(new token(new symbol(symbol::identifier),0,0));
	tokens_read.back()->set_identifier_value(scan->get_current_identifier_name());
	scan->must_be(symbol::identifier);
	while(scan->have(symbol::comma_sym)){
		scan->must_be(symbol::comma_sym);	
		tokens_read.push_back(new token(new symbol(symbol::identifier),0,0));
		tokens_read.back()->set_identifier_value(scan->get_current_identifier_name());
		scan->must_be(symbol::identifier);
	}
	if(debugging)
		cout<<"Exiting IDENTIFIER_LIST"<<endl;	
	return tokens_read;
}

lille_kind parser::PARAM_KIND(){
	if(debugging)
		cout<<"Entering PARAM_KIND"<<endl;
	lille_kind kd = lille_kind::unknown;
	if(scan->have(symbol::value_sym)){
		scan->must_be(symbol::value_sym);
		kd = lille_kind::value_param;
	}
	else{
		scan->must_be(symbol::ref_sym);
		kd = lille_kind::ref_param;
	}
	if(debugging)
		cout<<"Exiting PARAM_KIND"<<endl;	
	return kd;
}

void parser::STATEMENT_LIST(){
	if(debugging)
		cout<<"Entering STATEMENT_LIST"<<endl;
	STATEMENT();
	scan->must_be(symbol::semicolon_sym);
	while(IS_STATEMENT()){
		STATEMENT();	
		if(scan->have(symbol::semicolon_sym))
			scan->must_be(symbol::semicolon_sym);
//		else throw lille_exception("Error: Statement Not Read Successfully");
		else error->flag(scan->this_token(), 79);
	}
	if(debugging)
		cout<<"Exiting STATEMENT_LIST"<<endl;	
}

void parser::STATEMENT(){
	if(debugging)
		cout<<"Entering STATEMENT"<<endl;
	if(scan->have(symbol::identifier) || scan->have(symbol::exit_sym) || scan->have(symbol::return_sym) || scan->have(symbol::read_sym) || scan->have(symbol::write_sym) || scan->have(symbol::writeln_sym) || scan->have(symbol::nul))
		SIMPLE_STATEMENT();
	else{
		if(scan->have(symbol::if_sym))
			IF_STATEMENT();
		else if(scan->have(symbol::for_sym))
			FOR_STATEMENT();
		else if(scan->have(symbol::while_sym))
			WHILE_STATEMENT();
		else if(scan->have(symbol::loop_sym))
			LOOP_STATEMENT();
		//else throw lille_exception("Error: Statement Not Read Successfully");
		else error->flag(scan->this_token(), 79);
	}
	if(debugging)
		cout<<"Exiting STATEMENT"<<endl;	
}

void parser::SIMPLE_STATEMENT(){
	if(debugging)
		cout<<"Entering SIMPLE_STATEMENT"<<endl;
	lille_type id_type; 
	if(scan->have(symbol::identifier)){
//		scan->must_be(symbol::identifier);
//cout<<"YOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
		id_type = check_identifier();
//cout<<"YOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
		if(scan->have(symbol::left_paren_sym)){
			scan->must_be(symbol::left_paren_sym);
			EXPR(id_type);
			while(scan->have(symbol::comma_sym)){
				scan->must_be(symbol::comma_sym);
				EXPR(id_type);
			}
			scan->must_be(symbol::right_paren_sym);
		}
		else if(scan->have(symbol::becomes_sym)){
			scan->must_be(symbol::becomes_sym);
			EXPR(id_type);
		}
	}
	else if(scan->have(symbol::exit_sym)){
		scan->must_be(symbol::exit_sym);
		if(scan->have(symbol::when_sym)){
			scan->must_be(symbol::when_sym);
			EXPR(id_type);
		}
	}
	else if(scan->have(symbol::return_sym)){
		scan->must_be(symbol::return_sym);
		if(IS_EXPR())
			EXPR(lille_type::type_unknown);
	}
	else if(scan->have(symbol::read_sym)){
		scan->must_be(symbol::read_sym);
		if(scan->have(symbol::left_paren_sym))
			scan->must_be(symbol::left_paren_sym);
		scan->must_be(symbol::identifier);
		while(scan->have(symbol::comma_sym)){
			scan->must_be(symbol::comma_sym);
			scan->must_be(symbol::identifier);
		}	
		if(scan->have(symbol::right_paren_sym)){
			scan->must_be(symbol::right_paren_sym);
		}
	}
	else if(scan->have(symbol::write_sym)){
		scan->must_be(symbol::write_sym);
		if(scan->have(symbol::left_paren_sym))
			scan->must_be(symbol::left_paren_sym);
		EXPR(lille_type::type_unknown);
		while(scan->have(symbol::comma_sym)){
			scan->must_be(symbol::comma_sym);
			EXPR(lille_type::type_unknown);
		}	
		if(scan->have(symbol::right_paren_sym)){
			scan->must_be(symbol::right_paren_sym);
		}
	}
	else if(scan->have(symbol::writeln_sym)){
		scan->must_be(symbol::writeln_sym);
		if(scan->have(symbol::left_paren_sym))
			scan->must_be(symbol::left_paren_sym);
		EXPR(lille_type::type_unknown);
		while(scan->have(symbol::comma_sym)){
			scan->must_be(symbol::comma_sym);
			EXPR(lille_type::type_unknown);
		}	
		if(scan->have(symbol::right_paren_sym)){
			scan->must_be(symbol::right_paren_sym);
		}
	}
	else if(scan->have(symbol::nul)){
		scan->must_be(symbol::nul);
	}
	if(debugging)
		cout<<"Exiting SIMPLE_STATEMENT"<<endl;	
}

void parser::IF_STATEMENT(){
	if(debugging)
		cout<<"Entering IF_STATEMENT"<<endl;
	scan->must_be(symbol::if_sym);
	EXPR(lille_type::type_unknown);
	scan->must_be(symbol::then_sym);
	STATEMENT_LIST();
	while(scan->have(symbol::elsif_sym)){
		scan->must_be(symbol::elsif_sym);
		EXPR(lille_type::type_unknown);
		scan->must_be(symbol::then_sym);
		STATEMENT_LIST();
	}
	if(scan->have(symbol::else_sym)){
		scan->must_be(symbol::else_sym);
		STATEMENT_LIST();
	}
	scan->must_be(symbol::end_sym);
	scan->must_be(symbol::if_sym);
	if(debugging)
		cout<<"Exiting IF_STATEMENT"<<endl;	
}

void parser::WHILE_STATEMENT(){
	if(debugging)
		cout<<"Entering WHILE_STATEMENT"<<endl;
	scan->must_be(symbol::while_sym);
	EXPR(lille_type::type_unknown);
	LOOP_STATEMENT();
	if(debugging)
		cout<<"Exiting WHILE_STATEMENT"<<endl;	
}

void parser::FOR_STATEMENT(){
	if(debugging)
		cout<<"Entering FOR_STATEMENT"<<endl;
	scan->must_be(symbol::for_sym);
	token* proc_name = scan->this_token();
	scan->must_be(symbol::identifier);
   if(id_tab->lookup(scan->this_token()->get_identifier_value())==NULL) {
	id_tab->enter_id(proc_name,lille_type::type_integer,lille_kind::unknown,id_tab->scope(),0,lille_type::type_unknown);
   } 
//			id_tab->dump_id_table();
//	check_identifier();
	scan->must_be(symbol::in_sym);
	if(scan->have(symbol::reverse_sym))
		scan->must_be(symbol::reverse_sym);
	RANGE();
	LOOP_STATEMENT();
	if(debugging)
		cout<<"Exiting FOR_STATEMENT"<<endl;	
}

void parser::LOOP_STATEMENT(){
	if(debugging)
		cout<<"Entering LOOOOOOOOOOOOOOOOOOOOOOOOP_STATEMENT"<<endl;
	scan->must_be(symbol::loop_sym);
	STATEMENT_LIST();
	if(scan->have(symbol::end_sym)){
		scan->must_be(symbol::end_sym);
		scan->must_be(symbol::loop_sym);
	}
	else throw lille_exception("Error: End Symbol Not Found At End Of LOOP");
	if(debugging)
		cout<<"Exiting LOOP_STATEMENT"<<endl;	
}

void parser::RANGE(){
	if(debugging)
		cout<<"Entering RANGE"<<endl;
	SIMPLE_EXPR(lille_type::type_integer);
	scan->must_be(symbol::range_sym);	
	SIMPLE_EXPR(lille_type::type_integer);	
	if(debugging)
		cout<<"Exiting RANGE"<<endl;	
}

void parser::EXPR(lille_type typ){
	if(debugging)
		cout<<"Entering EXPR"<<endl;
	SIMPLE_EXPR(typ);
	if(scan->have(symbol::in_sym)){
		scan->must_be(symbol::in_sym);
		RANGE();
	}
	else if(IS_RELOP()){
		RELOP();
		SIMPLE_EXPR(typ);
	}
	if(debugging)
		cout<<"Exiting EXPR"<<endl;	
}

void parser::BOOL(){
	if(debugging)
		cout<<"Entering BOOL"<<endl;
	if(scan->have(symbol::true_sym))
		scan->must_be(symbol::true_sym);
	else	
		scan->must_be(symbol::false_sym);	
	if(debugging)
		cout<<"Exiting BOOL"<<endl;	
}

void parser::RELOP(){
	if(debugging)
		cout<<"Entering RELOP"<<endl;
	if(scan->have(symbol::greater_than_sym))
		scan->must_be(symbol::greater_than_sym);
	else if(scan->have(symbol::less_than_sym))
		scan->must_be(symbol::less_than_sym);
	else if(scan->have(symbol::equals_sym))
		scan->must_be(symbol::equals_sym);
	else if(scan->have(symbol::not_equals_sym))	 
		scan->must_be(symbol::not_equals_sym);
	else if(scan->have(symbol::less_or_equal_sym))
		scan->must_be(symbol::less_or_equal_sym);
	else if(scan->have(symbol::greater_or_equal_sym))
		scan->must_be(symbol::greater_or_equal_sym);
	if(debugging)
		cout<<"Exiting RELOP"<<endl;
}

void parser::SIMPLE_EXPR(lille_type typ){
	if(debugging)
		cout<<"Entering SIMPLE_EXPR"<<endl;
	EXPR2(typ);
	while(scan->have(symbol::ampersand_sym)){
		STRINGOP();
		EXPR2(typ);
	}
	if(debugging)
		cout<<"Exiting SIMPLE_EXPR"<<endl;	
}

void parser::STRINGOP(){
	if(debugging)
		cout<<"Entering STRINGOP"<<endl;
	scan->must_be(symbol::ampersand_sym);
	if(debugging)
		cout<<"Exiting STRINGOP"<<endl;	
}

void parser::EXPR2(lille_type typ){
	if(debugging)
		cout<<"Entering EXPR2"<<endl;
	TERM(typ);	
	while(IS_ADDOP() || scan->have(symbol::or_sym)){
		if(IS_ADDOP()){
			ADDOP();
		}
		else{
			scan->must_be(symbol::or_sym);
		}
		TERM(typ);
	}
	if(debugging)
		cout<<"Exiting EXPR2"<<endl;	
}

void parser::ADDOP(){
	if(debugging)
		cout<<"Entering ADDOP"<<endl;
	if(scan->have(symbol::plus_sym))
		scan->must_be(symbol::plus_sym);
	else
		scan->must_be(symbol::minus_sym);		
	if(debugging)
		cout<<"Exiting ADDOP"<<endl;	
}

void parser::TERM(lille_type typ){
	if(debugging)
		cout<<"Entering TERM"<<endl;
	FACTOR(typ);
	while(IS_MULTOP() || scan->have(symbol::and_sym)){
		if(IS_MULTOP()){
			MULTOP();
		}
		else{
			scan->must_be(symbol::and_sym);
		}
		FACTOR(typ);
	}
	if(debugging)
		cout<<"Exiting TERM"<<endl;	
}

void parser::MULTOP(){
	if(debugging)
		cout<<"Entering MULTOP"<<endl;
	if(scan->have(symbol::asterisk_sym))
		scan->must_be(symbol::asterisk_sym);
	else
		scan->must_be(symbol::slash_sym);		
	if(debugging)
		cout<<"Exiting MULTOP"<<endl;	
}

void parser::FACTOR(lille_type typ){
	if(debugging)
		cout<<"Entering FACTOR"<<endl;
	if(IS_ADDOP()){
		ADDOP();
		PRIMARY(typ);
	}
	else if(IS_PRIMARY()){
		PRIMARY(typ);
		if(scan->have(symbol::asterisk_sym)){
//			scan->must_be(symbol::asterisk_sym);
//			scan->must_be(symbol::asterisk_sym);
			scan->must_be(symbol::power_sym);
			PRIMARY(typ);
		}
	}
//	else throw lille_exception("Error: No Factor Found");
	if(debugging)
		cout<<"Exiting FACTOR"<<endl;	
}

void parser::PRIMARY(lille_type typ){
	if(debugging)
		cout<<"Entering PRIMARY"<<endl;
//cout<<"heeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeey";
	if(scan->have(symbol::not_sym)){
		scan->must_be(symbol::not_sym);
		EXPR(lille_type::type_unknown);
	}
	else if(scan->have(symbol::odd_sym)){
		scan->must_be(symbol::odd_sym);
		EXPR(lille_type::type_unknown);
	}
	else if(scan->have(symbol::left_paren_sym)){
		scan->must_be(symbol::left_paren_sym);
		SIMPLE_EXPR(lille_type::type_unknown);
		scan->must_be(symbol::right_paren_sym);
	}
	else if(scan->have(symbol::identifier)){
//		scan->must_be(symbol::identifier);
		lille_type found_type;
		string id_val_found = scan->this_token()->get_identifier_value();
		found_type = check_identifier();
      //scan->must_be(symbol::identifier);
	if(!found_type.is_type(lille_type::type_unknown) and !typ.is_type(lille_type::type_unknown)) {
		if(found_type.is_type(lille_type::type_real) or found_type.is_type(lille_type::type_integer) or
			found_type.is_type(lille_type::type_string) or found_type.is_type(lille_type::type_boolean)) {
		if(typ.is_type(lille_type::type_real) or typ.is_type(lille_type::type_integer) or
			typ.is_type(lille_type::type_string) or typ.is_type(lille_type::type_boolean)) {
		if(!check_type(found_type,typ)) {
			cout << "Identifier with Incompatible Type: " << id_val_found << endl;
//		throw lille_exception("Error: Incompatible Types Detected");
		error->flag(scan->this_token(), 114);
		}
		}
		}
		}
		if(scan->have(symbol::left_paren_sym)){
			scan->must_be(symbol::left_paren_sym);
			EXPR(found_type);
			while(scan->have(symbol::comma_sym)){
				scan->must_be(symbol::comma_sym);
				EXPR(found_type);
			}
			scan->must_be(symbol::right_paren_sym);
		}
	//}
	}
	else if(IS_NUMBER()){
		if(scan->have(symbol::integer)){
			scan->must_be(symbol::integer);
		}
		else if(scan->have(symbol::real_num))
			scan->must_be(symbol::real_num);
		//NUMBER(); //I have to do this
	}
	else if(scan->have(symbol::strng)){
		scan->must_be(symbol::strng);
		//STRING();
	}
	else if(IS_BOOL()){
		BOOL();
	}	
	if(debugging)
		cout<<"Exiting PRIMARY"<<endl;	
}
/*
void parser::IDENT(){
	scan->must_be(symbol::string_sym); //THIS OR STRNG
	while(scan->have(symbol:: || have(symbol::string_sym) || have(symbol::integer_sym)){ //NO UNDERSCORE SYMBOL and what do for letter/digit
		scan->have(symbol::); //UNDERSCORE
		IDENT2();
	}
}

void parser::IDENT2();{
	if(scan->have(symbol::string_sym) //THIS OR STRNG
		scan->must_be(symbol::string_sym); //THIS OR STRNG
	else
		scan->must_be(symbol::integer_sym); //THIS OR real
}

void parser::NUMBER(){
	DIGIT_SEQ();
	if(scan->have(symbol::_sym) //No symbol for decimal
		scan->must_be(symbol::_sym); //No symbol for decimal
	if(IS_EXPR()){
		EXPR();
		ADDOP();
		DIGITSEQ();
	}
}

void parser::DIGIT_SEQ(){
	scan->must_be(symbol::integer_sym); //THIS OR real, no digit symbol
	while(have(symbol::integer_sym){ //no digit symbol
		scan->must_be(symbol::integer_sym); //THIS OR real, no digit symbol
	}
}

void parser::EXP(){
	scan->must_be(symbol::'E'); //No symbol for E or e
	scan->must_be(symbol::'e'); //No symbol for E or e
}

void parser::PRAGMA(){
	scan->must_be(symbol::pragma_sym);
	PRAGMA_NAME();
	while(IS_NUMBER()||scan->have(symbol::identifier)){
		if(IS_NUMBER())
			DIGIT_SEQ();
		else
			scan->must_have(symbol::identifier));
	}	
}

void parser::PRAGMA_NAME(){
	//
}
*/
bool parser::IS_EXPR(){ 
	if(IS_ADDOP() || IS_PRIMARY())	
		return true;
	else
		return false;
}

bool parser::IS_BOOL(){
	if(scan->have(symbol::true_sym) || scan->have(symbol::false_sym))
		return true;
	else
		return false;
}

bool parser::IS_RELOP(){
	if(scan->have(symbol::greater_than_sym))
		return true;
	else if(scan->have(symbol::less_than_sym))
		return true;
	else if(scan->have(symbol::equals_sym))
		return true;
	else if(scan->have(symbol::not_equals_sym)) 
		return true;
	else if(scan->have(symbol::less_or_equal_sym))
		return true;
	else if(scan->have(symbol::greater_or_equal_sym))
		return true;
	else
		return false;	
}

bool parser::IS_MULTOP(){
	if(scan->have(symbol::asterisk_sym) || scan->have(symbol::slash_sym))
		return true;
	else
		return false;
}

bool parser::IS_ADDOP(){
	if(scan->have(symbol::plus_sym) || scan->have(symbol::minus_sym)){
		return true;
	}
	else{
		return false;
	}
}

bool parser::IS_PRIMARY(){
	if(scan->have(symbol::not_sym)){
		return true;
	}
	else if(scan->have(symbol::odd_sym)){
		return true;
	}
	else if(scan->have(symbol::left_paren_sym)){
		return true;
	}
	else if(scan->have(symbol::identifier)){
		return true;
	}
	else if(IS_NUMBER()){
		return true;
	}
	else if(scan->have(symbol::strng)){
		return true;
	}
	else if(IS_BOOL()){
		return true;
	}
	else	
		return false;
}

bool parser::IS_NUMBER(){
	if(scan->have(symbol::integer) || scan->have(symbol::real_num))
		return true;
	else
		return false;
}

bool parser::IS_DECLARATION(){ 
	if(scan->have(symbol::identifier) || scan->have(symbol::procedure_sym) || scan->have(symbol::function_sym))
		return true;
	else
		return false;
}

bool parser::IS_STATEMENT(){ 
	if(scan->have(symbol::identifier) || scan->have(symbol::exit_sym) || scan->have(symbol::return_sym) || scan->have(symbol::read_sym) || scan->have(symbol::write_sym) || scan->have(symbol::writeln_sym) || scan->have(symbol::nul) || scan->have(symbol::if_sym) || scan->have(symbol::for_sym) || scan->have(symbol::while_sym) || scan->have(symbol::loop_sym))
		return true;
	else
		return false;
}

lille_type parser::check_identifier() {
	id_table_entry* idte = id_tab->lookup(scan->this_token()->get_identifier_value());       
	if(idte!=NULL) {
		scan->must_be(symbol::identifier);
		return idte->tipe(); 
	}
	else {
		cout << "Attempted to Access: " << scan->this_token()->get_identifier_value() << endl; 
//      throw lille_exception("Error: Attempt to access non-declared variable.");
		error->flag(scan->this_token(), 81);
	token* proc_name = scan->this_token();
	scan->must_be(symbol::identifier);
	id_tab->enter_id(proc_name,lille_type::type_integer,lille_kind::unknown,id_tab->scope(),0,lille_type::type_unknown);
		
	}
	return lille_type::type_unknown;
}

bool parser::check_type(lille_type typ1, lille_type typ2) {
	if(typ1.is_type(lille_type::type_unknown) or typ1.is_type(lille_type::type_unknown)) {
//      if(debug) cout << "Type Check Redundant" << endl;
		return false;
	}
	else if(!typ1.is_type(typ2)) 
      //throw lille_exception("Error: Type Incompatability Detected");
		return false;
	else if(typ1.is_type(typ2)) return true;
	else return false;
}
