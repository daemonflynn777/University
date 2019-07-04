#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char * argv[])
{
    struct stat f_st1, f_st2;
    int f, f_cp, eq = 0;
    char buf[BUFSIZ];
    long count;

    if (argc != 3)
    {
        printf("Wrong command! Ending...\n");
        exit(1);
    }
    lstat(argv[1], &f_st1);
    lstat(argv[2], &f_st2);
    if (f_st1.st_ino == f_st2.st_ino)
        eq = 1;

    if (eq)
    {
        printf("%s and %s are the same files!\n", argv[1], argv[2]);
        exit(-1);
    }

    if ((f_cp = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
    {
        printf("First argument is incorrect. Ending...\n");
        close(f_cp);
        exit(1);
    }

    if S_ISDIR(f_st2.st_mode)
    {
        char *full_path = NULL;
        size_t p_len, full_path_len, f_len = 0, f_name_index;

        p_len = strlen(argv[2]);
        full_path_len = strlen(argv[1]);
        for (size_t i = full_path_len; i != 0; i--)
        {
            if (argv[1][i-1] == '/')
            {
                f_len = full_path_len - i;
                break;
            }
        if (f_len == 0)
            f_len = full_path_len;
        }
        full_path = calloc(f_len + p_len + 1, sizeof(char));
        f_name_index = full_path_len - f_len;
        for (int i = 0; i < p_len; i ++)
            full_path[i] = argv[2][i];
        full_path[p_len] = '/';
        for (int i = 0; i < f_len; i ++)
            full_path[i + p_len + 1] = argv[1][i + f_name_index];
        if ((f_cp = open(full_path, f_st1.st_mode)) == -1)
        {
            printf("Second argument is incorrect. Ending...\n");
            close(f_cp);
            free(full_path);
            exit(1);
        }
        free(full_path);
    }

    if ((f = open(argv[2], O_RDONLY, 0)) == -1)
    {
        printf("Second argument is incorrect. Ending...\n");
        close(f);
        close(f_cp);
        exit(1);
    }

    while ((count = read(f, buf, BUFSIZ)) > 0)
        write(f_cp, buf, count);
    close(f);
    close(f_cp);
    if (!eq)
        printf("Finished successfully.\n");
    return 0;
}
