#include "parallel.cpp"
#include "sequential.cpp"
#include <chrono>

using namespace std::chrono;

int main() {
    Matrix matrixA(N), matrixB(N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            //printf("%d ", i);
            matrixA(i, j) = i + 1;
            matrixB(i, j) = i + 2;
        }

    // Macro para medir tiempo de cualquier llamada
    #define TIMED(label, call, result)                                          \
        auto start_##result = high_resolution_clock::now();                     \
        Matrix result = call;                                                   \
        auto end_##result = high_resolution_clock::now();                       \
        double ms_##result = duration<double, std::milli>(                      \
            end_##result - start_##result).count();

    TIMED("Clásico",           seqClassic(matrixA, matrixB, N),           resClassic)
    TIMED("Tiling Seq",          seqTiling(matrixA, matrixB, N, 64),        resTiling)
    TIMED("Strassen Seq",        seqStrassen(matrixA, matrixB, N),         resStrassen)
    TIMED("Tiling Paralelo",     parTiling(matrixA, matrixB, N, 64),        resParTiling)
    TIMED("Strassen Paralelo",   parStrassen(matrixA, matrixB, N),         resParStrassen)
    TIMED("Hibrido",            parHybrid(matrixA, matrixB, N),            resHybrid)

    //imprimir(matrixA,        "Matriz A");
    //imprimir(matrixB,        "Matriz B");

    //imprimir(resClassic,     "Resultado Clásico");
    printf("  ⏱  Tiempo Clásico:            %.4f ms\n\n", ms_resClassic);

    //imprimir(resTiling,      "Resultado Tiling (b=2)");
    printf("  ⏱  Tiempo Tiling Seq:         %.4f ms\n\n", ms_resTiling);

    //imprimir(resStrassen,    "Resultado Strassen");
    printf("  ⏱  Tiempo Strassen Seq:       %.4f ms\n\n", ms_resStrassen);

    //imprimir(resParTiling,   "Resultado Tiling Paralelo");
    printf("  ⏱  Tiempo Tiling Paralelo:    %.4f ms\n\n", ms_resParTiling);

    //imprimir(resParStrassen, "Resultado Strassen Paralelo");
    printf("  ⏱  Tiempo Strassen Paralelo:  %.4f ms\n\n", ms_resParStrassen);

    printf("  ⏱  Tiempo Hibrido:  %.4f ms\n\n", ms_resHybrid);


    // Tabla resumen
    printf("┌─────────────────────────┬──────────────┐\n");
    printf("│ Algoritmo               │ Tiempo (ms)  │\n");
    printf("├─────────────────────────┼──────────────┤\n");
    printf("│ Clásico                 │ %12.4f │\n", ms_resClassic);
    printf("│ Tiling Seq (b=64)        │ %12.4f │\n", ms_resTiling);
    printf("│ Strassen Seq            │ %12.4f │\n", ms_resStrassen);
    printf("│ Tiling Paralelo (b=64)   │ %12.4f │\n", ms_resParTiling);
    printf("│ Strassen Paralelo       │ %12.4f │\n", ms_resParStrassen);
    printf("│ Hibrido                 │ %12.4f │\n", ms_resHybrid);
    printf("└─────────────────────────┴──────────────┘\n");

    return 0;
}
