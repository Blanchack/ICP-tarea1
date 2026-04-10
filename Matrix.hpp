#ifndef MATRIX
#define MATRIX

#include <iostream>
#include <array>
#include <iomanip>

const int N = 4;
using Matrix = std::array<std::array<int, N>, N>;

inline Matrix sumar(const Matrix& A, const Matrix& B, int size = N) {
    Matrix res = {0};
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res[i][j] = A[i][j] + B[i][j];
    return res;
}

inline Matrix restar(const Matrix& A, const Matrix& B, int size = N) {
    Matrix res = {0};
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res[i][j] = A[i][j] - B[i][j];
    return res;
}

inline void imprimir(const Matrix& m, const std::string& titulo) {
    std::cout << "=== " << titulo << " ===" << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << std::setw(3) << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

#endif
