#include "id.h"

Id::Id() { //default
    declare = false;
    assign = false;
}

std::string Id::get_name()
{
    return name;
}

Lex_type Id::get_type()
{
    return type;
}

double Id::get_value()
{
    return value;
}

bool Id::get_assign()
{
    return assign;
}

bool Id::get_declare()
{
    return declare;
}

void Id::put_name(const char *nm)
{
    name = std::string(nm);
}

void Id::put_type(Lex_type tp)
{
    type = tp;
}

void Id::put_value(double val)
{
    value = val;
}

void Id::put_assign()
{
    Id::assign = true;
}
void Id::put_declare()
{
    declare = true;
}
