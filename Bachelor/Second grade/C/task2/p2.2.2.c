#include <stdio.h>
double x, y, a, res, dres;

int main()
  {
    res = 0;
    printf("Enter variable\n");
    scanf("%lf", &x);
    printf("Enter indexes\n");
    scanf("%lf", &a);
    res = a;
    dres = 0;
    while (scanf("%lf", &a) != EOF)
      {
        dres = dres*x + res;
        res = res*x + a;
      }
    printf("The result is %.3lf\n", res);
    if (x != 0)
      printf("The first derivative is %.5lf\n", dres);
    else printf("The first derivative is 0\n");
  }
