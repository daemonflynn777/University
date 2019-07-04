#include <stdlib.h>
#include <string.h>
#include "list.h"
#define SIZE 16

void null_list()
{
 sizelist = 0;
 curlist = 0;
 lst = NULL;
}

void clear_list()
{
  int i;
  sizelist = 0;
  curlist = 0;
  if (lst == NULL)
      return;
  for (i = 0; lst[i] != NULL; i++)
      free(lst[i]);
  free(lst);
  lst = NULL;
}

void termlist()
{
    if (lst == NULL)
        return;
    if (curlist > sizelist - 1)
        lst = realloc(lst,(sizelist + 1)*sizeof(*lst));
    lst[curlist] = NULL;
    lst = realloc(lst,(sizelist = curlist + 1)*sizeof(*lst));
}

void nullbuf()
{
  buf = NULL;
  sizebuf = 0;
  curbuf = 0;
}

void addsym()
{
  if (curbuf > sizebuf - 1)
      buf = realloc(buf, sizebuf += SIZE);
  buf[curbuf++] = c;
}

int symset(int c)
{
 return c != '\n' &&
        c != ' ' &&
        c != '\t' &&
        c != '>' &&
        c != '|' &&
        c != '&' &&
        c != ';' &&
        c != '<' &&
        c != '(' &&
        c != ')' &&
        c != EOF ;
}

int error_list(int c)
{
    char yes_symbols[76] = "\n\t ><&;|()1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM$_/.";
    for (int i = 0; i < 76; i++)
    {
      if (c == yes_symbols[i])
        return 0;
    }
    return 1;
}


void addword()
{
 if (curbuf > sizebuf - 1)
    buf = realloc(buf, sizebuf += 1);
 buf[curbuf++] = '\0';
 buf = realloc(buf,sizebuf = curbuf);
 if (curlist > sizelist - 1)
    lst = realloc(lst, (sizelist += SIZE)*sizeof(*lst));
 lst[curlist++] = buf;
}

void print_list(list l)
{
  if (l == NULL) return;
  while (*l != NULL)
  printf("%s\n",*l++);
}

char read_symbol()
{
    if (strcount >  length - 1)
    {
      if ((length < SIZE) && (length == strcount))
        return EOF;
      length = fread(str, sizeof(char), SIZE, stdin);
      if (length == 0)
        return EOF;
      strcount = 0;
    }
    return str[strcount++];
}

list build_list()
{
  typedef enum {Start, Word, Greater, Greater2, Newline, Stop, Otherspec, Ampersand, Ampersand2, Stick, Stick2, Error} vertex;
  vertex V = Start;
  c = read_symbol();
  null_list();
  while(1 == 1) switch(V)
  {
      case Start:
      if(c == ' ' || c == '\t')
          c = read_symbol();
      else if (c == EOF)
      {
          termlist();
          if (lst)
          {
            printf("List size is %d\n", sizelist - 1);
            printf("Here is your list \n");
          }
          V = Stop;
      }
      else if (c == '\n')
      {
          termlist();
          if (feof(stdin) == 0)
          {
            printf("\nSize of the list: %d\n", sizelist-1);
            printf("Your result: \n");
          }
          V = Newline;
      }
      else if (error_list(c))
          V = Error;
      else
      {
          nullbuf();
          addsym();
          if (c == '>')
                V = Greater;
          else if (c == '|')
                V = Stick;
          else if (c == '&')
                V = Ampersand;
          else if ((c == ';') || (c == '<') || (c == '(') || (c == ')'))
                V = Otherspec;
          else if (error_list(c) == 0)
                V = Word;
          else
          {
                printf("Oops!!! ERROR! Wrong input! \n");
                termlist();
                clear_list();
                V = Error;
                strcount = SIZE;
                length = -1;
                printf("Try again ASAP!!!\n\n");
                break;
          }
          c = read_symbol();
      }
      break;

      case Word:
      if (error_list(c) == 0)
      {
            if (symset(c))
            {
                addsym();
                c = read_symbol();
            }
            else
            {
                V = Start;
                addword();
            }
       }
       else
       {
            printf("ERROR! Wrong symbol! \n");
            termlist();
            clear_list();
            strcount = SIZE;
            printf("Enter again\n\n");
            V = Error;
        }
      break;

      case Greater:
      if(c == '>')
      {
          addsym();
          c = read_symbol();
          V = Greater2;
      }
      else
      {
          V = Start;
          addword();
      }
      break;

      case Greater2:
      V = Start;
      addword();
      break;

      case Stick:
      if (c == '|')
      {
            addsym();
            c = read_symbol();
            V = Stick2;
      }
      else
      {
            V = Start;
            addword();
      }
      break;

     case Stick2:
        V = Start;
        addword();
     break;

     case Ampersand:
        if (c == '&')
        {
            addsym();
            c = read_symbol();
            V = Ampersand2;
        }
        else
        {
            V = Start;
            addword();
        }
        break;

        case Ampersand2:
            V = Start;
            addword();
        break;

        case Otherspec:
            addword();
            V = Start;
        break;

      case Newline:
      return lst;
      break;

      case Stop:
      return lst;
      break;

      case Error:
      termlist();
      clear_list();
      return NULL;
      break;
    }
}

void sort_list()
{
    char *tmp;
    int i, j;
    for (i = 0; i < curlist; ++i)
    {
        for (j = 0; j < curlist - 1; ++j)
        {
            if (strcmp(lst[j], lst[j + 1]) > 0)
            {
                tmp = lst[j];
                lst[j] = lst[j + 1];
                lst[j + 1] = tmp;
            }
        }

    }
}
