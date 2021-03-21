#ifndef THREAD_WEB_SERVER_JACOBI_H
#define THREAD_WEB_SERVER_JACOBI_H

#include <omp.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "matrix.h"

class Jacobi
{
private:
    double Eps;
    int Iter;
    int MaxIter;
public:
    Jacobi(int maxIter, double eps);
    std::vector<double> StartSolver(QSMatrix<double> A,  std::vector<double> F);
};

std::vector<double> Jacobi::StartSolver(QSMatrix<double> A, std::vector<double> F)
{
    std::vector<double> res(F.size());
    std::vector<double> resNew(F.size());

    for (int i = 0; i < F.size(); i++)
        res[i] = F[i] / A(i, i);

    double normError;

    do
    {
        if (Iter > 1)
            std::copy(resNew.begin(), resNew.end(), res.begin());

        normError = 0.0;
        for (int i = 0; i < F.size(); i++)
        {
            resNew[i] = F[i];
            for (int j = 0; j < F.size(); j++)
                if (i != j)
                    resNew[i] -= A(i, j) * res[j];


            resNew[i] /= A(i, i);
            normError += pow(res[i] - resNew[i], 2);
        }

        normError = sqrt(normError);
        Iter++;

    } while (Iter < MaxIter && normError > Eps);
    return res;
}

inline Jacobi::Jacobi(int maxIter, double eps)
{
    Eps = eps;
    MaxIter = maxIter;
    Iter = 0;
}

#endif //THREAD_WEB_SERVER_JACOBI_H
