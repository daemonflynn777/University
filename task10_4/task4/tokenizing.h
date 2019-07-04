#ifndef tokenizing_h
#define tokenizing_h

#include <stack>
#include "lexem.h"
#include "table.h"
#include "error.h"

#define N 100

extern Table TID;

class Tokenizer
{
    static char *TW[];     //Table of words
    static Lex_type words[];//Name of words
    static char *TD[];     //Table of delimiters
    static Lex_type dlms[]; //Name of delimiters
    std::ifstream program;  //Program in stream to execute
    char buf[N];            //Buffer of chars
    int buf_top;            //Index of top of buffer
    int exec_str = 1;       //Number of string to analyze
    int exec_char = 0;      //Number of char to analyze
    int a, b;   //Integer variable
    char c;     //Char variable
    double d;   //Double variable
    Lex_type type = LEX_NULL;//Type of lexem
    double value = 0;        //Value of lexem

    void clear();   //Clear buffer
    char _getchar();//Get char from program
    int get_name(const char* buf, char ** list);//Get name of lexem
    double str2double(char * str); //Redoing string into double

    typedef bool (Tokenizer::*Stage)(char c);
    Stage stage = nullptr;    //Current stage of recognition

    bool stage_main(char c);   //Main stage
    bool stage_ident(char c);  //Identificator stage
    bool stage_num(char c);    //Number stage
    bool stage_realnum(char c);//Real number stage
    bool stage_comment(char c);//Comemnt stage
    bool stage_comparison(char c);    //Above, less ot equal stage
    bool stage_not_equal(char c);    //Not or equal stage
    bool stage_dlm(char c);    //Delimiter stage
public:
    Tokenizer() = default;          //Default constructor
    Tokenizer(const char * program);//Constructor
    bool get_lex(Lexem & Lex);      //Get lexem from stream
};
#endif
