#ifndef error_h
#define error_h

#include <iostream>
#include <exception>
#include <fstream>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "lexem.h"

class unknown_symbol : public std::exception
{
    char *message; //Error message
public:
    unknown_symbol(const char* bf);
    virtual const char* what() const noexcept {return message;}; //Return string with error message
};


class expected_error : public std::exception
{
    char *message; //Error message
public:
    int _str, _stlb;
    expected_error(Lex_type lt, int str, int stlb);
    virtual const char* what() const noexcept{return message;}; //Return string with error message
};

class semantic_error : public std::exception
{
    char *message; //Error message
public:
    int _str,_stlb;
    semantic_error(const char* a, int str, int stlb);
    virtual const char* what() const noexcept{return message;}; //Return string with error message
};
#endif
