#include "parallel.cpp"
#include "sequential.cpp"

int main() {
    // Inicialización de matrices constantes según tu imagen original
    Matrix matrixA, matrixB;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrixA[i][j] = i + 1;
            matrixB[i][j] = i + 2;
        }
    }

    // Ejecución y comparación
    Matrix resClassic = seqClassic(matrixA, matrixB, N);
    Matrix resTiling  = seqTiling(matrixA, matrixB, N, 2);
    Matrix resStrassen = seqStrassen(matrixA, matrixB, N);

    Matrix resParTiling = parTiling(matrixA, matrixB, N, 2);

    imprimir(matrixA, "Matriz A");
    imprimir(matrixB, "Matriz B");
    imprimir(resClassic, "Resultado Clásico");
    imprimir(resTiling, "Resultado Tiling (b=2)");
    imprimir(resStrassen, "Resultado Strassen");

    imprimir(resParTiling, "Resultado Tiling Paralelo");

    return 0;
}
