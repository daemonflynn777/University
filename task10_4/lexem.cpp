#include "tokenizing.h"

Lexem::Lexem(Lex_type t, double v , int str , int num): t_lex(t), v_lex(v), str_lex(str), num_lex(num){ //constructor
}

Lex_type Lexem::get_type()
{
    return t_lex;
}

double Lexem::get_value()
{
    return v_lex;
}

std::ostream & operator<< (std::ostream &s, Lexem l)
{
    s << '(' << l.t_lex << ',' << l.v_lex << ");";
    return s;
}
