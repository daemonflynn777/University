#ifndef __LIST__
#define __LIST__
#include <stdio.h>

#define SIZE 16

typedef char ** list;
typedef void *(*vertex)(void);

list lst;
char *buf;
char str[SIZE];
int err;
int c;
int length;
int sizebuf;
int sizelist;
int curbuf;
int curlist;
int strcount;
char ok_ok[76];

void null_list();
void clear_list();
void termlist();
void nullbuf();
void addsym();
int symset();
int error_list();
void addword();
void sort_list();
void print_list(list l);
char read_symbol();
list build_list();

#endif
