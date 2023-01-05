#ifndef parser_h
#define parser_h

#include <stack>
#include "lexem.h"
#include "tokenizing.h"
#include "table.h"
#include "RPN.h"
#include "error.h"

class Parser
{
    Lexem cur_lex;
    Lex_type cur_type;
    double cur_val;
    Tokenizer tokenizer;

    std::stack <int> var_stack;        //Stack of variables
    std::stack <Lex_type>  lexem_stack;  //Stack of lexem
    std::stack <int> prev_label;    //Stack for previous labels
    std::stack <int> next_label;    //Stack for next labels
    std::stack <int> st_break;      //Stack for break
    std::stack <int> st_continue;   //Stack for continue

    int cycles = 0;

    void get_lex();

    void START();   //Start of program
    void VARPART(); //Var part of program
    void DESCR();   //Desciption of variables
    void MAIN();    //Main part of program
    void OP();      //Operator
    void FULLEXPR();//Full expression
    void EXPR();    //Expression
    void TERM();    //Term
    void FACTOR();  //Factor

    void check_declared (Lex_type type);//Check variable if declared
    void check_not_declared();      //Check variable if not declared
    void check_ops();   //Check operands types in expression
    void check_not();   //Check operand with "not"
    void check_eq();    //Check operands type in assignment
    void check_bool();  //Check boolean in assignment
    void check_read();  //Check variable if not declared in read
public:
    RPN _RPN;

    void analyze();
    Parser(const char * program);
};
#endif
