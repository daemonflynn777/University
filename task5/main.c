#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include "list.h"
#include "tree.h"
#include "exec.h"

extern list lst;
extern int EOF_flag;
extern int oshibka;
extern intlist * intL;
tree t = NULL;
sigjmp_buf begin;

void bash_replica();

void sgnl(int s)
{
    putchar('\n');
    lst = NULL;
    t = NULL;
    siglongjmp(begin, 1);
}

extern void error(int n, char *message, list lst_tmp)
{
    lst_tmp = lst_tmp + 1;
    if (n == 1)
    {
        fprintf(stderr, "\nLexical error! Try again.\n");
        longjmp(begin, 1);
    }
    else if (n == 2)
        fprintf(stderr, "\nSyntax error! %s\n", message);
}

int main(int argc, char * argv[])
{
    argc++;
    argv++;
    while (1 != 2)
    {
        sigsetjmp(begin, 1);
        oshibka = 0;
        clear_list(lst);
        clear_tree(t);
        clear_zombie(intL);
        bash_replica();
        printf("WeLcOmE tO sHeLl :)\n");
        if (EOF_flag != 1)
            printf("DIGINick:$ ");
        build_list();
        if (EOF_flag == 1)
            break;
        t = build_tree(lst);
        if (oshibka == 0)
        {
            //print_tree(t, 1);
            execute_tree(t);
        }
    }
    signal(SIGINT, SIG_DFL);
    clear_zombie(intL);
    return 0;
}

void bash_replica()
{
    printf("%s", "\x1b[32m"); // green (like leaves in spring) color
    char s[100];
    gethostname(s, 100);
    printf("%s@%s", getenv("USER"), s);
    printf("%s", "\x1B[34m");   // gray (like Moscow in November) color
    getcwd(s, 100);
    printf(":%s$ ", s);
    printf("%s", "\x1B[37m");
    putchar('\n');
}
