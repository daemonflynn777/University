#include <stdio.h>
double e, x;
double res1, res2, res3, tmp;

int main()
  {
    printf("Enter prescision\n");
    scanf("%lf", &e);
    printf("Enter numbers\n");
    while (scanf("%lf", &x) != EOF)
        {
          res1 = 1;
          res2 = x;
          if (res2 > res1)
            {
              tmp = res2;
              res2 = res1;
              res1 = tmp;
            }
            res2 = 0.5*(res1 + (x/res1));
            res3 = res2;
          while (res1 - res2 >= e)
            {
              //res1 = res2;
              res3 = 0.5*(res2 + (x/res2));
              res1 = res2;
              res2 = res3;
            }
          printf("Square root of %lf is %.10g\n",x,res2);
        }
  }