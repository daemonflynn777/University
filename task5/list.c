#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include "list.h"
#define SIZE 5

list lst = NULL;
static int c;
static char *buf;
static int sizebuf;
static int sizelist;
static int curbuf;
static int curlist;
static char str[SIZE];
static int strcount = 0;
static int realloc_extension = 0;
static int quotes_count = 0;
int EOF_flag = 0;
int oshibka;
char *path;
int sl, quote;
extern char **environ;

void error(int , char *, list);

void destruct()
{
  int no_enter = 1;
  for (int i = 0; i < SIZE; i++)
  {
      if ((str[i] == '\n') || (str[i] == '\0'))
      {
          no_enter = 0;
          break;
      }
  }
  if (no_enter)
      while (((c = getchar()) != EOF) && (c != '\n'));
  rewind(stdin);
}

void null_list()
{
  sizelist = 0;
  curlist = 0;
  lst = NULL;
}

void clear_list(list l)
{
  int i;
  sizelist = 0;
  curlist = 0;
  if (l == NULL)
      return;
  for (i = 0; l[i] != NULL; i++)
  {
      free(l[i]);
      l[i] = NULL;
  }
  free(l);
  l = NULL;
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

void addsym(char d)
{
   if (curbuf != 0)
   {
       if (curbuf == (SIZE * realloc_extension))
           buf = realloc(buf, (++realloc_extension * SIZE));
       buf[curbuf++] = d;
   }
   else
   {
       buf = calloc(SIZE, sizeof(char));
       buf[curbuf++] = d;
   }
}

/* static int symset(int c)
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
        c != '"' &&
        c != '#' &&
        c != '\\' &&
        c != EOF ;
} */

int error_list(int c)
{
    char yes_symbols[81] = "-#\n\t \\><&;|()1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM$_/.";
    yes_symbols[80] = '"';
    yes_symbols[81] = EOF;
    for (int i = 0; i < 81; i++)
    {
      if (c == yes_symbols[i])
        return 0;
    }
    return 1;
}

char *sub_string(char *s, char *reference, char *sub_str, int t)
{
    static char B1[4096], B2[4096], B3[4096], B4[4096];
    char *p;
    char *rmv_str = NULL;
    p = rmv_str;
    if (!(p = strstr(s, reference)))
        return s;
    if (t == 1)
    {
        strncpy(B1, s, p - s + 1);
        B1[p - s] = '\0';
        sprintf(B1 + (p - s), "%s%s", sub_str, p + strlen(reference));
        return B1;
    }

    else if (t == 2)
    {
        strncpy(B2, s, p - s + 1);
        B2[p - s] = '\0';
        sprintf(B2 + (p - s), "%s%s", sub_str, p + strlen(reference));
        return B2;
    }

    else if (t == 3)
    {
        strncpy(B3, s, p - s + 1);
        B3[p - s] = '\0';
        sprintf(B3 + (p - s), "%s%s", sub_str, p + strlen(reference));
        return B3;
    }

    else
    {
        strncpy(B4, s, p - s + 1);
        B4[p - s] = '\0';
        sprintf(B4 + (p - s), "%s%s", sub_str, p + strlen(reference));
        return B4;
    }
}

void add_word_lite(char *b)
{
    if (curlist > sizelist - 1)
        lst = realloc(lst, (sizelist += SIZE) * sizeof(*lst));
    lst[curlist++] = b;
    buf = NULL;
}

void addword()
{
  int check1 = 0;
  int check2 = 0;
  if ((curbuf != 0) && (quotes_count != 1))
  {
      if (curbuf == (SIZE * realloc_extension))
          buf = realloc(buf, (realloc_extension) * SIZE + 1);
      buf[curbuf++] = '\0';
      buf = realloc(buf, curbuf);
      if (strchr(buf, '$') != NULL)
      {
          while ((strstr(buf, "$HOME") != NULL) ||
                (strstr(buf, "$SHELL") != NULL) ||
                 (strstr(buf, "$USER") != NULL) ||
                 (strstr(buf, "$EUID") != NULL))
                 {
                      char *tmp = NULL;
                      if (strstr(buf, "$SHELL") != NULL)
                      {
                          tmp = sub_string(buf, "$SHELL", getenv("SHELL"), 1);
                          check1 = 1;
                      }
                      else
                          tmp = buf;
                      if (strstr(buf, "$USER") != NULL)
                      {
                          tmp = sub_string(tmp, "$USER", getenv("USER"), 2);
                          check2 = 1;
                      }
                      else if (check1 == 0)
                          tmp = buf;
                      if (strstr(buf, "$HOME") != NULL)
                          tmp = sub_string(tmp, "$HOME", getenv("HOME"), 3);
                      else if ((check1 == 0) && (check2 == 0))
                          tmp = buf;
                      if (strstr(buf, "$EUID") != NULL)
                      {
                          char timed[4];
                          sprintf(timed, "%u", getuid());
                          tmp = sub_string(tmp, "$EUID", timed, 4);
                      }
                free(buf);
                buf = NULL;
                curbuf = 0;
                realloc_extension = 1;
                for (int i = 0; tmp[i] != '\0'; i++)
                    addsym(tmp[i]);
                if (curbuf == (SIZE * realloc_extension))
                    buf = realloc(buf, (realloc_extension) * SIZE + 1);
                buf[curbuf++] = '\0';
                buf = realloc(buf, curbuf);
                }
      }
      add_word_lite(buf);
      curbuf = 0;
      realloc_extension = 1;
  }
}

void print_list(list l)
{
  if (l == NULL)
      return;
  while (*l != NULL)
      printf("%s\n", *l++);
}

int read_symbol(char f_str[SIZE],char p_c)
{
    if (p_c != '\0')
        return p_c;
    if ((strcount == SIZE) || ((curbuf > 0) && (buf[curbuf - 1] == '\n')))
    {
        fgets(str, SIZE + 1, stdin);
        strcount = 0;
    }
    if (f_str[strcount] != '\0')
        return f_str[strcount++];
    else
        return EOF;
}

void  build_list(void)
{
    char b, c;
    char pair_symbol = '\0';
    int cycle = 1;
    int quote = 0;
    int single_quote = 0;
    int sub_quote = 0;
    int single_sub_quote = 0;
    int bslash_flg = 0;
    buf = NULL;
    curbuf = 0;
    realloc_extension = 1;
    for (int i = 0; i < SIZE; i++)
        str[i] = '\0';
    if (EOF_flag == 1)
    {
        lst = NULL;
        return;
    }
    rewind(stdin);
    while (cycle != 0)
    {
        cycle = 0;
        strcount = 0;
        null_list();
        fgets(str, SIZE + 1, stdin);
        while ((b = read_symbol(str, pair_symbol)) != EOF)
        {
            pair_symbol = '\0';
            if (bslash_flg == 1)
            {
                bslash_flg = 0;
                addsym(b);
            }
            else if ((b == '&') ||
                     (b == '|') ||
                     (b == ';') ||
                     (b == '>') ||
                     (b == '\'') ||
                     (b == '<') ||
                     (b == ')') ||
                     (b == '(') ||
                     (b == '#') ||
                     (b == '"'))
                     {
                          if (((quote == 1) || (single_quote == 1)) && ((b != '"') && (b != '\'')))
                              addsym(b);
                          else
                          {
                              addword();
                              if ((b == '"') || (b == '\''))
                              {
                                  if ((b == '"') && (single_quote != 1) && (single_sub_quote == 0))
                                  {
                                      if (quote == 0)
                                          quote = 1;
                                      else
                                          quote = 0;
                                      if (sub_quote == 0)
                                          sub_quote += 1;
                                      else
                                          sub_quote -= 1;
                                  }
                                  else if ((b == '\'') && (quote != 1) && (sub_quote == 0))
                                  {
                                      if (single_quote == 0)
                                          single_quote = 1;
                                      else
                                          single_quote = 0;
                                      if (single_sub_quote == 0)
                                          single_sub_quote += 1;
                                      else
                                          single_sub_quote -= 1;
                                  }
                                  else if ((b == '\'') && (quote == 1) && (sub_quote > 0))
                                  {
                                      quotes_count = 1;
                                      addsym(b);
                                  }
                                  else if ((b == '"') && (single_quote == 1) && (single_sub_quote > 0))
                                  {
                                      quotes_count = 1;
                                      addsym(b);
                                  }
                              }
                              else if (b == '#')
                              {
                                  if ((quote == 1) || (single_quote == 1))
                                      addsym(b);
                                  else if ((str[SIZE - 1] == '\n') || (str[SIZE - 1] == '\0'))
                                      break;
                                  else if (strstr(str, " #") != NULL)
                                  {
                                      while (b != '\n')
                                      b = getchar();
                                      break;
                                  }
                                  else
                                      addsym(b);
                                }
                                else
                                {
                                    buf = calloc(2, sizeof(char));
                                    buf[0] = b;
                                    buf[1] = '\0';
                                    if  ((b == '&') || (b == '|') || (b == '>'))
                                    {
                                        c  = read_symbol(str, pair_symbol);
                                    if (b == c)
                                    {
                                        buf[1] = c;
                                        buf = realloc(buf, 3);
                                        buf[2] = '\0';
                                    }
                                    else
                                        pair_symbol = c;
                                    }
                                add_word_lite(buf);
                                }
                            }
                      }
            else if (b == '\\')
            {
                 if (single_quote == 1)
                    addsym(b);
                 else
                 {
                    /* В случае \ проверяем следующий */
                    printf(">");
                    c  = read_symbol(str, pair_symbol);
                    if (b == c)
                        /* Получили \\ */
                        addsym(b);
                    else
                    {
                        pair_symbol = c;
                        if ((quote == 0) && (quote == 0))
                            bslash_flg = 1;
                        else if (quote == 1)
                        {
                            bslash_flg = 1;
                            if ((c != '$') && (c != '"'))
                                addsym(b);
                        }
                    }
                }
            }
            else if ((b == ' ') && ((quote == 1) || (single_quote == 1)))
                addsym(b);
            else if ((b == ' ') || (b == '\n') || (b == '\t'))
            {
                addword();
                quotes_count = 0;
                if (b == '\n')
                    break;
            }
            else if (((b >= 'a') && (b <= 'z')) ||
                     ((b >= 'A') && (b <= 'Z')) ||
                     ((b >= '0') && (b <= '9')) ||
                     (b == '$') ||
                     (b == '_') ||
                     (b == '/') ||
                     (b == '.') ||
                     (b == '-') ||
                     (b == ':') ||
                     (b == '!') ||
                     (b == ','))
                          addsym(b);
            else
            {
                cycle = 1;
                quote = 0;
                single_quote = 0;
                bslash_flg = 0;
                sub_quote = 0;
                single_sub_quote = 0;
                free(buf);
                termlist();
                clear_list(lst);
                null_list();
                buf = NULL;
                curbuf = 0;
                realloc_extension = 1;
                int clear_stdin = 1;
                for (int i = 0; i < SIZE; i++)
                {
                    if ((str[i] == '\n') || (str[i] == '\0'))
                    {
                        clear_stdin = 0;
                        break;
                    }
                }
                for (int i = 0; i < SIZE; i++)
                    str[i] = '\0';
                if (clear_stdin)
                    while (((b = getchar()) != EOF) && (b != '\n'));
                rewind(stdin);
                error(1, NULL, NULL);
                printf("DIGINick:$ ");
            }
        }
        if ((quote == 1) ||
        (sub_quote == 1) ||
        (single_quote == 1) ||
        (single_sub_quote == 1))
        {
            cycle = 1;
            quote = 0;
            single_quote = 0;
            sub_quote = 0;
            single_sub_quote = 0;
            bslash_flg = 0;
            free(buf);
            termlist();
            clear_list(lst);
            null_list();
            buf = NULL;
            curbuf = 0;
            realloc_extension = 1;
            int clear_stdin = 1;
            for (int i = 0; i < SIZE; i++) {
                if ((str[i] == '\n') || (str[i] == '\0'))
                {
                    clear_stdin = 0;
                    break;
                }
            }
            for (int i = 0; i < SIZE; i++)
                str[i] = '\0';
            if (clear_stdin)
                while (((b = getchar()) != EOF) && (b != '\n'));
            rewind(stdin);
            error(1, NULL, NULL);
            printf("DIGINick:$ ");
        }
    }
    if (buf != NULL)
        addword();
    termlist();
    sizelist = 0;
    curlist = 0;
    if (b == EOF)
        EOF_flag = 1;
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
