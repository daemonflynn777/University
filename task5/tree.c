#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "tree.h"

#define SIZE 5

static char *current_lex;
static int words = 0;
static int brckts = 0;
static int *sub_cmd = NULL;
int oshibka = 0;

static tree create_cmd(void);
static void add_cmd(tree t);
static void sub_commands(void);
void rmv_br(void);
static void flush_error(void);
void error(int , char *, list *);

tree build_tree(list l)
{
    tree start_cmd;
    tree p_cmd;
    tree c_cmd;
    if ((l == NULL) || ((current_lex = l[words++]) == NULL))
    {
        words = 0;
        return NULL;
    }
    start_cmd = c_cmd = create_cmd();
    p_cmd = c_cmd;
    if ((strcmp(current_lex, ")") == 0) ||
        (strcmp(current_lex, "<") == 0) ||
        (strcmp(current_lex, ">") == 0) ||
        (strcmp(current_lex, ">") == 0) ||
        (strcmp(current_lex, ";") == 0) ||
        (strcmp(current_lex, "&") == 0) ||
        (strcmp(current_lex, "||") == 0) ||
        (strcmp(current_lex, "&&") == 0) ||
        (strcmp(current_lex, "|") == 0))
        {
            flush_error();
            error(2, "Wrong symbol!", &l);
            return c_cmd;
    }
    if (strcmp(current_lex, "(") == 0)
    {
        if (l[words] == NULL)
        {
            flush_error();
            error(2, "No ) found!", &l);
            return c_cmd;
        }
        sub_commands();
        c_cmd = build_tree(l);
        p_cmd->psubcmd = c_cmd;
        c_cmd = p_cmd;
    }
    else
    {
       if ((brckts > 0) && (c_cmd->argv == NULL))
           sub_cmd[brckts - 1]++;
        add_cmd(c_cmd);
    }
    current_lex = l[words++];
    while ((current_lex != NULL) && (oshibka == 0))
    {
        if (((strcmp(current_lex, "||") == 0) ||
             (strcmp(current_lex, "&&") == 0) ||
             (strcmp(current_lex, "|") == 0)) && (l[words] == NULL))
             {
                flush_error();
                error(2, "Missing command after ||, && or |!", &l);
                return c_cmd;
             }

        if (strcmp(current_lex, "(") == 0)
        {
            flush_error();
            error(2, "Wrong commad format before (!", &l);
            return c_cmd;
        }

        else  if (strcmp(current_lex, ")") == 0)
        {
            if ((brckts != 0) && (sub_cmd[brckts - 1] == 0))
            {
                flush_error();
                error(2, "Enter commands in brackets!", &l);
                return c_cmd;
            }
            else if (brckts == 0)
            {
                flush_error();
                error(2, "No ( found!", &l);
                return c_cmd;
            }
            if ((brckts > 0) && (sub_cmd[brckts - 1] > 1))
            {
                sub_cmd[brckts - 1]--;
                return start_cmd;
            }
            else if ((brckts > 0) && (sub_cmd[brckts - 1] == 1))
            {
                sub_cmd[brckts - 1] = 0;
                sub_cmd = realloc(sub_cmd, ((brckts - 1) * sizeof(int)));
                if (brckts == 1)
                    free(sub_cmd);
                brckts--;
                return start_cmd;
            }
        }

        else if (strcmp(current_lex, "<") == 0)
        {
            if (l[words] == NULL)
            {
                flush_error();
                error(2, "Wrong command format after < !", &l);
                return c_cmd;
            }
            current_lex = l[words++];
            if (c_cmd->infile != NULL)
            {
                flush_error();
                error(2, "Only one input file allowed!", &l);
                return c_cmd;
            }
            long tmp = strlen(current_lex);
            c_cmd->infile= calloc(tmp + 1, sizeof(char));
            strncpy(c_cmd->infile, current_lex, tmp + 1);
            current_lex = l[words++];
        }

        else if ((strcmp(current_lex, ">") == 0) || (strcmp(current_lex, ">>") == 0))
        {
            if (c_cmd->outfile != NULL)
            {
                flush_error();
                error(2, "Only one output file allowed!", &l);
                return c_cmd;
            }
            if (l[words] == NULL)
            {
                flush_error();
                error(2, "Nothing after > (>>)!", &l);
                return c_cmd;
            }
            if (strcmp(current_lex, ">>") == 0)
                c_cmd->append = 1;
            current_lex = l[words++];
            long tmp = strlen(current_lex);
            c_cmd->outfile= calloc(tmp + 1, sizeof(char));
            strncpy(c_cmd->outfile, current_lex, tmp + 1);
            current_lex = l[words++];
        }

        else if (strcmp(current_lex, "|") == 0)
        {
            c_cmd = build_tree(l);
            p_cmd->pipe = c_cmd;
            if (c_cmd->backgrnd == 1)
                p_cmd->backgrnd = 1;
            if (c_cmd->next != NULL) {
                p_cmd->next = c_cmd->next;
                c_cmd->next = NULL;
                p_cmd->type = c_cmd->type;
                c_cmd->type = NXT;
            }
            p_cmd = c_cmd;
        }

        else if (strcmp(current_lex, "||") == 0)
        {
            c_cmd = build_tree(l);
            p_cmd->next = c_cmd;
            p_cmd->type = OR;
            p_cmd = c_cmd;
        }

        else if (strcmp(current_lex, "&&") == 0)
        {
            c_cmd = build_tree(l);
            p_cmd->next = c_cmd;
            p_cmd->type = AND;
            p_cmd = c_cmd;
        }

        else if (strcmp(current_lex, ";") == 0)
        {
            if ((brckts > 0) && (l[words] != NULL) && (l[words][0] == ')'))
            {
                current_lex = l[words++];
                continue;
            }
            c_cmd = build_tree(l);
            p_cmd->next = c_cmd;
            p_cmd->type = NXT;
            p_cmd = c_cmd;
        }

        else if (strcmp(current_lex, "&") == 0)
        {
            c_cmd->backgrnd = 1;
            if ((brckts > 0) && (l[words] != NULL) && (l[words][0] == ')'))
            {
                current_lex = l[words++];
                continue;
            }
            c_cmd = build_tree(l);
            p_cmd->next = c_cmd;
            p_cmd->type = NXT;
            p_cmd = c_cmd;
        }
        else
        {
            if ((c_cmd->infile != NULL) || (c_cmd->outfile != NULL) || (c_cmd->psubcmd != NULL))
            {
                flush_error();
                error(2, "Unexpected lexem!", &l);
                return c_cmd;
            }
            add_cmd(c_cmd);
            current_lex = l[words++];
        }
    }

    if ((brckts != 0) && (current_lex == NULL))
    {
        flush_error();
        error(2, "Looks like you miss \')\'!", &l);
        return c_cmd;
    }
    words = 0;
    return start_cmd;
}


void sub_commands(void)
{
    brckts++;
    if (brckts == 1)
        sub_cmd = calloc(1, sizeof(int));
    else
        sub_cmd = realloc(sub_cmd, (brckts - 1) * sizeof(int));
    sub_cmd[brckts - 1] = 0;
    if (brckts > 1)
    {
        sub_cmd[brckts - 2]++;
    }
}

void rmv_br(void)
{
    if (brckts != 0)
    {
        brckts = 0;
        sub_cmd = realloc(sub_cmd, 0);
        free(sub_cmd);
    }
}

void flush_error(void)
{
    words = 0;
    rmv_br();
    oshibka = 1;
}

tree create_cmd()
{
    tree t = NULL;
    t = calloc(1, sizeof(*t));
    t->argv = NULL;
    t->infile = NULL;
    t->outfile = NULL;
    t->append = 0;
    t->backgrnd = 0;
    t->psubcmd = NULL;
    t->pipe = NULL;
    t->next = NULL;
    return t;
}

void add_cmd(tree t)
{
    int i = 0;
    if (t->argv == NULL)
        t->argv = calloc(1, sizeof(*t->argv));
    while (t->argv[i] != NULL)
        i += 1;
    long tmp = strlen(current_lex);
    t->argv[i] = calloc(tmp + 1, sizeof(char));
    strncpy(t->argv[i++], current_lex, tmp + 1);
    t->argv = realloc(t->argv, ((i + 1) * sizeof(*t->argv)));
    t->argv[i] = NULL;
}

/* static int symset(char *string)
{
    return (strcmp(string, "&") != 0) &&
           (strcmp(string, "&&") != 0) &&
           (strcmp(string, "|") != 0) &&
           (strcmp(string, "||") != 0) &&
           (strcmp(string, ";") != 0) &&
           (strcmp(string, ">") != 0) &&
           (strcmp(string, ">>") != 0) &&
           (strcmp(string, "<") != 0) &&
           (strcmp(string, "(") != 0) &&
           (strcmp(string, ")") != 0);
} */

void make_shift(int n)
{
    while (n--)
        putc(' ', stderr);
}

void print_argv(char **p, int shift)
{
    char **q = p;
    if (p != NULL)
    {
        while (*p != NULL)
        {
            make_shift(shift);
            fprintf(stderr, "argv[%d]=%s\n",(int) (p - q), *p);
            p++;
        }
    }
}

void print_tree(tree t, int shift)
{
    char **p;
    if (t == NULL)
        return;
    p = t->argv;
    if (p != NULL)
        print_argv(p, shift);
    else
    {
        make_shift(shift);
        fprintf(stderr, "psubshell\n");
    }
    make_shift(shift);
    if (t->infile == NULL)
        fprintf(stderr, "infile=NULL\n");
    else
        fprintf(stderr, "infile=%s\n", t->infile);
    make_shift(shift);
    if (t->outfile == NULL)
        fprintf(stderr, "outfile=NULL\n");
    else
        fprintf(stderr, "outfile=%s\n", t->outfile);
    make_shift(shift);
    fprintf(stderr, "append=%d\n", t->append);
    make_shift(shift);
    fprintf(stderr, "background=%d\n", t->backgrnd);
    make_shift(shift);
    fprintf(stderr, "type=%s\n", t->type == NXT ? "NXT": t->type ==OR ? "OR": "AND");
    make_shift(shift);
    if(t->psubcmd == NULL)
        fprintf(stderr, "psubcmd=NULL \n");
    else
    {
        fprintf(stderr, "psubcmd---> \n");
        print_tree(t->psubcmd, shift + 5);
    }
    make_shift(shift);
    if(t->pipe == NULL)
        fprintf(stderr, "pipe=NULL \n");
    else
    {
        fprintf(stderr, "pipe---> \n");
        print_tree(t->pipe, shift + 5);
    }
    make_shift(shift);
    if(t->next == NULL)
        fprintf(stderr, "next=NULL \n");
    else
    {
        fprintf(stderr, "next---> \n");
        print_tree(t->next, shift + 5);
    }
}

void clear_tree(tree T)
{
    if (T == NULL)
        return;
    clear_tree(T->next);
    clear_tree(T->pipe);
    clear_tree(T->psubcmd);
    free(T->infile);
    free(T->outfile);
    clear_list(T->argv);
    free(T);
    T = NULL;
}
