#include <stdio.h>
short a=1;
int b=2, num;
long c=3;
float d=4.0;
double e=5.0;
long double f=6.0;
int main()
{
  num = sizeof a;
  printf("short %d\n", num);
  num = sizeof b;
  printf("int %d\n", num);
  num = sizeof c;
  printf("long %d\n", num);
  num = sizeof d;
  printf("float %d\n", num);
  num = sizeof e;
  printf("double %d\n", num);
  num = sizeof f;
  printf("long double %d\n", num);
}