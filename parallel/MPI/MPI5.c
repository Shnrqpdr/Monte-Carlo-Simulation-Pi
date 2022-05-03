#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define M_PI 3.141592653589793238462643

#define N 1000

void main(int argc, char **argv)
{

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
		printf("\n Acurácia: %.32f\n", ((M_PI - pi) / M_PI) * 100.0);
	}

	MPI_Finalize();
}
