#include <iostream>
#include <vector>
#include <mpi.h>
#include <cmath>

using namespace std;


int FindBin(double value, const vector<double>& binMaxes, double minMeas) 
{
    int bin = 0;
    while (bin < binMaxes.size() && value >= binMaxes[bin]) 
    {
        ++bin;
    }
    return bin;
}

int main(int argc, char* argv[]) 
{
    
    int processRank, numProcesses;
    int dataCount = 20;
    double minMeas = 0.0, maxMeas = 5.0;
    int binCount = 5;
    vector<double> data = {1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3,
                                4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9};
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    double binWidth = (maxMeas - minMeas) / binCount;
    vector<double> binMaxes(binCount);
    vector<int> binCounts(binCount, 0);
    
    for (int b = 0; b < binCount; ++b) 
    {
        binMaxes[b] = minMeas + binWidth * (b + 1);
    }

    int localDataCount = dataCount / numProcesses;
    int remaining = dataCount % numProcesses;
    if (processRank < remaining) 
    {
        ++localDataCount;
    }

    vector<double> localData(localDataCount);
    vector<int> sendCounts(numProcesses);
    vector<int> displacements(numProcesses);

    int offset = 0;
    for (int i = 0; i < numProcesses; ++i) 
    {
        sendCounts[i] = dataCount / numProcesses + (i < remaining ? 1 : 0);
        displacements[i] = offset;
        offset += sendCounts[i];
    }

    MPI_Scatterv(data.data(), sendCounts.data(), displacements.data(), MPI_DOUBLE,
                 localData.data(), localDataCount, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    vector<int> localBinCounts(binCount, 0);
    for (double value : localData) 
    {
        int bin = FindBin(value, binMaxes, minMeas);
        ++localBinCounts[bin];
    }

    MPI_Reduce(localBinCounts.data(), binCounts.data(), binCount, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (processRank == 0) 
    {
        cout << "Histograma:\n";
        for (int b = 0; b < binCount; ++b) 
        {
            cout << "Bin " << b << " [" << (b == 0 ? minMeas : binMaxes[b - 1]) << ", " << binMaxes[b] << "): " << binCounts[b] << endl;
        }
    }
    MPI_Finalize();
}