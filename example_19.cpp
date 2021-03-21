#include <omp.h>
#include <iostream>
#include <cmath>
#include <time.h>
#define PI 3.14159265
using namespace std;

clock_t t;

int f(int x) {
//    t = clock();
    double result = 0;
    for (unsigned int i = 0; i < 28'000'000; i++) {
        result += sin(30 * PI / 180) * pow(cos(60 * PI / 180), 2);
        result *= 0.95;
    }
//    std::cout << "time calculating task: " << ((double)clock() - t) / ((double) CLOCKS_PER_SEC) << std::endl;
//    cout << "result = " << result << endl;
    return x;
}

int main() {
    int a[100], b[100];
// Инициализация массива b
    for(int i = 0; i<100; i++)
        b[i] = i;
// Директива OpenMP для распараллеливания цикла
#pragma omp parallel for
    for(int i = 0; i<100; i++)
    {
        a[i] = f(b[i]);
        b[i] = 2*a[i];
    }
    int result = 0;
// Далее значения a[i] и b[i] используются, например, так:
#pragma omp parallel for reduction(+ : result)
    for(int i = 0; i<100; i++)
        result += (a[i] + b[i]);
    cout << "Result = " << result << endl;
//
    return EXIT_SUCCESS;
}