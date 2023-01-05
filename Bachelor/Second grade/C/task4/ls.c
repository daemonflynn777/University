#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int elem_count = 1;
    DIR *directory;
    struct dirent *fl;
    directory = opendir(".");
    while ((fl = readdir(directory)) != 0)
    {
        if (!strcmp(fl->d_name, ".") || !strcmp(fl->d_name, ".."))
            continue;
        printf("%s   ", fl->d_name);
        elem_count += 1;
        if (elem_count == 8)
        {
            printf("\n");
            elem_count = 1;
        }
    }
    printf("\n");
    closedir(directory);
    return(0);
}
