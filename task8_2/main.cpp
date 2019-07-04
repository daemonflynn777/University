#include <iostream>
#include <cstring>
#include <cstdio>
#include "matrix.h"

using namespace std;

int main()
{
    Matrix mtrx;
    std::string s;

    std::cout << "> Type help to see available comands\n";
    std::cout << "> ";

    while (1 == 1)
    {
          std::getline(std::cin, s);

          if (s == "help")
              std::cout << "\n  print\n" << "  fill\n" << "  rows\n" << "  columns\n" << "  set\n" << "  mul\n" << "  sum\n" << "  quit\n\n";

          if (s == "print")
              std::cout << mtrx << endl;

          if (s == "fill")
          {
            std::cout << "> Enter your matrix:\n>  ";
            std::string str2matr;
            std::getline(std::cin, str2matr);
            char inpt[1000];
            int i = 0;
            for (int p = 0; p < 1000; p++)
                inpt[p] = '\0';
            while ((str2matr[i] != '\0') && (i != 1024))
            {
                  inpt[i] = str2matr[i];
                  i++;
            }
            if (i == 1000)
            {
                std::cout << "Error. Use less symbols." << std::endl;
                continue;
            }
            try
            {
                Matrix m(inpt);
                mtrx = m;
                std::cout << "Completed."<< std::endl;
            }
            catch (errors error)
            {
                std::cerr << error.what();
            }
          }

          if (s == "rows")
              std::cout << mtrx.rows() << "\n" << endl;

          if (s == "columns")
              std::cout << mtrx.columns() << "\n" << endl;

          if (s == "set")
          {
              int i, j;
              double value;

              std::cout << "Enter the number of column\n> ";
              std::cin >> i;
              std::cout << "Enter the number of row\n> ";
              std::cin >> j;
              std::cout << "Enter the value\n> ";
              std::cin >> value;
              try
              {
                  Matrix m;
                  m = mtrx;
                  m.set(j - 1, i - 1, value);
                  mtrx = m;
              }
              catch (errors error)
              {
                  error.what();
              }
              continue;
          }

          if (s == "mul")
          {
              int multiplier;
              std::cout << "Enter a number for multiplication:\n> ";
              std::cin >> multiplier;
              try
              {
                  mtrx *= multiplier;
                  std::cout << "Completed. Use print to view result"<< std::endl;
              }
              catch (errors error)
              {
                  error.what();
              }
          }

          if ( s == "sum")
          {
              std::string inpt[2];
              for (int j = 0; j < 2; j++)
              {
                  std::cout << "Enter matrix " << j + 1 << "\n> ";
                  int i = 0;
                  std::getline(std::cin, inpt[j]);
                  char str[1000];
                  for (int k = 0; k<1000; k++)
                      str[k] = '\0';
                  while ((inpt[j][i] != '\0') && (i != 1024))
                  {
                      str[i] = inpt[j][i];
                      i++;
                  }
              }
              try
              {
                int j = 1;
                int i = 0;
                char str[1000];
                while ((inpt[j][i] != '\0') && (i != 1024))
                {
                    if ((inpt[j][i] != '{') && (inpt[j][i] != ',') && (inpt[j][i] != '}'))
                        str[i] = ((inpt[0][i]) - '0' + inpt[1][i] - '0') + '0';
                    else
                        str[i] = inpt[j][i];
                    i++;
                    std::cout << str[i];
                }
                Matrix m(str);
                mtrx = m;
                std::cout << "Success!\n\n> ";
              }
              catch (errors error)
              {
                std::cerr << error.what();
              }
            continue;
          }

          if (s == "quit")
              break;

          else if ((s != "help") &&
                  (s != "print") &&
                   (s != "fill") &&
                   (s != "rows") &&
                (s != "columns") &&
                    (s != "set") &&
                    (s != "mul") &&
                    (s != "sum") &&
                   (s != "quit"))
              std::cout << "Unknown command! Use help to see available commands\n";

          std::cout << "> ";
        }
}
