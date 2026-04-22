#ifndef SEQUENTIAL
#define SEQUENTIAL
#include "Matrix.hpp"

Matrix seqClassic(const Matrix& A, const Matrix& B, int size) {
    Matrix C(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
    return C;
}

Matrix seqTiling(const Matrix& A, const Matrix& B, int size, int b) {
    Matrix C(size);
    for (int i = 0; i < size / b; i++) {
        for (int j = 0; j < size / b; j++) {
            for (int k = 0; k < b; k++) {
                for (int l = 0; l < b; l++) {
                    for (int t = 0; t < size; t++) {
                        C((i * b) + k, (j * b) + l) += A((i * b) + k, t) * B(t, (j * b) + l);
                    }
                }
            }
        }
    }
    return C;
}

Matrix seqStrassen(const Matrix& A, const Matrix& B, int size) {
    if (size <= 2) {
        return seqClassic(A, B, size);
    }

    int m = size / 2;
    Matrix a11(m);
    Matrix a12(m);
    Matrix a21(m);
    Matrix a22(m);
    Matrix b11(m);
    Matrix b12(m);
    Matrix b21(m);
    Matrix b22(m);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            a11(i, j) = A(i, j);
            a12(i, j) = A(i, j + m);
            a21(i, j) = A(i + m, j);
            a22(i, j) = A(i + m, j + m);

            b11(i, j) = B(i, j);
            b12(i, j) = B(i, j + m);
            b21(i, j) = B(i + m, j);
            b22(i, j) = B(i + m, j + m);
        }
    }

    // Las 7 multiplicaciones de Strassen
    Matrix p1 = seqStrassen(sumar(a11, a22, m), sumar(b11, b22, m), m);
    Matrix p2 = seqStrassen(sumar(a21, a22, m), b11, m);
    Matrix p3 = seqStrassen(a11, restar(b12, b22, m), m);
    Matrix p4 = seqStrassen(a22, restar(b21, b11, m), m);
    Matrix p5 = seqStrassen(sumar(a11, a12, m), b22, m);
    Matrix p6 = seqStrassen(restar(a21, a11, m), sumar(b11, b12, m), m);
    Matrix p7 = seqStrassen(restar(a12, a22, m), sumar(b21, b22, m), m);

    // Combinar resultados
    Matrix c11 = sumar(restar(sumar(p1, p4, m), p5, m), p7, m);
    Matrix c12 = sumar(p3, p5, m);
    Matrix c21 = sumar(p2, p4, m);
    Matrix c22 = sumar(restar(sumar(p1, p3, m), p2, m), p6, m);

    // Reconstruir matriz final
    Matrix C(size);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            C(i, j) = c11(i, j);
            C(i, j + m) = c12(i, j);
            C(i + m, j) = c21(i, j);
            C(i + m, j + m) = c22(i, j);
        }
    }
    return C;
}
#endif
