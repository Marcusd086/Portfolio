#include <iostream>
#include <string>
//#include "token.h"
//#include "lille_type.h"
//#include "lille_kind.h"
//#include "id_table.h"
#include "id_table_entry.h"

using namespace std;

id_table_entry::id_table_entry(){
	id_entry = NULL;
	typ_entry = lille_type::type_unknown;
	kind_entry = lille_kind::unknown;
	lev_entry = 0;
	offset_entry = 0;
	r_ty_entry = lille_type::type_unknown;
	trace_entry = false;
	i_val_entry = 0;
	r_val_entry = 0.0;
	s_val_entry = "";
	b_val_entry = false;
	p_list_entry = NULL;
	n_par_entry = 0;
}

id_table_entry::id_table_entry(token* id, lille_type typ, lille_kind kind, int level, int offset, lille_type return_tipe){
	id_entry = id;
        typ_entry = typ;
        kind_entry = kind;
        lev_entry = level;
        offset_entry = offset;
        r_ty_entry = return_tipe;
	trace_entry = false;
        i_val_entry = 0;
        r_val_entry = 0.0;
        s_val_entry = "";
        b_val_entry = false;
        p_list_entry = NULL;
        n_par_entry = 0;
}
 
bool id_table_entry::trace(){
	return trace_entry;
}

int id_table_entry::offset(){
	return offset_entry;
}

int id_table_entry::level(){
	return lev_entry;
}

lille_kind id_table_entry::kind(){
	return kind_entry;
}

lille_type id_table_entry::tipe(){
	return typ_entry;
}

token* id_table_entry::token_value(){
	return id_entry;
}

string id_table_entry::name(){
	return id_entry->get_identifier_value();
}

int id_table_entry::integer_value(){
	return i_val_entry;
}

float id_table_entry::real_value(){
	return r_val_entry;
}

string id_table_entry::string_value(){
	return s_val_entry;
}

bool id_table_entry::bool_value(){
	return b_val_entry;
}

lille_type id_table_entry::return_tipe(){
	return r_ty_entry;
}

void id_table_entry::fix_const(int integer_value, float real_value, string string_value, bool bool_value){
	if (typ_entry.is_type(lille_type::type_integer))
		i_val_entry = integer_value;
	else if (typ_entry.is_type(lille_type::type_real))
		r_val_entry = real_value;
	else if (typ_entry.is_type(lille_type::type_string))
		s_val_entry = string_value;
	else if (typ_entry.is_type(lille_type::type_boolean))
		b_val_entry = bool_value;
}

void id_table_entry::fix_return_type(lille_type ret_ty){
	this->r_ty_entry = ret_ty;
}

void id_table_entry::add_param(id_table_entry* param_entry){
	if (typ_entry.is_type(lille_type::type_func) or typ_entry.is_type(lille_type::type_proc)){
		id_table_entry* p = this->p_list_entry;
		id_table_entry* q;
		if(p==NULL)
			this->p_list_entry = param_entry;
		else
		{
			while (p != NULL)
			{
				q = p;
				p = p->p_list_entry;
			}
			q->p_list_entry = param_entry;
		}
		n_par_entry++;
	}
	else
		throw lille_exception("Internal compiler error - adding parameter to something other than a procedure or function.");
}

id_table_entry* id_table_entry::nth_parameter(int n){
	id_table_entry* p = this->p_list_entry;
	id_table_entry * q;
	if(n<=n_par_entry)
	{
		for(int x = 0; x < n; x++)
		{
			q = p;
			p = p->p_list_entry;
		}
		return p;
	}
	else
		throw lille_exception("Internal compiler error - attempting to read past end of parameter list!");

}

int id_table_entry::number_of_params(){
        return n_par_entry;
}

string id_table_entry::to_string(){
	if(typ_entry.is_type(lille_type::type_integer))
		return std::to_string(i_val_entry);
	else if(typ_entry.is_type(lille_type::type_real))
		return std::to_string(r_val_entry);
	else if(typ_entry.is_type(lille_type::type_string))
		return s_val_entry;
	else if(typ_entry.is_type(lille_type::type_boolean))
		if(b_val_entry)
			return "True";
		else
			return "False";
	else
		throw lille_exception("Internal compiler error - type cannot be converted to a string!");
}






