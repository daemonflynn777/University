#ifndef RPN_h
#define RPN_h

#include "lexem.h"
#include "table.h"
#include <vector>
#include <stdlib.h>

extern Table TID;

class RPN
{
    std::vector <Lexem> RPN_array;
    int used_mem = 0;
public:
    RPN();
    ~RPN();
    void add_lexem(Lexem Lex);
    void add_lexem(Lexem Lex, int i);
    int get_free_index();
    void print();
    void skip_lexem();

    Lexem & operator[] (int index);
};
#endif
