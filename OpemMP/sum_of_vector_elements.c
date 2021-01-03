// Felix F Feliu opemMP reduction clouse
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main() {
	// variable declaration
	int a[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int sum, i;
	//initialize sum
	sum = 0;

	//add vector elements using a parallel block implementing reduction clouse
#pragma omp parallel for reduction(+:sum) //reduction clouse

	for (i = 0; i <= 9; i++) {
		sum += a[i];
	}
	//print sum value
	printf("Sum of vector a is %d.\n", sum);
	return 0;
}

