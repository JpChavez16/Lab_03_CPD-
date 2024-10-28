#include <iostream>
#include <mpi.h>
#include <cmath>

using namespace std;

int main(int argc, char* argv[]) 
{
    int processRank, comm_sz, value;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   
    value = processRank + 1;

    
    int nearestPowerOfTwo = 1;
    while (nearestPowerOfTwo * 2 <= comm_sz) 
    {
        nearestPowerOfTwo *= 2;
    }

    
    if (processRank < nearestPowerOfTwo) 
    {
        int stages = log2(nearestPowerOfTwo);

        for (int i = 0; i < stages; ++i) 
        {
            int partner = processRank ^ (1 << i); 
            int receivedValue = 0;

            
            MPI_Sendrecv(&value, 1, MPI_INT, partner, 0,
                         &receivedValue, 1, MPI_INT, partner, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            
            value += receivedValue;
        }
    }

   
    if (processRank >= nearestPowerOfTwo) 
    {
        MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    
    if (processRank == 0) 
    {
        int extraValue;
        for (int i = nearestPowerOfTwo; i < comm_sz; ++i) 
        {
            MPI_Recv(&extraValue, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            value += extraValue;
        }

       
        cout << "La suma global es: " << value << endl;
    }

    MPI_Finalize();
    return 0;
}
