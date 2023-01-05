#include <stdio.h>

main(int argc,int **argv)
{
  int i;
  argc--;
  for(i=1; i<=argc; i++)
      printf("%s%c", argv[i], i==argc? '\n': ' ');
}
