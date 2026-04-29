#include "parallel.cpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;
using namespace std::chrono;

int main() {
    int n = 2048; 
    vector<int> hilos = {1, 2, 4, 8, 16};
    int b = 64;

    cout << "=== EXPERIMENTOS PARTE 2: ALGORITMOS PARALELOS (N=" << n << ") ===" << endl;
    
    // --- SECCIÓN 1: ESCALABILIDAD (BLOQUES VS STRASSEN) ---
    cout << "P\tT_Bloques(ms)\tS_Bloques\tT_Strassen(ms)\tS_Strassen" << endl;
    
    double t1_bloques, t1_strassen;

    for (int p : hilos) {
        omp_set_num_threads(p);
        Matrix A(n, 1), B(n, 2);

        // Medir Bloques Paralelo
        auto s1 = high_resolution_clock::now();
        parTiling(A, B, n, b);
        double tp_bloques = duration<double, milli>(high_resolution_clock::now() - s1).count();
        if(p == 1) t1_bloques = tp_bloques;

        // Medir Strassen Paralelo
        auto s2 = high_resolution_clock::now();
        parStrassen(A, B, n);
        double tp_strassen = duration<double, milli>(high_resolution_clock::now() - s2).count();
        if(p == 1) t1_strassen = tp_strassen;

        cout << p << "\t" << tp_bloques << "\t" << t1_bloques/tp_bloques 
             << "\t" << tp_strassen << "\t" << t1_strassen/tp_strassen << endl;
    }

    // --- SECCIÓN 2: VERSIÓN HÍBRIDA ---
    cout << "\n=== PRUEBA VERSIÓN HÍBRIDA (P=8) ===" << endl;
    omp_set_num_threads(8);
    Matrix Ah(n, 1), Bh(n, 2);
    auto sh = high_resolution_clock::now();
    parHybrid(Ah, Bh, n);
    double th = duration<double, milli>(high_resolution_clock::now() - sh).count();
    cout << "Tiempo Híbrido: " << th << " ms" << endl;

    // --- SECCIÓN 3: EFECTO DEL UMBRAL (n0) EN STRASSEN ---
    cout << "\n=== EFECTO DEL UMBRAL n0 (N=1024, P=8) ===" << endl;
    
    return 0;
}

