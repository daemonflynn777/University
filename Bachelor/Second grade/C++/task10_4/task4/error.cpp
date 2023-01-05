#include "error.h"

unknown_symbol::unknown_symbol(const char* buf) {
    message = new char[strlen(" unknown symbol. Error") + strlen(buf) + 3];
    strcpy(message, "\"");
    strcat(message, buf);
    strcat(message, "\"");
    strcat(message, " unknown symbol. Error");
}

expected_error::expected_error(Lex_type lt, int str, int stlb): _str(str), _stlb(stlb) {
    message = new char[strlen(" Error. Expected ") + strlen(lexem_name[lt]) + 2];
    strcpy(message, " Error. Expected ");
    strcat(message, lexem_name[lt]);
}

semantic_error::semantic_error(const char* a, int str, int stlb): _str(str), _stlb(stlb) {
    message = new char[strlen("ERROR!!! ") + strlen(a) + 1];
    strcpy(message, "ERROR!!! ");
    strcat(message, a);
}
