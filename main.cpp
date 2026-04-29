#include "sequential.cpp"
#include <chrono>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

int main() {
    // Tamaños requeridos por la tarea
    vector<int> tamanos = {256, 512, 1024, 2048, 4096};

    cout << "=== EXPERIMENTOS PARTE 1: ALGORITMOS SECUENCIALES (1 EJECUCION) ===" << endl;
    cout << "N\tClasico(ms)\tTiling(ms, b=64)\tStrassen(ms)" << endl;

    for (int n : tamanos) {
        Matrix A(n, 1), B(n, 2);

        // 1. Clasico
        auto s1 = high_resolution_clock::now();
        seqClassic(A, B, n);
        double t_cla = duration<double, milli>(high_resolution_clock::now() - s1).count();

        // 2. Tiling (bloque fijo 64)
        auto s2 = high_resolution_clock::now();
        seqTiling(A, B, n, 64);
        double t_til = duration<double, milli>(high_resolution_clock::now() - s2).count();

        // 3. Strassen
        auto s3 = high_resolution_clock::now();
        seqStrassen(A, B, n);
        double t_str = duration<double, milli>(high_resolution_clock::now() - s3).count();

        cout << n << "\t" << t_cla << "\t" << t_til << "\t" << t_str << endl;
    }

    // --- EXPERIMENTO 5.1.2: EFECTO DE B ---
    cout << "\n=== EXPERIMENTO 5.1.2: EFECTO DEL TAMANO DE BLOQUE (N=1024) ===" << endl;
    cout << "b\tTiempo(ms)" << endl;
    vector<int> bloques = {16, 32, 64, 128, 256};
    Matrix A1024(1024, 1), B1024(1024, 2);

    for (int b : bloques) {
        auto s = high_resolution_clock::now();
        seqTiling(A1024, B1024, 1024, b);
        double t_b = duration<double, milli>(high_resolution_clock::now() - s).count();
        cout << b << "\t" << t_b << endl;
    }

    return 0;
}