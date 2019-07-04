#include "table.h"

Table::Table(int max_size) {
    id = new Id[size = max_size];
    top = 1;
}

Table::~Table() { //Destructor
    delete[] id;
}

int Table::add_id(const char *buf)
{
    for (int j = 1; j < top; j++)
        if (std::string(buf) == id[j].get_name())
            return j;
    id[top].put_name(buf);
    ++top;
    return top-1;
}

Id& Table::operator[] (int i) //return ID
{
    return id[i];
}
