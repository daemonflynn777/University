#ifndef tree_h
#define tree_h

#include "list.h"

enum link_type
{
    NXT, AND, OR
};

struct command_structure
{
    char ** argv;
    char *infile;
    char *outfile;
    int append;
    int backgrnd;
    struct command_structure *psubcmd;
    struct command_structure *pipe;
    struct command_structure *next;
    enum link_type type;
};

typedef struct command_structure *tree;
typedef struct command_structure node;

tree build_tree(list);
void print_tree(tree, int);
void clear_tree(tree);

#endif
