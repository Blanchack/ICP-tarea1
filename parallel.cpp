#ifndef PARALLEL
#define PARALLEL

#include "Matrix.hpp"
#include "sequential.cpp"
#include <omp.h>

Matrix parTiling(const Matrix& A, const Matrix& B, int size, int b) {
    Matrix C(size);

    //each tile in parallel
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size / b; i++) {
        for (int j = 0; j < size / b; j++) {
            //normal sequential algorithm for each tile
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

Matrix parStrassen(const Matrix& A, const Matrix& B, int size) {
    if (size <= 2) {
        return seqClassic(A, B, size);
    }

    int m = size / 2;
    Matrix a11(m), a12(m), a21(m), a22(m);
    Matrix b11(m), b12(m), b21(m), b22(m);

    #pragma omp parallel for collapse(2)
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
    Matrix p1(m), p2(m), p3(m), p4(m), p5(m), p6(m), p7(m);
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp taskgroup
            {
                #pragma omp task shared(p1)
                p1 = parStrassen(parSumar(a11, a22, m), parSumar(b11, b22, m), m);
                #pragma omp task shared(p2)
                p2 = parStrassen(parSumar(a21, a22, m), b11, m);
                #pragma omp task shared(p3)
                p3 = parStrassen(a11, parRestar(b12, b22, m), m);
                #pragma omp task shared(p4)
                p4 = parStrassen(a22, parRestar(b21, b11, m), m);
                #pragma omp task shared(p5)
                p5 = parStrassen(parSumar(a11, a12, m), b22, m);
                #pragma omp task shared(p6)
                p6 = parStrassen(parRestar(a21, a11, m), parSumar(b11, b12, m), m);
                #pragma omp task shared(p7)
                p7 = parStrassen(parRestar(a12, a22, m), parSumar(b21, b22, m), m);
            }
        }
    }

    // Combinar resultados
    Matrix c11(m), c12(m), c21(m), c22(m);
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp taskgroup
            {
                #pragma omp task shared(c11)
                c11 = sumar(restar(sumar(p1, p4, m), p5, m), p7, m);
                #pragma omp task shared(c12)
                c12 = sumar(p3, p5, m);
                #pragma omp task shared(c21)
                c21 = sumar(p2, p4, m);
                #pragma omp task shared(c22)
                c22 = sumar(restar(sumar(p1, p3, m), p2, m), p6, m);
            }
        }
    }

    // Reconstruir matriz final
    Matrix C(size);
    #pragma omp parallel for collapse(2)
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

Matrix parHybrid(const Matrix& A, const Matrix& B, int size) {
    if (size <= 128) {
        return parTiling(A, B, size, 16);
    }

    int m = size / 2;
    Matrix a11(m), a12(m), a21(m), a22(m);
    Matrix b11(m), b12(m), b21(m), b22(m);

    // División de submatrices en paralelo
    #pragma omp parallel for collapse(2)
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

    Matrix p1(m), p2(m), p3(m), p4(m), p5(m), p6(m), p7(m);

    // Ejecución de las 7 tareas de Strassen
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            #pragma omp task shared(p1)
            p1 = parHybrid(sumar(a11, a22, m), sumar(b11, b22, m), m);
            #pragma omp task shared(p2)
            p2 = parHybrid(sumar(a21, a22, m), b11, m);
            #pragma omp task shared(p3)
            p3 = parHybrid(a11, restar(b12, b22, m), m);
            #pragma omp task shared(p4)
            p4 = parHybrid(a22, restar(b21, b11, m), m);
            #pragma omp task shared(p5)
            p5 = parHybrid(sumar(a11, a12, m), b22, m);
            #pragma omp task shared(p6)
            p6 = parHybrid(restar(a21, a11, m), sumar(b11, b12, m), m);
            #pragma omp task shared(p7)
            p7 = parHybrid(restar(a12, a22, m), sumar(b21, b22, m), m);

            #pragma omp taskwait
        }
    }

    // Combinación de resultados (C11, C12, C21, C22)
    Matrix c11 = sumar(restar(sumar(p1, p4, m), p5, m), p7, m);
    Matrix c12 = sumar(p3, p5, m);
    Matrix c21 = sumar(p2, p4, m);
    Matrix c22 = sumar(restar(sumar(p1, p3, m), p2, m), p6, m);

    Matrix C(size);
    // Reconstrucción final
    #pragma omp parallel for collapse(2) if(size > 256)
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
