// p8 mpi code Felix Feliu
//this version do not accept imput from user
//it is harwired in order to get the same result as in the serial version  
// adaptation from the book Parallel programming author Peter Pacheco  pages 98 -99
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h> //mpi runtime



/* Function we're integrating */
double f(double x)
{
	return(4.0 / (1.0 + x * x));
}

//function fragment to calculate a finite integral using trapezoidal rules
double fragment(double a, double b, double num_fragments, double h)
{
	double est, x;
	int i;

	est = (f(a) + f(b)) / 2.0;
	for (i = 1; i <= num_fragments - 1; i++) {
		x = a + i * h;
		est += f(x);
	}
	est = est * h;

	return est;
}//fragment end

//begin main

int main(int argc, char** argv) {

	int my_rank, comm_sz, n = 1000000000.0, local_n;
	double a = 0.0, b = 1.0, h, local_a, local_b;
	double local_int = 0.0, total_int = 0.0;
	int source;

	/* start up MPI */
	MPI_Init(&argc, &argv);

	/* get rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* total number of processes being used */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	h = (b - a) / n;          /* h is the same for all processes */
	local_n = n / comm_sz;  /* So is the number of trapezoids  */


	// interval of each process
	local_a = a + my_rank * local_n * h;
	local_b = local_a + local_n * h;
	local_int = fragment(local_a, local_b, local_n, h);


	/* Add up the integrals calculated by each process */
	if (my_rank != 0) {
		MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0,
			MPI_COMM_WORLD);
	}
	else {
		total_int = local_int;
		for (source = 1; source < comm_sz; source++) {
			MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0,
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total_int += local_int;
		}
	}


	/* Print the result */
	if (my_rank == 0) {
		printf("With n = %d trapezoids, our estimate\n", n);
		printf("of the integral from %f to %f = %.15e\n",
			a, b, total_int);
	}

	/* Shut down MPI */
	MPI_Finalize();

	return 0;
} /* end  main  */


	