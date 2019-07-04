#ifndef id_h
#define id_h

#include "lexem.h"

class Id
{
    std::string name;
    Lex_type type;
    double value;
    bool assign;
    bool declare;
public:
    Id();
    std::string get_name();
    Lex_type get_type();
    double get_value();
    bool get_assign ();
    bool get_declare();
    void put_name(const char *nm);
    void put_type(Lex_type tp);
    void put_value(double val);
    void put_assign ();
    void put_declare();
};
#endif
