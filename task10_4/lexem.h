#ifndef lexem_h
#define lexem_h

#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <fstream>

#define N 100

enum Lex_type { //all possible types of lexems
    LEX_NULL,
    LEX_AND,
    LEX_BEGIN,
    LEX_BREAK,
    LEX_BOOL,
    LEX_CONTINUE,
    LEX_DO,
    LEX_ELSE,
    LEX_END,
    LEX_IF,
    LEX_FALSE,
    LEX_FOR,
    LEX_INT,
    LEX_NOT,
    LEX_OR,
    LEX_PROGRAM,
    LEX_READ,
    LEX_REAL,
    LEX_THEN,
    LEX_TRUE,
    LEX_TO,
    LEX_VAR,
    LEX_WHILE,
    LEX_WRITE,
    LEX_EOF,
    LEX_SEMICOLON,
    LEX_COMMA,
    LEX_COLON,
    LEX_ASSIGN,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_EQ,
    LEX_LSS,
    LEX_GTR,
    LEX_PLUS,
    LEX_MINUS,
    LEX_MULT,
    LEX_SLASH,
    LEX_LEQ,
    LEX_NEQ,
    LEX_GEQ,
    LEX_NUM,
    LEX_REALNUM,
    LEX_ID,
    RPN_LABEL,
    RPN_ADDRESS,
    RPN_GO,
    RPN_FGO
};


static char* lexem_name[] = { //all possible lexems
    (char*)"",
    (char*)"and",
    (char*)"begin",
    (char*)"break",
    (char*)"bool",
    (char*)"continue",
    (char*)"do",
    (char*)"else",
    (char*)"end",
    (char*)"if",
    (char*)"false",
    (char*)"for",
    (char*)"int",
    (char*)"not",
    (char*)"or",
    (char*)"program",
    (char*)"read",
    (char*)"real",
    (char*)"then",
    (char*)"true",
    (char*)"to",
    (char*)"var",
    (char*)"while",
    (char*)"write",
    (char*)"EOF",
    (char*)";",
    (char*)",",
    (char*)":",
    (char*)":=",
    (char*)"(",
    (char*)")",
    (char*)"=",
    (char*)"<",
    (char*)">",
    (char*)"+",
    (char*)"-",
    (char*)"*",
    (char*)"/",
    (char*)"<=",
    (char*)"!=",
    (char*)">=",
    (char*)"NUM",
    (char*)"REAL_NUM",
    (char*)"ID",
    (char*)"LABEL",
    (char*)"ADDRESS",
    (char*)"GO",
    (char*)"FGO",
    nullptr
};

class Lexem
{
    Lex_type t_lex;
    double v_lex;
public:
    int str_lex;
    int num_lex; //Order of lexem

    Lexem(Lex_type t = LEX_NULL, double v = 0, int str = 0, int num = 0);
    Lex_type get_type();
    double get_value();
    friend std::ostream & operator<< (std::ostream &s, Lexem l);
};
#endif
