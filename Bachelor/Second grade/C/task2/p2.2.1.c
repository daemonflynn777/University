#include <stdio.h>
double x, a, res;

int main()
  {
    res = 0;
    printf("Enter variable\n");
    scanf("%lf", &x);
    printf("Enter indexes\n");
    scanf("%lf", &a);
    res = a;
    while (scanf("%lf", &a) != EOF)
      {
        res = res*x + a;
      }
    printf("The result is %.3lf\n", res);
  }
