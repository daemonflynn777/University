#include "RPN.h"

RPN::RPN(){ //default
}

RPN::~RPN(){ //destructor
    RPN_array.clear();
}

void RPN::add_lexem(Lexem Lex) //Add lexem into end of array
{
    RPN_array.push_back(Lex);
    used_mem ++;
}

void RPN::add_lexem(Lexem Lex, int i) //Add lexem to the i-position
{
    RPN_array[i] = Lex;
}

void RPN::skip_lexem()
{
    RPN_array.push_back(Lexem(LEX_NULL));
    used_mem++;
}

void RPN::print()
{

    std::cout << std::endl;
    std::cout << "RPN table:" << std::endl;
    for (int i = 0; i < used_mem; ++i)
    {
        std::cout.width(4);
        std::cout << i;
        std::cout << ": ";
        std::cout << RPN_array[i];
        std::cout << "  ";
        std::cout << lexem_name[RPN_array[i].get_type()];
        if (RPN_array[i].get_type() == LEX_ID || RPN_array[i].get_type() == RPN_ADDRESS)
        {
            std::cout << "  " << TID[RPN_array[i].get_value()].get_name();
        }
        std::cout << std::endl;
    }
}

int RPN::get_free_index()
{
    return used_mem;
}

Lexem& RPN::operator[] (int index)
{
    if (index > used_mem)
        throw "RPN: out of memory";
    else
        return RPN_array[index];
}
