#include "mpi.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]) {
    double starttime;
    int size, rank, tag, rc, N, generations, outPoints, s;
    MPI_Status Stat;
    MPI_Request request1 = MPI_REQUEST_NULL;
    MPI_Request request2 = MPI_REQUEST_NULL;
    MPI_Request request3 = MPI_REQUEST_NULL;
    MPI_Request request4 = MPI_REQUEST_NULL;
    ofstream output("output.txt");
    rc = MPI_Init(&argc, &argv);
    if (rc != 0) {
        cout << "Error starting MPI." << endl;
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        starttime = MPI_Wtime();
        ifstream file("input.txt");

        if (!file) {
            cout << "Error opening file" << endl;
            MPI_Finalize();
            return -1;
        }

        file >> N >> generations >> outPoints;
        s = N / size;    // какого размера срезы
        char theBoard[N][N];
        for (int i = 0; i < N; i++) {
            string temp;
            file >> temp;
            for (int j = 0; j < N; j++) theBoard[i][j] = (int) temp[j] - 48;
        }
        file.close();

        // отправка данных всем процессам
        int info[4];
        info[0] = N;
        info[1] = s;
        info[2] = generations;
        info[3] = outPoints;
        for (int dest = 0; dest < size; dest++) MPI_Send(&info, 4, MPI_INT, dest, 1, MPI_COMM_WORLD);
        char slice[N / size][N];
        for (int z = 0; z < size; z++) {
            for (int k = 0; k < s; k++)
                for (int l = 0; l < N; l++)
                    slice[k][l] = theBoard[k + (z * s)][l];
            cout << 7 << endl;
            MPI_Send(&slice, N * s, MPI_CHAR, z, 1, MPI_COMM_WORLD);
            cout << 8 << endl;
        }

    }

    // получение данных отправленных процессом 0
    int localinfo[4];
    cout << 1 << endl;
    MPI_Recv(&localinfo, 4, MPI_INT, 0, 1, MPI_COMM_WORLD, &Stat);
    cout << 2 << endl;
    N = localinfo[0];
    s = localinfo[1];
    generations = localinfo[2];
    outPoints = localinfo[3];
    char myslice[s][N];
    //char*  myslice = new double[s*N];
    //myslice[i*N+j]
    cout << 3 << endl;
    MPI_Recv(&myslice, N * s, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &Stat);
    cout << 4 << endl;
    char todown[N];
    char toup[N];
    char fromdown[N];
    char fromup[N]; // массивы для получения и отправки
    for (int g = 1; g <= generations; g++) {
        // обмен крайними значениями
        if (rank != size - 1) // все кроме последнего отправляем вниз
        {
            for (int j = 0; j < N; j++) todown[j] = myslice[s - 1][j];
            MPI_Isend(&todown, N, MPI_CHAR, rank + 1, 1, MPI_COMM_WORLD, &request1);

        } else {
            for (int k = 0; k < N; k++) fromdown[k] = 0;
        }

        if (rank != 0) // все кроме первого получаем сверху
        {
            MPI_Irecv(&fromup, N, MPI_CHAR, rank - 1, 1, MPI_COMM_WORLD, &request2);

        } else { for (int k = 0; k < N; k++) fromup[k] = 0; }

        if (rank != 0) // все кроме первого отправляем вверх
        {
            for (int j = 0; j < N; j++) toup[j] = myslice[0][j];
            MPI_Isend(&toup, N, MPI_CHAR, rank - 1, 1, MPI_COMM_WORLD, &request3);
        }

        if (rank != size - 1) // все кроме последнего получаем снизу
        {
            MPI_Irecv(&fromdown, N, MPI_CHAR, rank + 1, 1, MPI_COMM_WORLD, &request4);
        }

        // подсчет соседей
        int sum = 0;
        char mynewslice[s][N];
        for (int x = 1; x < s-1; x++) {
            for (int y = 1; y < N-1; y++) {
                //общий случай, для элементов внутри массива, не на крайних линиях
                sum = myslice[x - 1][y - 1] + myslice[x - 1][y] + myslice[x - 1][y + 1] + myslice[x][y + 1] +
                      myslice[x + 1][y + 1] + myslice[x + 1][y] + myslice[x + 1][y - 1] + myslice[x][y - 1];

                if (myslice[x][y] == 1 && (sum == 2 || sum == 3)) mynewslice[x][y] = 1;
                else if (myslice[x][y] == 1 && sum > 3) mynewslice[x][y] = 0;
                else if (myslice[x][y] == 1 && sum < 1) mynewslice[x][y] = 0;
                else if (myslice[x][y] == 0 && sum == 3) mynewslice[x][y] = 1;
                else mynewslice[x][y] = 0;
            }
        }

        if (rank != size - 1)
        MPI_Wait(&request1, &Stat);
        if (rank != 0)
        MPI_Wait(&request2, &Stat);
        if (rank != 0)
        MPI_Wait(&request3, &Stat);
        if (rank != size - 1)
        MPI_Wait(&request4, &Stat);

        for (int x = 0; x < s; x++) {
            for (int y = 0; y < N; y++) {
                if (x == 0 && y == 0) //верхне-левый угол
                    sum = myslice[x + 1][y] + myslice[x + 1][y + 1] + myslice[0][y + 1] + fromup[0] + fromup[1];
                else if (x == 0 && y == N - 1) //верхне-правый угол
                    sum = myslice[x][y - 1] + myslice[x + 1][y - 1] + myslice[x + 1][y] + fromup[N - 1] + fromup[N - 2];
                else if (x == s - 1 && y == 0) //нижне-левый угол
                    sum = myslice[x][y + 1] + myslice[x - 1][y + 1] + myslice[x - 1][y] + fromdown[0] + fromdown[1];
                else if (x == s - 1 && y == N - 1) //нижне-правый угол
                    sum = myslice[x - 1][y] + myslice[x - 1][y - 1] + myslice[x][y - 1] + fromdown[N - 1] +
                          fromdown[N - 2];
                else // крайние линии массива, без углов
                {
                    if (y == 0)
                        sum = myslice[x - 1][y] + myslice[x - 1][y + 1] + myslice[x][y + 1] + myslice[x + 1][y + 1] +
                              myslice[x + 1][y];
                    else if (y == N - 1)
                        sum = myslice[x - 1][y] + myslice[x - 1][y - 1] + myslice[x][y - 1] + myslice[x + 1][y - 1] +
                              myslice[x + 1][y];
                    else if (x == 0)
                        sum = myslice[x][y - 1] + myslice[x + 1][y - 1] + myslice[x + 1][y] + myslice[x + 1][y + 1] +
                              myslice[x][y + 1] + fromup[y - 1] + fromup[y] + fromup[y + 1];
                    else if (x == s - 1)
                        sum = myslice[x - 1][y - 1] + myslice[x - 1][y] + myslice[x - 1][y + 1] + myslice[x][y + 1] +
                              myslice[x][y - 1] + fromdown[y - 1] + fromdown[y] + fromdown[y + 1];
                    else continue;
                }

                if (myslice[x][y] == 1 && (sum == 2 || sum == 3)) mynewslice[x][y] = 1;
                else if (myslice[x][y] == 1 && sum > 3) mynewslice[x][y] = 0;
                else if (myslice[x][y] == 1 && sum < 1) mynewslice[x][y] = 0;
                else if (myslice[x][y] == 0 && sum == 3) mynewslice[x][y] = 1;
                else mynewslice[x][y] = 0;

            }
        }

        for (int x = 0; x < s; x++)
            for (int y = 0; y < N; y++)
                myslice[x][y] = mynewslice[x][y];
        if (g % outPoints == 0) {
            if (rank == 0) {
                char aBoard[s][N];
                output << "Generation " << g << ":" << endl;
                for (int x = 0; x < s; x++) {
                    for (int y = 0; y < N; y++) output << (int) myslice[x][y];
                    output << endl;
                }
                for (int i = 1; i < size; i++) {
                    MPI_Recv(&aBoard, N * s, MPI_CHAR, i, 1, MPI_COMM_WORLD, &Stat);
                    for (int x = 0; x < s; x++) {
                        for (int y = 0; y < N; y++) output << (int) aBoard[x][y];
                        output << endl;
                    }
                }
                output << endl << endl;
            } else MPI_Send(&myslice, N * s, MPI_CHAR, 0, 1, MPI_COMM_WORLD);


        }
    }
    if (rank == 0)
        cout << "That took " << MPI_Wtime() - starttime << " seconds" << endl;
    output.close();
    MPI_Finalize();
}


