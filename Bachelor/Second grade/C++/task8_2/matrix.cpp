#include <iostream>
#include <cstring>
#include <cstdlib>
#include "matrix.h"

  const double Matrix::delta = 0.5;

  Matrix::Matrix() { //default
      matr = NULL;
      row = -1;
      column = -1;
  }

  Matrix::Matrix(const Matrix &copy): Matrix(){ //copy constructor
        *this = copy;
        std::cout << "Copy success!\n";
  }

  Matrix::Matrix(int n, int m): row(n), column(m) { //1
      int j;
      if ((n <= 0) or (m <= 0))
        throw errors("\"Constructor\" Incorrect input");
      matr = new double*[n];
          for (j = 0; j < n; j++)
          {
              matr[j] = new double[m];
              for (int i = 0; i < m; i++)
                  matr[j][i] = 0.0;
          }
  }

  Matrix::Matrix(double m): row(1), column(1) { //2
      matr = new double*[1];
      matr[0] = new double[1];
      matr[0][0] = m;
  }

  Matrix::Matrix(double* arr, int m): row(1), column(m) { //3
      if ((arr == NULL) || (arr[0] == '\0'))
          throw errors("\"Constructor\" error. Incorrect input");
      matr = new double*[1];
      matr[0] = new double[m];
      for (int j = 0; j < m; j++)
      {
          matr[0][j] = arr[j];
      }
  }

  Matrix::Matrix(int n, double* arr): row(n), column(1) { //4
      if ((arr == NULL) || (arr[0] == '\0'))
          throw errors("\"Constructor\" error. Incorrect input");
      matr = new double*[n];

      for (int j = 0; j < n; j++)
      {
          matr[j] = new double[1];
          matr[j][0] = arr[j];
      }
  }

  Matrix::Matrix(char* str): row(1), column(1) { //5
      if ((str == NULL) || (str[0] == '\0'))
          throw errors("Error! Empty input!");
      int length = strlen(str);
      int width = 1, height = 0;
      for (int i = 1; i < length; i++)
          if (str[i] == '{')
              height++;
      for (int i = 1; str[i] != '}'; i++)
          if (str[i] == ',')
              width++;
      if (height == 0)
          throw errors("Error! Weird input!");

      row = height;
      column = width;
      matr = new double*[height];
      for (int i = 0; i < height; i++)
          matr[i] = new double[width];
      bool is_digit = false;
      int i = 0, j = 0, tmp_index = 0;
      char tmp[100];
      for (int k = 1; k < length; k++)
      {
          if (not (((str[k] >= '0') && (str[k] <= '9')) ||
                                        (str[k] == '{') ||
                                        (str[k] == '}') ||
                                        (str[k] == ',') ||
                                        (str[k] == '.') ||
                                        (str[k] == 'E') ||
                                        (str[k] == 'e') ||
                                        (str[k] == '+') ||
                                        (str[k] == '-')))
                throw errors("Wrong symbol in the Matrix!");
          if (str[k] == '{')
          {
              if (is_digit)
                  throw errors("Not row number after {!");
              j = 0;
              continue;
          }
          if (((str[k] >= '0') && (str[k] <= '9')) ||
                                 (str[k] == '.') ||
                                 (str[k] == 'E') ||
                                 (str[k] == 'e') ||
                                 (str[k] == '+') ||
                                 (str[k] == '-'))
          {
              if (not is_digit)
                  is_digit = true;
              tmp[tmp_index++] = str[k];
              continue;
          }
          if (str[k] == ',')
          {
              if (not is_digit)
                  throw errors("Not row number after ,!");
              is_digit = false;
              matr[i][j] = str_double(tmp);
              for (int tmp_i = 0; tmp_i < 100; tmp_i++)
                  tmp[tmp_i] = '\0';
              tmp_index = 0;
              j++;
              continue;
          }
          if (str[k] == '}')
          {
              if ((not is_digit) && (k != length - 1))
                  throw errors("Matrix size error (input string)!");
              if (is_digit)
              {
                  matr[i][j] = str_double(tmp);
                  for (int tmp_i = 0; tmp_i < 100; tmp_i++)
                      tmp[tmp_i] = '\0';
                  tmp_index = 0;
                  k++;
                  is_digit = false;
                  i++;
                  if (j != width - 1)
                      throw errors("Matrix size error (columns)!");
              }
          }
      }
  }

  Matrix Matrix::identity(int n) { //6
      if (n <= 0)
          throw errors("\"identity\" error. Incorrect input");
      Matrix m(n, n);
      for (int i = 0; i < n; i++)
          m.set(i, i, 1.0);
      return m;
  }

  Matrix Matrix::diagonal(double* vals, int n) { //7
      if (n <= 0)
          throw errors("\"diagonal\" error. Incorrect input");
      if (vals == NULL)
          throw errors("\"diagonal\" error. Empty input string");
      Matrix m(n, n);
      for (int i = 0; i < n; i++)
      {
          if (vals[i] == '\0')
              throw errors("\"diagonal\" error. Incorrect input");
          m.set(i, i, vals[i]);
      }
      return m;
  }

  int Matrix::rows() { //9
      return row;
  }

  int Matrix::columns() { //10
      return column;
  }

  void Matrix::set(int i, int j, double val) { //11
        if (matr == NULL)
            throw errors("\"set\" error. Empty Matrix");
        if ((i >= row) || (j >= column))
            throw errors("\"set\" error. Incorrect index");
        matr[i][j] = val;
  }

  Matrix Matrix::operator[](int i) { //12 and 13
      if (i < 0)
          throw errors("\"operator []\" error. Incorrect input");
      if ((row > i) && (row != 1) && (i >= 0))
      {
          Matrix m(matr[i], column);
          return m;
      }
      else if ((column > i) && (i >= 0))
      {
          int size = static_cast<int>(row);
          double tmp[size];
          for (int j = 0; j < row; j++)
              tmp[j] = matr[j][i];
          Matrix m(row, tmp);
          return m;
      }
      else
          throw errors("\"operator []\" error. Incorrect index");
  }

  Matrix operator*(Matrix &mtr, double d) { //14
    if (mtr.matr == NULL)
        throw errors("\"operator *\" error. Empty Matrix");
    Matrix res(mtr.row, mtr.column);
    for (int i = 0; i < mtr.row; i++)
        for (int j = 0; j < mtr.column; j++)
            res.set(i, j, mtr.matr[i][j] * d);
    return res;
  }

  Matrix& Matrix::operator*=(double d) { //14
    if (matr == NULL)
        throw errors("\"operator *=\" error. Empty Matrix");
    for (int i = 0; i < row; i++)
      for (int j = 0; j < column; j++)
          matr[i][j] *= d;
    return *this;
  }

  std::ostream& operator<<(std::ostream &out, const Matrix &mtr) { //15
    if (mtr.matr == NULL)
        out << "NULL" << std::endl;
    for (int i = 0; i < mtr.row; i++)
    {
        for (int j = 0; j < mtr.column; j++)
        {
            out << " ";
            out.width(5);
            out << mtr.matr[i][j];
        }
        out << std::endl;
    }
    return out;
  }

  Matrix &Matrix::operator=(const Matrix &mtr) {
    row = mtr.row;
    column = mtr.column;
    /* if (matr != NULL)
    {
        for (int i = 0; i < row; i++)
            delete[] matr[i];
        delete[] matr;
    } */
    if (mtr.matr == NULL)
        matr = NULL;
    else
    {
        matr = new double *[row];
        for (int i = 0; i < row; i++)
        {
            matr[i] = new double[column];
            for (int j = 0; j < column; j++)
                matr[i][j] = mtr.matr[i][j];
        }
    }
    return *this;
  }

  Matrix operator+(Matrix &mtr1, Matrix &mtr2) { //16
    if ((mtr1.matr == NULL) || (mtr2.matr == NULL))
        throw errors("Operator + error!");
    Matrix res(mtr1.row, mtr1.column);
    for (int i = 0; i < mtr1.row; i++)
        for (int j = 0; j < mtr1.column; j++)
            res.set(i, j, mtr1.matr[i][j] + mtr2.matr[i][j]);
    return res;
}

Matrix& Matrix::operator+=(Matrix &mtr) { //17
    if (matr == NULL)
        throw errors("Operator += error!");
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            matr[i][j] += mtr.matr[i][j];
    return *this;
}

Matrix operator-(Matrix &mtr1, Matrix &mtr2) { //18
    if ((mtr1.matr == NULL) || (mtr2.matr == NULL))
        throw errors("Operator - error!");
    Matrix res(mtr1.row, mtr1.column);
    for (int i = 0; i < mtr1.row; i++)
        for (int j = 0; j < mtr1.column; j++)
            res.set(i, j, mtr1.matr[i][j] - mtr2.matr[i][j]);
    return res;
}

Matrix& Matrix::operator-=(Matrix &mtr) { //19
    if (matr == NULL)
        throw errors("Operator -= error!");
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            matr[i][j] -= mtr.matr[i][j];
    return *this;
}

Matrix Matrix::operator-() const { //20
    if (matr == NULL)
        throw errors("Left-side operator - error!");
    Matrix n(row, column);
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            n.set(i, j, -matr[i][j]);
    return n;
}

bool Matrix::operator==(const Matrix &mtr) const { //21
    if ((matr == NULL) || (mtr.matr == NULL))
        throw errors("Operator == error! (empty matrix)");
    if ((row != mtr.row) || (column != mtr.column))
        throw errors("Operator == error! (rows and columns error)");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (mtr.matr[i][j] - matr[i][j] > delta)
                return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &mtr) const { //22
    if ((matr == NULL) || (mtr.matr == NULL))
        throw errors("Operator != error! (empty matrix)");
    if ((row != mtr.row) || (column != mtr.column))
        throw errors("Operator != error! (rows and columns error)");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (mtr.matr[i][j] - matr[i][j] > delta)
                return true;
        }
    }
    return false;
}

Matrix Matrix::operator*(const Matrix &mtr) const { //23
    if (column != mtr.row)
        throw errors("Operator * error!");
    Matrix new_mtr(row, mtr.column);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < mtr.column; ++j)
        {
            for (int k = 0; k < column; ++k)
            {
                //the new Matrix was fill with 0
                new_mtr.matr[i][j] += matr[i][k] * mtr.matr[k][j];
            }
        }
    }
    return new_mtr;
}

Matrix& Matrix::operator*=(const Matrix &mtr) { //24
    *this = *this * mtr;
    return *this;
}

Matrix Matrix::operator|(const Matrix &mtr) const { //25
    if ((mtr.matr == NULL) || (matr == NULL))
        throw errors("Operator | error! (empty matrix)");
    if (mtr.row != row)
        throw errors("Operator | error! (rows error)");
    Matrix new_mtr(row, mtr.column + column);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < mtr.column + column; j++)
        {
            if (j < mtr.column)
                new_mtr.matr[i][j] = matr[i][j];
            else
                new_mtr.matr[i][j] = mtr.matr[i][j - mtr.column];
        }
    }
    return new_mtr;
}

Matrix Matrix::operator/(const Matrix &mtr) const { //26
    if ((mtr.matr == NULL) || (matr == NULL))
        throw errors("Operator / error! (empty matrix)");
    if (mtr.column != column)
        throw errors("Operator / error! (columns error)");
    Matrix new_mtr(row + mtr.row, column);
    for (int i = 0; i < mtr.row + row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (i < row)
                new_mtr.matr[i][j] = matr[i][j];
            else
                new_mtr.matr[i][j] = mtr.matr[i-mtr.row][j];
        }
    }
    return new_mtr;
}

  Matrix::~Matrix() { //destructor
      if (matr != NULL)
      {
          for (int i = 0; i < row; i++)
              delete[] matr[i];
          delete[] matr;
      }
  }

  double Matrix::str_double(char *str)
    { // My function is cool too, but this one is more versatile. Thanks to some guys from the internet :)
      double res = 0.0;
      int i = 0;
      int e = 0;
      while ((*(str + i) != '\0') && (*(str + i) >= '0') && (*(str + i) <= '9')) {
          res = res * 10.0 + *(str + i) - '0';
          i++;
      }
      if (*(str + i) == '.'){
          i++;
          while ((*(str + i) != '\0') && (*(str + i) >= '0') && (*(str + i) <= '9')) {
              res = res * 10.0 + *(str + i) - '0';
              e--;
              i++;
          }
      }
      if ((*(str + i) == 'e') || (*(str + i) == 'E')) {
          int k = 0;
          i++;
          if (*(str + i) == '-') {
              i++;
              while ((*(str + i) != '\0') && (*(str + i) >= '0') && (*(str + i) <= '9')) {
                  k = k * 10.0 + *(str + i) - '0';
                  i++;
              }
              e -= k;
          }
          else {
              if (*(str + i) == '+') i++;
              while ((*(str + i) != '\0') && (*(str + i) >= '0') && (*(str + i) <= '9')) {
                  k = k * 10.0 + *(str + i) - '0';
                  i++;
              }
              e += k;
          }
      }
      while (e > 0) {
          res *= 10.0;
          e--;
      }
      while (e < 0) {
          res *= 0.1;
          e++;
      }
      return res;
  }
