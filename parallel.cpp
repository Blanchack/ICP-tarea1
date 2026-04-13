#include "Matrix.hpp"
#include <omp.h>

Matrix parTiling(const Matrix& A, const Matrix& B, int size, int b) {
    Matrix C = {0};

    //each tile in parallel
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size / b; i++) {
        for (int j = 0; j < size / b; j++) {
            //normal sequential algorithm for each tile
            for (int k = 0; k < b; k++) {
                for (int l = 0; l < b; l++) {
                    for (int t = 0; t < size; t++) {
                        C[(i * b) + k][(j * b) + l] += A[(i * b) + k][t] * B[t][(j * b) + l];
                    }
                }
            }
        }
    }
    return C;
}

Matrix parStrassen(const Matrix& A, const Matrix& B, int size) {
    if (size <= 2) {
        return seqClassic(A, B, size);
    }

    int m = size / 2;
    Matrix a11 = {0}, a12 = {0}, a21 = {0}, a22 = {0};
    Matrix b11 = {0}, b12 = {0}, b21 = {0}, b22 = {0};

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {

            a11[i][j] = A[i][j];
            a12[i][j] = A[i][j + m];
            a21[i][j] = A[i + m][j];
            a22[i][j] = A[i + m][j + m];

            b11[i][j] = B[i][j];
            b12[i][j] = B[i][j + m];
            b21[i][j] = B[i + m][j];
            b22[i][j] = B[i + m][j + m];
        }
    }

    // Las 7 multiplicaciones de Strassen
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp taskgroup
            {
                #pragma omp task
                Matrix p1 = seqStrassen(parSumar(a11, a22, m), parSumar(b11, b22, m), m);
                #pragma omp task
                Matrix p2 = seqStrassen(parSumar(a21, a22, m), b11, m);
                #pragma omp task
                Matrix p3 = seqStrassen(a11, parRestar(b12, b22, m), m);
                #pragma omp task
                Matrix p4 = seqStrassen(a22, parRestar(b21, b11, m), m);
                #pragma omp task
                Matrix p5 = seqStrassen(parSumar(a11, a12, m), b22, m);
                #pragma omp task
                Matrix p6 = seqStrassen(parRestar(a21, a11, m), parSumar(b11, b12, m), m);
                #pragma omp task
                Matrix p7 = seqStrassen(parRestar(a12, a22, m), parSumar(b21, b22, m), m);
            }
        }
    }
    // Combinar resultados

    Matrix c11 = parSumar(parRestar(parSumar(p1, p4, m), p5, m), p7, m);
    Matrix c12 = parSumar(p3, p5, m);
    Matrix c21 = parSumar(p2, p4, m);
    Matrix c22 = parSumar(parRestar(parSumar(p1, p3, m), p2, m), p6, m);

    // Reconstruir matriz final
    Matrix C = {0};
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            C[i][j] = c11[i][j];
            C[i][j + m] = c12[i][j];
            C[i + m][j] = c21[i][j];
            C[i + m][j + m] = c22[i][j];
        }
    }
    return C;
}
