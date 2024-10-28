#include <iostream>
#include <mpi.h>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) 
{
    int processRank, numProcesses;
    int pingPongCount = 100000; 
    int message = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    if (numProcesses != 2) {
        if (processRank == 0) {
            cerr << "Este programa requiere exactamente 2 procesos." << endl;
        }
        MPI_Finalize();
        return 1;
    }

    clock_t startClock, endClock;
    startClock = clock();

    double startWtime, endWtime;
    startWtime = MPI_Wtime();

    for (int i = 0; i < pingPongCount; ++i) {
        if (processRank == 0) {
            
            MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else if (processRank == 1) {
           
            MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    endWtime = MPI_Wtime();
    endClock = clock();

    
    double elapsedClockTime = static_cast<double>(endClock - startClock) / CLOCKS_PER_SEC;
    double elapsedWtime = endWtime - startWtime;

   
    if (processRank == 0) {
        cout << "Tiempo usando clock(): " << elapsedClockTime << " segundos." << endl;
        cout << "Tiempo usando MPI_Wtime(): " << elapsedWtime << " segundos." << endl;
    }

    MPI_Finalize();
    return 0;
}
