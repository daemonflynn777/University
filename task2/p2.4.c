#include <stdio.h>
#define len 50
char strng[len];

double str2double(char str[])
{
  int i = 0;
  double d = 0.1; //для дробной части, а потом для степени мантиссы
  char sgn, sgnnum = '+';
  int deg = 0;
  double res = 0;
  double drob = 0;

  if ((str[0]>='0') && (str[0]<='9'))
    sgnnum = '+';
  if (str[0] == '+')
    {sgnnum = '+';
    i += 1;}
  else if (str[0] == '-')
    {sgnnum = '-';
    i += 1;}
  //узнали знак числа
  while ((str[i] >= '0') && (str[i]<='9'))
    {
      res = res*10 + (str[i] - '0');
      i += 1;
    }
  //нашли целую часть числа
  if (str[i]  == '.')
  {
    i += 1;
    while ((str[i]>='0') && (str[i]<='9'))
    {
      drob = drob + d*(str[i] - '0');
      d *= 0.1;
      i += 1;
    }
  }
  //нашли дробную часть числа
  res = res + drob;
  if (str[i] == 'e' || str[i] == 'E')
  {
    i += 1;
    sgn = str[i];
  }
  //проверили мантиссу и знак степени
  i += 1;
  while ((str[i]>='0') && (str[i]<='9'))
    {
        deg = deg + (str[i] - '0');
        i += 1;
  }
  //посчитали степень десятки (например e+2)
  d = 1;
  if (sgn == '+')
    {
      while (deg != 0)
        {
          d *= 10;
          deg -= 1;
        }
      }
  else
  {
    while (deg != 0)
      {
        d *= 0.1;
        deg -=1;
      }
  }
//возвели десятку в нужную степень
  res = res*d;
//получили окончательный результат
  if (sgnnum == '+')
    return res;
  else return (-1)*res;

}

int main()
{
  while (scanf("%s", strng) != EOF)
    {
      printf("The entered number is %.10g\n", str2double(strng));
    }
}
