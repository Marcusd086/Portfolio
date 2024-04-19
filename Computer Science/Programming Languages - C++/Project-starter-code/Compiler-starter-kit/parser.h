#include "scanner.h"
#include "symbol.h"
#include "error_handler.h"
#include "token.h"
#include "scanner.h"
#include "lille_exception.h"
#include "vector"


class parser{
	private:
		bool debugging {false};
		bool simple;
		scanner* scan;
		error_handler* error;
		string ident_name;
		id_table* id_tab;

		void BLOCK();
		void BLOCK(token* id, lille_type lt, lille_kind kd,lille_type ret,int q);
		void DECLARATION();
		lille_type TYPE(bool CONST);
		vector<id_table_entry*> PARAM_LIST(lille_type pkind);
		lille_kind PARAM(bool b);
		lille_kind PARAM_KIND();
		vector<token*> IDENTIFIER_LIST();
		void STATEMENT_LIST();
		void STATEMENT();
		void SIMPLE_STATEMENT();
		void IF_STATEMENT();
		void WHILE_STATEMENT();
		void FOR_STATEMENT();
		void LOOP_STATEMENT();
		void RANGE();
		void EXPR(lille_type typ);
		void BOOL();
		void RELOP();
		void SIMPLE_EXPR(lille_type typ);
		void STRINGOP();
		void EXPR2(lille_type typ);
		void ADDOP();
		void TERM(lille_type typ);
		void MULTOP();
		void FACTOR(lille_type typ);
		void PRIMARY(lille_type typ);
//		void IDENT();
//		void IDENT2();
//		void NUMBER();
//		void DIGIT_SEQ();

		bool IS_EXPR();
		bool IS_BOOL();
		bool IS_RELOP();
		bool IS_MULTOP();
		bool IS_ADDOP();
		bool IS_PRIMARY();
		bool IS_NUMBER();
		bool IS_DECLARATION();
		bool IS_STATEMENT();
		lille_type check_identifier();
		bool check_type(lille_type typ1, lille_type typ2);

		public:
		parser();
		parser(scanner* s,id_table* t, error_handler* e);

		void PROG();
};




