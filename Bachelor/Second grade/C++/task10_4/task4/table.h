#ifndef table_h
#define table_h

#include "id.h"

class Table
{
    Id *id;
    int size;
    int top;
public:
    Table(int max_size);
    ~Table();
    int add_id(const char *buf);
    Id & operator [] (int i);
};
#endif
