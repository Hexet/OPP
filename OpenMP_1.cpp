#include "Jacobi.h"
#include <iterator>

int main()
{
    const int size  = 3;
    QSMatrix<double> A(size, size, 0);
    std::vector<double> F(size);
    std::vector<double> X(size);

//    for (int i = 0; i < size; i++)
//        F[i] = i;

    A(0, 0) = 10;
    A(0, 1) = 1;
    A(0, 2) = -1;

    A(1, 0) = 1;
    A(1, 1) = 10;
    A(1, 2) = -1;

    A(2, 0) = -1;
    A(2, 1) = 1;
    A(2, 2) = 10;

    F[0] = 11;
    F[1] = 10;
    F[2] = 10;

    Jacobi jcb (100, 1e-3);

    X = jcb.StartSolver(A, F);

    std::copy(X.begin(), X.end(), std::ostream_iterator<double>(std::cout, " "));

    return EXIT_SUCCESS;
}