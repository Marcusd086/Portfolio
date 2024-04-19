/*
 * idtable.cpp
 *
 *  Created on: Jun 18, 2020
 *      Author: Marcus DeKnatel
 */



#include <iostream>
#include <string>

#include "token.h"
#include "error_handler.h"
#include "id_table.h"
#include "queue"
//#include "id_table_entry.h"
//#include "lille_kind.h"
//#include "lille_type.h"

using namespace std;
/*
id_table::node* id_table::search_tree(string s, node* p){
	if(s.compare(p->idt->name())==0)
		return p;
	else if(p==NULL)
		return NULL;
	node* q = search_tree(s,p->left);
	node* r = search_tree(s,p->right);
	if(s.compare(q->idt->name())==0)
		return q;
	else if(s.compare(r->idt->name())==0)
		return r;
	else
		return NULL;
}
*/

id_table::node* id_table::search_tree(string s, node* p) {
   node* q;
   node* r;
   bool flag1 = false, flag2 = false;
   if(p==NULL)
      return NULL;
   else if(p->idt!=NULL) {
      if(s.compare(p->idt->name())==0)
         return p;
      if(p->left!=NULL) {
         q = search_tree(s,p->left);
	 flag1 = true;
      }
      if(p->right!=NULL) {
         r = search_tree(s,p->right); 
 	 flag2 = true;
      }        
   }
   if(flag1) {
      if(q!=NULL) {
         if(s.compare(q->idt->name())==0)
            return q;
      }
   }
   if(flag2) {
      if(r!=NULL) {
         if(s.compare(r->idt->name())==0)
            return r;
      }
   }
   return NULL;
}

id_table::node* id_table::insert(id_table_entry *idt_entry, node *p){
	if(p==NULL){
		p= new node;
		p->idt = idt_entry;
		p->left=NULL;
		p->right=NULL;
		return p;
	}		

	if(idt_entry-> token_value()->get_identifier_value() > p->idt->token_value()->get_identifier_value())
	{
		p->right = insert(idt_entry, p->right);
	}
	else{
		p->left = insert(idt_entry, p->left);
	}
	return p;
}

void id_table::add_table_entry(id_table_entry* id){
	sym_table[scope_level]=insert(id, sym_table[scope_level]);
}
/*
void id_table::add_table_entry(id_table_entry* id){
	node* p = new node(it);
	if(ptr == NULL){
		ptr = p;
		return;
	}
	bool added = false;
	queue<node*> node_queue;
	node_queue.push(ptr);
	
	while(!added){
		ptr = node_queue.front();
		if(ptr->left == NULL){
			ptr->left = p;
			added = true;
		}
		else if(ptr->right == NULL){
			ptr->right=p;
			added = true;
		}
		else{
			node_queue.pop();
			node_queue.push(ptr->left);
			node_queue.push(ptr->right);
		}
		
	}
}
*/
void id_table::dump_tree(node* ptr){
	if(!ptr)
		return;
	dump_tree(ptr->left);
	cout<<"Ident Name: "<< ptr->idt->token_value()->get_identifier_value()<<endl;
	cout << "Kind: " << ptr->idt->kind().to_string()<<endl;
	cout<<"Type: " << ptr->idt->tipe().to_string() <<endl;
	cout<<"Value: "<<ptr->idt->name()<<endl<<endl;
	dump_tree(ptr->right);
}

id_table::id_table(error_handler* err)
{
	error = err;
	scope_level=0;
	sym_table[0]=NULL;
    // INSERT CODE HERE
	
}

void id_table::enter_scope(){
	scope_level++;	
	sym_table[scope_level]=NULL;
}

void id_table::exit_scope(){
	scope_level--;
//	scope_level--
}

int id_table::scope(){
	return scope_level;
}
/*
id_table_entry* id_table::lookup(string s){
	int s_lvl=scope_level;
	node* p = (search_tree(s, sym_table[s_lvl]));
	s_lvl--;
	while(p == NULL && s_lvl >=0){
		p = search_tree(s,sym_table[s_lvl]);
	}

	if (p == NULL){
		return NULL;
	}
	else{
		return p-> idt;
	}
}

id_table_entry* id_table::lookup(token* tok){
	string key = tok -> get_identifier_value();
	return lookup(key);
}
*/
id_table_entry* id_table::lookup(string s) { 
   node* p;
   for(int x = scope_level; x>=0; x--) {
      p = search_tree(s,sym_table[x]);
      if(p!=NULL) {
         if(p->idt!=NULL) {
//            if(debug) cout << "Entry Found: " << p->idt->name() << " at Scope Level: " << x << endl;
            return p->idt;
         }
      }
   }
   return NULL;
   //*/
}

id_table_entry* id_table::lookup(token* tok) {
//   if(debug) dump_id_table();
   node* p;
   queue<node*> node_queue;
   node_queue.push(sym_table[scope_level]);
   for(int x = scope_level; x>=0; x--) {
      while(!node_queue.empty()) {
         p = node_queue.front();
//         if(debug) cout << "Number of Nodes: " << node_queue.size() << endl;
         if(p->idt->token_value()==tok) {
 	    /*if(debug) {
	       cout << "Entry Found: " << p->idt->name() << " at Scope Level: " << x << endl;
	    }*/
	    return p->idt;
         }	 

         if(p->left!=NULL) {
	    node_queue.push(p->left);
	    /*if(debug) {
               cout << "Added Left Node to Stack" << endl;
	       cout << p->left->idt->name() << endl;
	    }*/
         }
	 if(p->right!=NULL) {
	    //if(debug) cout << "Added Right Node to Stack" << endl;
	    node_queue.push(p->right);

 	 } 

	 node_queue.pop();  
      } 
   }
   return NULL;
}
/*
void id_table::trace_all(bool b){

}

bool id_table::trace_all(){

}

void id_table::add_table_entry(id_table_entry* id){
	node* p = new node(it);
	if(ptr==NULL){
		ptr=p;
		return;
	}
	bool added=false;
	queue<node*> node_queue;
	node_queue.push(ptr);

	while(!added){
		ptr=node.queue.front();
		if(ptr->left == NULL){
			ptr->left = p;
			added=true;
		}
		else if(ptr->right == NULL){
			ptr->right=p;
			added=true;
		}
		else{
			node_queue.pop();
			node_queue.push(ptr->left);
			node_queue.push(ptr->right);
			
		}
	}
}
*/
id_table_entry* id_table::enter_id(token* id,lille_type typ, lille_kind kind, int level, int offset, lille_type return_tipe){
	id_table_entry *new_entry;
	new_entry = new id_table_entry(id,typ,kind,level,offset,return_tipe);
	add_table_entry(new_entry);
	return new_entry;
}

void id_table::dump_id_table(bool dump_all)
{
	int scope_temp = scope_level;
	if (!dump_all)
	{
		cout << "Dump of idtable for current scope only." << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout<<"Current scope level: " <<scope_level <<endl;
        // INSERT CODE HERE
	}
	else
	{
		cout << "Dump of the entire symbol table." << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout<<" "<<endl;
		while(scope_temp>=0){
			cout<<endl;
			cout<<"Current scope level: " <<scope_temp<<endl;
			dump_tree(sym_table[scope_temp]);
			scope_temp--;
		}
	// INSERT CODE HERE
	}
}


void id_table::init_table() {
	symbol* news = new symbol(symbol::identifier);
	token* place = new token(news,0,0);
	place->set_identifier_value("PROGRAM_HEADER");
   id_table_entry* id_te = new id_table_entry(place);
//   sym_table[0] = new node(id_te);

   token* predef_f;
   token* argument;
   symbol* predef_s;
   id_table_entry* func_id;
   id_table_entry* param_id;

   // for int2real
   predef_s = new symbol(symbol::identifier);
   predef_f = new token(predef_s, 0, 0);
   predef_f->set_identifier_value("INT2REAL");
   func_id = enter_id(predef_f, lille_type::type_func, lille_kind::unknown, lille_type::type_real);
   
   argument = new token(predef_s,0,0);
   argument->set_identifier_value("__int2real_arg__");
   param_id = new id_table_entry(argument, lille_type::type_integer, lille_kind::value_param, lille_type::type_unknown);


   // for real2int
   predef_s = new symbol(symbol::identifier);
   predef_f = new token(predef_s, 0, 0);
   predef_f->set_identifier_value("REAL2INT");
   func_id = enter_id(predef_f, lille_type::type_func, lille_kind::unknown, lille_type::type_integer);

   argument = new token(predef_s,0,0);
   argument->set_identifier_value("__real2int_arg__");
   param_id = new id_table_entry(argument, lille_type::type_real, lille_kind::value_param, lille_type::type_unknown);



   // for int2string
   predef_s = new symbol(symbol::identifier);
   predef_f = new token(predef_s, 0, 0);
   predef_f->set_identifier_value("INT2STRING");
   func_id = enter_id(predef_f, lille_type::type_func, lille_kind::unknown, lille_type::type_string);

   argument = new token(predef_s,0,0);
   argument->set_identifier_value("__int2string_arg__");
   param_id = new id_table_entry(argument, lille_type::type_integer, lille_kind::value_param, lille_type::type_unknown);



   // for real2string
   predef_s = new symbol(symbol::identifier);
   predef_f = new token(predef_s, 0, 0);
   predef_f->set_identifier_value("REAL2STRING");
   func_id = enter_id(predef_f, lille_type::type_func, lille_kind::unknown, lille_type::type_string);

   argument = new token(predef_s,0,0);
   argument->set_identifier_value("__real2string_arg__");
   param_id = new id_table_entry(argument, lille_type::type_real, lille_kind::value_param, lille_type::type_unknown);



}


