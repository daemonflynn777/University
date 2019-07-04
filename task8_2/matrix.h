#ifndef MATRIX_H
#define MATRIX_H

#include <exception>

class Matrix {
    int row, column;
    double **matr;
    double str_double(char *s);
    static const double delta;

  public:
    Matrix();
    Matrix(const Matrix &copy);
    Matrix(int n, int m);
    Matrix(double d);
    Matrix(double*, int m);
    Matrix(int n, double*);
    Matrix(char*);
    static Matrix identity(int n);
    static Matrix diagonal(double *vals, int n);
    int rows();
    int columns();
    void set(int i, int j, double val);
    Matrix operator[](int i);
    friend Matrix operator*(Matrix &x, double d);
    Matrix& operator*=(double d);
    friend std::ostream &operator<<(std::ostream &out, const Matrix &x);
    Matrix& operator=(const Matrix& x);
    friend Matrix operator+(Matrix &x, Matrix &y);
    Matrix& operator+=(Matrix &x);
    friend Matrix operator-(Matrix &x, Matrix &y);
    Matrix& operator-=(Matrix &x);
    Matrix operator* (const Matrix &x) const;
    Matrix& operator*=(const Matrix &x);
    Matrix operator-() const;
    bool operator==(const Matrix &x) const;
    bool operator!=(const Matrix &x) const;
    Matrix operator| (const Matrix &x) const;
    Matrix operator/ (const Matrix &x) const;
    ~Matrix();
};

class errors : public std::exception {
private:
    char* err_msg;
public:
    errors(const char* s) {
        err_msg = new char[strlen(s)];
        strcpy(err_msg, s);
    }
    /* void print() {
        std::cout << err_msg << std::endl;
    } */

    virtual const char* what() const noexcept
    {
        //std::cout << "\ERROR\n";
        return err_msg;
    };
};
#endif
