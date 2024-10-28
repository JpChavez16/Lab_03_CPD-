#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) 
{
    int processRank, numProcesses;
    long long int numberOfTosses, localNumberOfTosses, numberInCircle = 0;
    long long int globalNumberInCircle = 0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    if (processRank == 0)
    {
        cout << "Ingresa la cantidad total de lanzamientos:";
        cin >> numberOfTosses;
    }

    
    MPI_Bcast(&numberOfTosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

  
    localNumberOfTosses = numberOfTosses / numProcesses;

 
    srand(time(0) + processRank);


    for (long long int toss = 0; toss < localNumberOfTosses; ++toss) 
    {
        double x = (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
        double y = (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
        double distanceSquared = x * x + y * y;

        if (distanceSquared <= 1.0) 
        {
            ++numberInCircle;
        }
    }


    MPI_Reduce(&numberInCircle, &globalNumberInCircle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (processRank == 0) {
        
        double piEstimate = 4.0 * static_cast<double>(globalNumberInCircle) / static_cast<double>(numberOfTosses);
        cout << "Valor estimado de pi:" << piEstimate << endl;
    }

    
    MPI_Finalize();
    return 0;
}
