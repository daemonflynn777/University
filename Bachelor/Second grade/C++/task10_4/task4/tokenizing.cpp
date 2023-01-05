#include "tokenizing.h"

Tokenizer::Tokenizer(const char * _program){
    std::cout << _program << ":" <<std::endl;
    program.open(_program);
    if (!program)
    {
        throw("Tokenizer: ERROR! Can't open program file");
    }
    clear();
    c = _getchar();
}

void Tokenizer::clear() //Clear buf
{
    buf_top = 0;
    for (int i = 0; i < N; i++)
        buf[i] = '\0';
}

char Tokenizer::_getchar() //Get char from program
{
    return(program.get());
}

bool Tokenizer::get_lex(Lexem & lex) //Get lexem from stream
{
    stage = &Tokenizer::stage_main;
    while (c != EOF)
    {
        if (!(this->*stage)(c))
        {
            lex = Lexem(type, value, exec_str, exec_char);
            std::cout << buf << std::endl;
            clear();
            return true;
        }
        c = _getchar();
    }
    if (c == EOF)
    {
        type = LEX_EOF;
        lex = Lexem(type, value, exec_str, exec_char);
        return true;
    }
    return false;
}

int Tokenizer::get_name(const char *buf, char **list) //Get name of lexem
{
    int i = 0;
    while (list[i])
    {
        if (!strcmp(buf, list[i]))
            return i;
        i++;
    }
    return 0;
}

double Tokenizer::str2double(char * str)//Redoing string into double
{
    double res = 0.0;
    int i = 0;
    int e = 0;

    while ((*(str + i) != '\0') && (*(str + i) >= '0') && (*(str + i) <= '9'))
    {
        res = res * 10.0 + *(str + i) - '0';
        i++;
    }
    if (*(str + i) == '.')
    {
        i++;
        while ((*(str + i) != '\0') && (*(str + i) >= '0') && (*(str + i) <= '9'))
        {
            res = res * 10.0 + *(str + i) - '0';
            e--;
            i++;
        }
    }
    if ((*(str + i) == 'e') || (*(str + i) == 'E'))
    {
        int k = 0;
        i++;
        if (*(str + i) == '-')
        {
            i++;
            while ((*(str + i) != '\0') && (*(str + i) >= '0') && (*(str + i) <= '9'))
            {
                k = k * 10.0 + *(str + i) - '0';
                i++;
            }
            e -= k;
        }
        else
        {
            if (*(str + i) == '+')
                i++;

            while ((*(str + i) != '\0') && (*(str + i) >= '0') && (*(str + i) <= '9'))
            {
                k = k * 10.0 + *(str + i) - '0';
                i++;
            }
            e += k;
        }
    }

    while (e > 0)
    {
        res *= 10.0;
        e--;
    }
    while (e < 0)
    {
        res *= 0.1;
        e++;
    }

    return res;
}

bool Tokenizer::stage_main(char c)//Main stage
{
    if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
    {
        if (c == '\n')
        {
            exec_str++;
            exec_char = 0;
        }
    }
    else
    {
        exec_char++;
        if (isalpha(c))
        {
            clear();
            buf[buf_top++] = c;
            stage = &Tokenizer::stage_ident;
        }
        else if (isdigit(c))
        {
            buf[buf_top++] = c;
            stage = &Tokenizer::stage_num;
        }
        else if (c == '{')
        {
            stage = &Tokenizer::stage_comment;
        }
        else if (c == ':' || c == '<' || c == '>')
        {
            clear();
            buf[buf_top++] = c;
            stage = &Tokenizer::stage_comparison;
        }
        else if (c == '!')
        {
            clear();
            buf[buf_top++] = c;
            stage = &Tokenizer::stage_not_equal;
        }
        else
        {
            clear();
            buf[buf_top++] = c;
            stage = &Tokenizer::stage_dlm;
        }
    }


    return true;
}

bool Tokenizer::stage_ident(char c) //Identificator stage
{
    if (isalpha(c) || isdigit(c))
    {
        buf[buf_top++] = c;
    }
    else if ((a = get_name(buf, TW)))
    {
        type = words[a];
        value = a;
        return false;
    }
    else
    {

        a = TID.add_id(buf);
        type = LEX_ID;
        value = a;
        return false;
    }

    return true;
}

bool Tokenizer::stage_num(char c) //Number stage
{
    if (isdigit(c))
    {
        buf[buf_top++] = c;
    }
    else
        if (c == '.')
        {
            buf[buf_top++] = c;
            stage = &Tokenizer::stage_realnum;
        }
        else
        {
            type = LEX_NUM;
            b = atoi(buf);
            value = b;
            return false;
        }

    return true;
}

bool Tokenizer::stage_realnum(char c) //Real number stage
{
    if (isdigit(c))
    {
        buf[buf_top++] = c;
    }
    else
        if (buf[buf_top - 1] == '.')
        {
            std::cout << exec_str << " - " << exec_char << " : ";
            throw(" in real");
        }
        else
        {
            type = LEX_REAL;
            d = str2double(buf);
            value = d;
            return false;
        }
    return true;

}

bool Tokenizer::stage_comment(char c) //Comemnt stage
{
    if (c == '}')
    {
        stage = &Tokenizer::stage_main;
    }
    else
        if (c == '{')
        {
            std::cout << exec_str << " - " << exec_char << " : ";
            throw unknown_symbol((char*)buf);
        }

    return true;
}

bool Tokenizer::stage_comparison(char c) //>, < or =
{
    if (c == '=')
    {
        buf[buf_top++] = c;
        a = get_name(buf, TD);
        type = dlms[a];
        value = a;
        Tokenizer::c = _getchar();
        return false;
    }
    else
    {
        a = get_name(buf, TD);
        type = dlms[a];
        value = a;
        Tokenizer::c = _getchar();
        return false;
    }
    return true;
}

bool Tokenizer::stage_not_equal(char c) //!=
{
    if (c == '=')
    {
        buf[buf_top++] = c;
        a = get_name(buf, TD);
        type = dlms[a];
        value = a;
        Tokenizer::c = _getchar();
        return false;
    }
    else
    {
        std::cout << exec_str << " - " << exec_char << " : ";
        throw(" in '!' ");
    }
    return true;
}

bool Tokenizer::stage_dlm(char c) //Delimiter stage
{
    if ((a = get_name(buf, TD)))
    {
        type = dlms[a];
        value = a;
        return false;
    }
    else
    {
        std::cout << exec_str << " - " << exec_char << " : ";
        throw unknown_symbol((char*)buf);
    }
    return true;
}

char* Tokenizer::TW[] = //Table of words
{
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
    nullptr
};

Lex_type Tokenizer::words[] = //Name of words
{
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
    LEX_NULL
};

char* Tokenizer::TD[] = //Table of delimiters
{
    (char*)"",
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
    nullptr
};

Lex_type Tokenizer::dlms[] = //Name of delimiters
{
    LEX_NULL,
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
    LEX_NULL
};
