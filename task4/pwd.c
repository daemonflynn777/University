#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 32

int main()
{
    char *path;
    int s = 1;
    path = (char*)malloc(SIZE);
    getcwd(path, s++ * SIZE);
    while (path[0] == '\0')
    {
        path = realloc(path, SIZE * s * sizeof(char));
        getcwd(path, s++ * SIZE);
    }
    printf("%s", path);
    free(path);
    putchar('\n');
    return 0;
}
