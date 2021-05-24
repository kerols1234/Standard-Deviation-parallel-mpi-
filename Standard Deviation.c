#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char * argv[])
{
    int n;
    int localSum = 0;
    int globalSum = 0;
	int temp;
    int globalMean;
    int localSumOfSqDiff = 0;
    int globalSumOfSqDiff = 0;
    int variance;
    int standardDev;
	int i;
    int my_rank;
    int p;

    MPI_Status status;

    MPI_Init( &argc, &argv );

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(my_rank == 0)
    {
        scanf("%d",&n);
    }

    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);

    int numbers[n];

    if(my_rank != 0)
    {
		srand(my_rank);
		
        for(i = 0; i < n; i++)
        {
            numbers[i] = (rand()%1000);
            localSum += numbers[i];
        }

    }

    MPI_Allreduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    globalMean = (globalSum / (n * (p - 1)));

    if(my_rank != 0)
    {
        for(i = 0; i < n; i++)
        {
            temp = (numbers[i] - globalMean);
            localSumOfSqDiff += (temp * temp);
        }
    }

    MPI_Reduce(&localSumOfSqDiff, &globalSumOfSqDiff, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(my_rank == 0)
    {
        variance = globalSumOfSqDiff/(n * (p - 1));
        standardDev = sqrt(variance);
        printf("Standard Deviation is %d\n",standardDev);
    }

    MPI_Finalize();

    return 0;
}
