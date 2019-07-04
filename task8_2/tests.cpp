#include <iostream>
#include <cstring>
#include "matrix.h"

int main() {
    std::cout << "\ntry Matrix x(5,4) and print it" << std::endl;
    try {
        Matrix m(5,4);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x(0,4) and print it" << std::endl;
    try {
        Matrix m(0,4);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x(-1,4) and print it" << std::endl;
    try {
        Matrix m(-1,4);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x(123) and print it" << std::endl;
    try {
        Matrix m(123);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x({1,2,3,4,5},4) and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        Matrix m(buf, 4);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x({1,2,3,4,5},5) and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        Matrix m(buf, 5);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x({1,2,3,4,5},6) and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        Matrix m(buf, 6);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x(5, {1,2,3,4,5}); and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        Matrix x(5, buf);
        std::cout << x << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x({{1,2,3,4,5}}) and print it" << std::endl;
    try {
        char buf[14] = "{{1,2,3,4,5}}";
        Matrix m(buf);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x({{1},{2},{3},{4},{5}}) and print it" << std::endl;
    try {
        char buf[22] = "{{1},{2},{3},{4},{5}}";
        Matrix m(buf);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x({{1},{2},{3},{4},{5, 6}}) and print it" << std::endl;
    try {
        char buf[25] = "{{1},{2},{3},{4},{5, 6}}";
        Matrix m(buf);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x({{1,2,3},{4,5,6},{7,8,9}}) and print it" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x({{1,2,3},{lalala},{7,8,9}}) and print it" << std::endl;
    try {
        char buf[27] = "{{1,2,3},{lalala},{7,8,9}}";
        Matrix m(buf);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix m = Matrix::identity(5); and print it" << std::endl;
    try {
        Matrix m = Matrix::identity(5);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix m = Matrix::identity(0); and print it" << std::endl;
    try {
        Matrix m = Matrix::identity(0);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix m = Matrix::diagonal({1,2,3,4,5},4); and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        Matrix m = Matrix::diagonal(buf, 4);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix m = Matrix::diagonal({1,2,3,4,5},5); and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        Matrix m = Matrix::diagonal(buf, 5);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix m = Matrix::diagonal({1,2,3,4,5},6); and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        Matrix m = Matrix::diagonal(buf, 6);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix m = Matrix::diagonal(NULL,100); and print it" << std::endl;
    try {
        double* buf = NULL;
        Matrix m = Matrix::diagonal(buf, 100);
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }


    std::cout << "\n\nnow we are using Matrix \"x({{1,2,3},{4,5,6}})\":" << std::endl;
    char buf[18] = "{{1,2,3},{4,5,6}}";
    Matrix x(buf);

    std::cout << "try x.rows(); and print it" << std::endl;
    try {
        int i = x.rows();
        std::cout << i << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry x.columns(); and print it" << std::endl;
    try {
        int i = x.columns();
        std::cout << i << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry x.set(1,2,10); and print it" << std::endl;
    try {
        x.set(1,2,10);
        std::cout << 10 << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry x.set(100,-1,10); and print it" << std::endl;
    try {
        x.set(100,-1,10);
        std::cout << x << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix m = x[0]; and print it" << std::endl;
    try {
        Matrix m = x[0];
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix m = x[1]; and print it" << std::endl;
    try {
        Matrix m = x[1];
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix m = x[2]; and print it" << std::endl;
    try {
        Matrix m = x[2];
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix m = x[3]; and print it" << std::endl;
    try {
        Matrix m = x[3];
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix m = x * 5; and print it" << std::endl;
    try {
        Matrix m = x * 5;
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry Matrix x *= 6; and print it" << std::endl;
    try {
        x *= 6;
        std::cout << x << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry n[0][0] and print it" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        Matrix n = m[0][0];
        std::cout << n << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator + and print it" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        char buf2[26] = "{{1,1,1},{1,1,1},{1,1,1}}";
        Matrix n(buf2);
        Matrix sum = n + m;
        std::cout << sum << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator += and print it" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        char buf2[26] = "{{1,1,1},{1,1,1},{1,1,1}}";
        Matrix sum(buf2);
        sum += m;
        std::cout << sum << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator - and print it" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        char buf2[26] = "{{1,1,1},{1,1,1},{1,1,1}}";
        Matrix n(buf2);
        Matrix sum = m - n;
        std::cout << sum << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator -= and print it" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        char buf2[26] = "{{1,1,1},{1,1,1},{1,1,1}}";
        Matrix sum(buf2);
        m -= sum;
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator -n and print it" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        Matrix n = -m;
        std::cout << n << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator == and print it (answer == true)" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        Matrix n(buf);
        std::cout << std::boolalpha << (m==n) << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator == and print it (answer == false)" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        char buf2[26] = "{{3,4,3},{4,5,6},{7,8,9}}";
        Matrix n(buf2);
        std::cout << std::boolalpha << (m==n) << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator != and print it (answer == false)" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        Matrix n(buf);
        std::cout << std::boolalpha << (m!=n) << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator != and print it (answer == true)" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        char buf2[26] = "{{3,4,3},{4,5,6},{7,8,9}}";
        Matrix n(buf2);
        std::cout << std::boolalpha << (m!=n) << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator m*n and print it" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        char buf2[26] = "{{3,4,3},{4,5,6},{7,8,9}}";
        Matrix n(buf2);
        Matrix pr = m * n;
        std::cout << pr << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator *=n and print it" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        char buf2[26] = "{{3,4,3},{4,5,6},{7,8,9}}";
        Matrix n(buf2);
        m *= n;
        std::cout << m << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator | and print it" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        char buf2[26] = "{{3,4,3},{4,5,6},{7,8,9}}";
        Matrix n(buf2);
        Matrix sum = m | n;
        std::cout << sum << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "\ntry operator / and print it" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        Matrix m(buf);
        char buf2[26] = "{{3,4,3},{4,5,6},{7,8,9}}";
        Matrix n(buf2);
        Matrix sum = m / n;
        std::cout << sum << std::endl;
    }
    catch (errors error) {
        std::cerr << error.what() << std::endl;
    }

    std::cout << "Testing completed." << std::endl;
    return 0;
}
