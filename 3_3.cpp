#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) 
{
    int processRank, commSize;
    int localValue, globalSum;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);

    localValue = processRank + 1;  

    int activeProcesses = commSize;
    int step = 1;

    
    while (activeProcesses > 1) 
    {
        int partner;
        if (activeProcesses % 2 != 0) 
        {
            
            if (processRank == activeProcesses - 1) 
            {
               
                MPI_Send(&localValue, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                activeProcesses--;
                continue;
            }
        }

        
        if (processRank % (2 * step) == 0) 
        {
            partner = processRank + step;
            if (partner < commSize) 
            {
                int recvValue;
                MPI_Recv(&recvValue, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                localValue += recvValue;
            }
        } else if ((processRank % (2 * step)) == step) 
        {
            partner = processRank - step;
            MPI_Send(&localValue, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
            break;
        }

        step *= 2;
        activeProcesses /= 2;
    }

    if (processRank == 0 && commSize % 2 != 0) 
    {
        int extraValue;
        MPI_Recv(&extraValue, 1, MPI_INT, commSize - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        localValue += extraValue;
    }

    if (processRank == 0) 
    {
        globalSum = localValue;
        cout << "Suma global :" << globalSum << endl;
    }

    MPI_Finalize();
    return 0;
}
