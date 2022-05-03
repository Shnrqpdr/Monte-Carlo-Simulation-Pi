#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define M_PI 3.141592653589793238462643

#define N 1000

void main(int argc, char **argv)
{

	struct timespec begin, endTime;
	double realTime, realTimeNano, realTimeSec;
	clock_gettime(CLOCK_REALTIME, &begin);

	MPI_Init(&argc, &argv);

	int rank, size;
	int total_div = N;
	double pi;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Bcast(&total_div, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// variables of each one

	int i;
	int div, start, end;
	double circle_count = 0, total_sum;
	double x_coordinate, y_coordinate;

	div = total_div / size;
	start = div * rank;
	end = start + div;

	srand(rank);

	for (i = start; i < end; i++)
	{

		x_coordinate = (double)rand() / (RAND_MAX);
		y_coordinate = (double)rand() / (RAND_MAX);

		if (x_coordinate * x_coordinate + y_coordinate * y_coordinate < 1)
			circle_count++;
	}

	MPI_Reduce(&circle_count, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		pi = 4 * total_sum / total_div;
		printf("\n Resultado: %.32f\n", pi);
		printf("\n Erro em porcentagem: %.8f \n", fabs(((M_PI - pi) / M_PI) * 100.0));

		clock_gettime(CLOCK_REALTIME, &endTime);

		realTimeSec = endTime.tv_sec - begin.tv_sec;
		realTimeNano = endTime.tv_nsec - begin.tv_nsec;
		realTime = realTimeSec + realTimeNano * 1e-9;

		printf("\nTempo de execução: %lf\n", realTime);
	}

	MPI_Finalize();
}
