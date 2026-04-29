#include <iostream>
#include <string>
#include <cstdlib>
#include <random>
#include <omp.h>
#include "parallel.cpp"
#include "sequential.cpp"
#include "Matrix.hpp"

using namespace std;

void fillMatrix(Matrix& matrix){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(1.0,10.0);

    for (int i = 0; i < matrix.data.size(); i++)
        matrix.data[i] = dis(gen);
}

int main(int argc, char* argv[])
{
    if (argc < 3){
        cerr << "Argumentos insuficientes";
        return 1;
    }
    string algo = argv[1]; int n = atoi(argv[2]); int b = (argc >=4) ? atoi(argv[3]) : 64;
    vector<int> threads = {1,2,4,8};
    Matrix A(n)  ; Matrix B(n);
    fillMatrix(A); fillMatrix(B);

    if (algo == "classic"){seqClassic(A,B,n);}
    else if (algo == "seq_tiling"){seqTiling(A,B,n,b);}
    else if (algo == "seq_strassen"){seqStrassen(A,B,n);}
    else if (algo == "par_tiling"){parTiling(A,B,n,b);}
    else if (algo == "par_strassen"){parStrassen(A,B,n);}
    else if (algo == "par_hybrid"){parHybrid(A,B,n);}
    else {
        cerr << "Unknown algorithm: " << algo << "\n";
        return 1;
    }
    return 0;
}
