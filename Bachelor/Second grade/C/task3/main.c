#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#define SIZE 16

int main()
{
    err = 0;
    strcount = SIZE;
    null_list();
    while (feof(stdin) == 0)
    {
        strcount = SIZE;
        printf("Enter text: \n");
        lst = build_list();
        if (err == 0 && lst)
        {
            print_list(lst);
            if (feof(stdin))
                printf("Ending program --------> zZzZzZ\n");
            else
                printf("\nHere is your sorted list: \n");
            sort_list();
            print_list(lst);
            clear_list();
            printf("\n");
        }
        err = 0;
    }
    clear_list();
    return 0;
}
