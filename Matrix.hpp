#ifndef MATRIX
#define MATRIX

#include <iostream>
#include <array>
#include <iomanip>
#include <vector>

const int N = 1024;

#include <vector>
#include <iostream>
#include <cassert>

struct Matrix {
    int n; // Dimensión única para filas y columnas
    std::vector<int> data;

    // Constructor simplificado para matrices cuadradas n x n
    Matrix(int size, int valor = 0)
        : n(size), data(size * size, valor) {}

    // Sobrecarga de paréntesis para lectura y escritura: matriz(i, j)
    inline int& operator()(int i, int j) {
        // Validación opcional: solo funciona en modo Debug
        assert(i >= 0 && i < n && j >= 0 && j < n);
        return data[i * n + j];
    }

    // Sobrecarga para solo lectura
    inline const int& operator()(int i, int j) const {
        assert(i >= 0 && i < n && j >= 0 && j < n);
        return data[i * n + j];
    }
};
inline Matrix sumar(const Matrix& A, const Matrix& B, int size = N) {
    Matrix res(size);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res(i, j) = A(i, j) + B(i, j);
    return res;
}

inline Matrix parSumar(const Matrix& A, const Matrix& B, int size = N) {
    Matrix res(size);
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res(i, j) = A(i, j) + B(i, j);
    return res;
}

inline Matrix restar(const Matrix& A, const Matrix& B, int size = N) {
    Matrix res(size);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res(i, j) = A(i, j) - B(i, j);
    return res;
}

inline Matrix parRestar(const Matrix& A, const Matrix& B, int size = N) {
    Matrix res(size);
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res(i, j) = A(i, j) - B(i, j);
    return res;
}

inline void imprimir(const Matrix& m, const std::string& titulo) {
    std::cout << "=== " << titulo << " ===" << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << std::setw(3) << m(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

#endif
