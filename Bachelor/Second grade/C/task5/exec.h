#ifndef EXEC_H
#define EXEC_H

#include <unistd.h>
#include "tree.h"

typedef struct backgroundlist //zombie list
{
    int pid;
    struct backgroundlist *next;
} intlist;

int execute_tree(tree);
void clear_zombie(intlist *);
#endif
