#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "exec.h"
#include "list.h"

#define SIZE 5

int exec_conveyor(tree);
int exec_cd(char *argv[]);
int exec_pwd(tree);
int exec_cmd (tree);
void zombies_all(int);
void kill_process(void);
extern int EOF_flag;
int start_pid;
intlist *intL = NULL;

int execute_tree (tree T)
{
    int res = 0;
    tree TR = T;
    start_pid = getpid();
    while (TR != NULL)
    {
        res = exec_conveyor(TR);
        if ((TR->type == AND) && (res != 0))
        {
            while ((TR->next != NULL) && (TR->type == AND))
                    TR = TR->next;
        }
        else if ((TR->type == OR) && (res == 0))
        {
            while ((TR->next != NULL) && (TR->type == OR))
                TR = TR->next;
        }
        TR = TR->next;
    }
    return res;
}

int exec_cmd(tree T)
{
    int in_file, out_file = 0, res = 0;
    if (T->backgrnd == 1)
        signal(SIGINT, SIG_IGN);
    else
       signal(SIGINT, SIG_DFL);
    if  (T->infile != NULL)
    {
        if ((in_file = open(T->infile, O_RDONLY)) < 0)
        {
            fprintf(stderr, "%s: Input file does not exist!\n", T->infile);
            exit(1);
        }
        dup2(in_file, 0);
        close(in_file);
    }
    if  (T->outfile != NULL)
    {
        if (T->append == 1)
        {
            if ((out_file = open(T->outfile, O_WRONLY | O_CREAT |O_APPEND, 0777)) < 0)
                exit(1);
        }
        else
        {
            if ((out_file = open(T->outfile, O_WRONLY | O_CREAT |O_TRUNC, 0777)) < 0)
                exit(1);
        }
        dup2(out_file, 1);
        close(out_file);
    }
    if (T->backgrnd == 1)
    {
        int f = open("/dev/null", O_RDONLY);
        dup2(f, 0);
        if (fork() == 0)
        {
            execvp(T->argv[0], T->argv);
            exit(0);
        }
        else
            kill(getpid(), SIGKILL);
    }
    if (T->psubcmd != NULL)
    {
        res = execute_tree(T->psubcmd);
        if (res != 0)
            exit(1);
        else
            exit(0);
    }
    else if ((T->argv != NULL) && (strcmp(T->argv[0], "pwd") == 0))
    {
        res = exec_pwd(T);
        exit(res);
    }
    else
    {
        execvp(T->argv[0], T->argv);
        fprintf(stderr, "%s: command  does not found!\n", T->argv[0]);
        exit(1);
    }
}

int exec_conveyor(tree T)
{
    tree TR = T;
    int pid = 0;
    int fd[2], old = -1;
    int status = 0;
    signal(SIGINT, SIG_IGN);
    while (TR != NULL)
    {
        if ((TR->argv != NULL) && (strcmp(TR->argv[0], "cd") == 0))
        {
            if (old != -1)
                close(old);
            if (TR->pipe != NULL)
            {
                if (pipe(fd) < 0)
                    exit(1);
                old = fd[0];
                close(fd[1]);
            }
            status = exec_cd(T->argv);
            return status;
        }
        else if ((TR->argv != NULL) && (strcmp(TR->argv[0], "exit") == 0))
        {
            if (getpid() != start_pid)
            {
                if (T->argv[1] != NULL)
                {
                    fprintf(stderr, "\nexit cannot have arguments! Nevermind, I will be exiting anyway.");
                    exit(1);
                }
                exit(0);
            }
            else
            {
                EOF_flag = 1;
                if (T->argv[1] != NULL)
                {
                    fprintf(stderr, "\nexit cannot have arguments! Nevermind, I will be exiting anyway.");
                    return 1;
                }
                return 0;
            }
        }
        if (TR == T)
        {
            if (TR->pipe != NULL)
            {
                if (pipe(fd) < 0)
                    exit(1);
                if ((pid = fork()) == 0)
                {
                    dup2(fd[1], 1);
                    close(fd[0]);
                    close(fd[1]);
                    exec_cmd(TR);
                }
                old = fd[0];
                close(fd[1]);
            }
            else if ((pid = fork()) == 0)
                exec_cmd(TR);
        }
        else if (TR->pipe == NULL)
        {
            if ((pid = fork()) == 0)
            {
                dup2(old, 0);
                close(old);
                exec_cmd(TR);
            }
            close(old);
        }
        else {
            if (pipe(fd) < 0)
                exit(1);
            if ((pid = fork()) == 0)
            {
                dup2(old, 0);
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);
                close(old);
                exec_cmd(TR);
            }
            close(fd[1]);
            close(old);
            old = fd[0];
        }
        TR = TR->pipe;
    }
    while (wait(&status) != -1);
    return status;
}

int exec_cd(char *argv[])
{
    char *s;
    if (argv[1] == NULL)
    {
        s = getenv("HOME");
        if (s == NULL)
        {
            fprintf(stderr, "No home folder found!\n");
            return 1;
        }
        else
            chdir(s);
    }
    else if (argv[2] != NULL)
    {
        fprintf(stderr, "cd has only one argument!\n");
        return 1;
    }
    else if (chdir(argv[1]))
    {
        perror(argv[1]);
        return 1;
    }
    return 0;
}

int exec_pwd(tree T)
{
    char *directory;
    int attempt = 1;
    if (T->argv[1] != NULL)
    {
        fprintf(stderr, "pwd does NOT have arguments!\n");
        return 1;
    }
    directory = calloc(SIZE, sizeof(char));
    getcwd(directory, attempt++ * SIZE);
    while (directory[0] == '\0')
    {
        directory = realloc(directory, SIZE * attempt * sizeof(char));
        getcwd(directory, attempt++ * SIZE);
    }
    fprintf(stdout, "%s\n", directory);
    free(directory);
    exit(0);
}

void kill_process(void)
{
    int pid;
    pid = getpid();
    if (start_pid != pid)
    	exit(9);
}

void zombies_all(int pid)
{
    intlist * tmp;
    tmp = malloc(sizeof(intlist));
    tmp->pid = pid;
    tmp->next = intL;
    intL = tmp;
}

void clear_zombie(intlist * intL)
{
    int status;
    intlist * tmp;
    while (intL != NULL)
    {
        waitpid(intL->pid, &status, 0);
        tmp = intL;
        intL = intL->next;
        free(tmp);
    }
    return;
}
