//hellompi modified code question 4 Felix f Feliu

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

    for (int i = 0; i < size; i++)
    {
        if (i == rank) continue;
        printf("Proc #%d sending message to Proc #%d\n", rank, i);
        MPI_Send(&msg, 1, MPI_CHAR, i, tag, MPI_COMM_WORLD);
    }

    for (int i = 0; i < size; i++)
    {
        if (i == rank) continue;
        MPI_Recv(&msg_recpt, 1, MPI_CHAR, i, tag, MPI_COMM_WORLD, &status);
        printf("Proc #%d received message from Proc #%d\n", rank, i);
    }

    printf("Finishing proc %d\n", rank);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}