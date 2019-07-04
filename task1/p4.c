#include <stdio.h>
char s1[10] = {'0','1','2','3','4','5','6','7','8','9'};
char s2[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
char s3[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char s4[63];
int i, j, tmp, check;

int main()
  {
    printf("The codes of numbers from 0 to 9:\n");
    check = 1;
    for (i = 0; i<=9; ++i)
      {
      printf("Code of symbol %c is %d\n", s1[i], s1[i]);
      if (s1[i] != '0' + i)
        check = 0;
      }
    if (check = 1)
      printf("All codes go one by one\n");
    printf("\n");
    printf("The codes of letters from a to z:\n");
    check = 1;
    for (i = 0; i<=25; ++i)
      {
      printf("Code of symbol %c is %d\n", s2[i], s2[i]);
      if (s1[i] != 'a' + i)
        check = 0;
      }
    if (check = 1)
      printf("All codes go one by one\n");
    printf("\n");
    printf("The codes of letters from A to Z:\n");
    check = 1;
    for (i = 0; i<=25; ++i)
      {
      printf("Code of symbol %c is %d\n", s3[i], s3[i]);
      if (s1[i] != 'A' + i)
        check = 0;
      }
    if (check = 1)
      printf("All codes go one by one\n");
    printf("\n");
    printf("The code of symbol ' ' is %d\n", ' ');
    for (i = 0; i <=25; ++i)
      s4[i] = s2[i];
    for (i = 0; i <=25; ++i)
        s4[i+26] = s3[i];
    for (i = 0; i <=9; ++i)
          s4[i+52] = s1[i];
    s4[62] = ' ';
    for (i = 0; i <= 62; ++i)
      {
        for (j = 0; j <= 61; ++j)
        {
          if (s4[j] > s4[j+1])
          {
            tmp = s4[j];
            s4[j] = s4[j+1];
            s4[j+1] = tmp;
          }
        }
      }
    printf("The order of symbols is:");
    for (i = 0; i <= 62; ++i)
      printf("%c, ", s4[i]);
  }
