#include <stdio.h>
//long a;
int i;

void fib(int a)
{
  int j;
  long res, f1, f2;
  f1 = 0;
  f2 = 1;
  for (j = 1; j <= a - 1; ++j)
    {
      res = f1 + f2;
      f1 = f2;
      f2 = res;
    }
  printf("Nuber %d Fibonnaci number is %ld\n", a, res);
}

long fib_rec(int b)
{
  if (b == 0)
    return 0;
  if (b == 1)
    return 1;
  return fib_rec(b - 1) + fib_rec(b - 2);
}

int main()
  {
    printf("Enter numbers\n");
    while (scanf("%d", &i) != EOF)
      {
        fib(i);
        printf("Recursive nuber %d Fibonnaci number is %ld\n", i, fib_rec(i));
        printf("\n");
      }
  }
