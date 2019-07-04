#ifndef __LIST__
#define __LIST__
#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

typedef char ** list;
extern int oshibka;

void null_list();
void clear_list();
void termlist();
void nullbuf();
void addsym(char);
int error_list();
void addword();
void sort_list();
void print_list(list l);
void build_list();
void clear_list(list l);
#endif
