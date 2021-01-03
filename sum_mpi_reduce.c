//ode for sum_mpi_reduce question 7.c Felix  Feliu


#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 


#define SET_SIZE 10 


int temporary[1000];

int main(int argc, char* argv[])
{
	int rank, size, SUBSET_SIZE, SUBSET_SIZE_RECEIVED;
	int SUM = 0;

	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	printf("Process # %d started \n", rank);
	// master process 
	if (rank == 0) {
		int set[SET_SIZE];

		for (int i = 0; i < SET_SIZE; i++)
			set[i] = rand() % 1000;

		printf("%s", "SET: [");
		for (int i = 0; i < SET_SIZE - 1; i++)
			printf("%d, ", set[i]);
		printf("%d ]\n", set[SET_SIZE - 1]);

		int i;
		int j;
		SUBSET_SIZE = SET_SIZE / size;
		if (size > 1) {
			for (i = 1; i < size - 1; i++) {
				int sum_received = 0;
				j = i * SUBSET_SIZE;
				MPI_Send(&SUBSET_SIZE, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(&set[j], SUBSET_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
				SUM += sum_received;
			}

			j = i * SUBSET_SIZE;
			int remaining_numbers_count = SET_SIZE - j;

			MPI_Send(&remaining_numbers_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&set[j], remaining_numbers_count, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		int SUBSET_SUM = 0;
		for (i = 0; i < SUBSET_SIZE; i++)
		{
			SUBSET_SUM += set[i];
		}
		printf("Proc #%d\tSUBSET_SUM = %d\n", rank, SUBSET_SUM);

		MPI_Reduce(&SUBSET_SUM, &SUM, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		printf("SET_SUM : %d\n", SUM);
	}
	// slave processes 
	else {
		MPI_Recv(&SUBSET_SIZE_RECEIVED, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&temporary, SUBSET_SIZE_RECEIVED, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		int SUBSET_SUM = 0;
		for (int i = 0; i < SUBSET_SIZE_RECEIVED; i++)
		{
			SUBSET_SUM += temporary[i];
		}
		printf("Proc #%d\tSUBSET_SUM = %d\n", rank, SUBSET_SUM);
		MPI_Reduce(&SUBSET_SUM, &SUM, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	}
	printf("Finishing proc %d\n", rank);
	MPI_Finalize();
	return 0;
}
