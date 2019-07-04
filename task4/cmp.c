#include <stdio.h>
#include <string.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
    FILE *f1, *f2;
    char c1, c2;
    size_t str_count = 1, byte_count = 1;

    if (argc == 1)
    {
        fprintf(stderr, "not enough operands...\n");
        exit(1);
    }
    if (argc == 2)
    {
        argv[argc] = malloc(2 * sizeof(char));
        strcpy(argv[argc], "-");
        argc += 1;
    }
    if (argc > 3)
        fprintf(stderr, "only two first operands are used!\n");
    if (!strcmp(argv[1], argv[2]))
        exit(0);
    if (!strcmp(argv[1], "-"))
        f1 = stdin;
    else
        f1 = fopen(argv[1], "r");
    if (f1 == NULL)
    {
        fprintf(stderr, "%s: could not open file!\n", argv[1]);
        exit(1);
    }
    if (!strcmp(argv[2], "-"))
        f2 = stdin;
    else
        f2 = fopen(argv[2], "r");
    if (f2 == NULL)
    {
        fprintf(stderr, "%s: could not open file!\n", argv[2]);
        exit(1);
    }
    c1 = fgetc(f1),
    c2 = fgetc(f2);
    while (!feof(f1) && !feof(f2))
    {
        if (c1 != c2)
        {
            printf("%s %s differ: line %zd, byte %zd\n", argv[1], argv[2], str_count, byte_count);
            exit(0);
        }
        if (c1 == '\n')
        {
            byte_count = 0;
            str_count += 1;
        }
        byte_count += 1;
        c1 = fgetc(f1);
        c2 = fgetc(f2);
    }
    if (feof(f1) && feof(f2))
        exit(0);
    printf("%s %s differ: line %zd, byte %zd\n", argv[1], argv[2], str_count, byte_count);
    exit(0);
}
