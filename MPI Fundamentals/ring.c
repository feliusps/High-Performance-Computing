// ring code question 5 Felix Feliu

#include <mpi.h>
#include <stdio.h>

int main(argc, argv)
int argc;
char** argv;
{
    int size, rank, tag = 1;
    char msg = 'A', msg_recpt;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Process #%d started \n", rank);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Proc #%d sending message to Proc #%d\n", rank, (rank + 1) % size);
        MPI_Send(&msg, 1, MPI_CHAR, (rank + 1) % size, tag, MPI_COMM_WORLD);

        MPI_Recv(&msg_recpt, 1, MPI_CHAR, (rank - 1 + size) % size, tag, MPI_COMM_WORLD, &status);
        printf("Proc #%d received message from Proc #%d\n", rank, (rank - 1 + size) % size);

    }
    else
    {
        MPI_Recv(&msg_recpt, 1, MPI_CHAR, (rank - 1 + size) % size, tag, MPI_COMM_WORLD, &status);
        printf("Proc #%d received message from Proc #%d\n", rank, (rank - 1 + size) % size);

        printf("Proc #%d sending message to Proc #%d\n", rank, (rank + 1) % size);
        MPI_Send(&msg, 1, MPI_CHAR, (rank + 1) % size, tag, MPI_COMM_WORLD);
    }

    printf("Finishing proc %d\n", rank);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}