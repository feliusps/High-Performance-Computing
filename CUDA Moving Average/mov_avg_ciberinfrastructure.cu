//Felic feliu exercise 3 cuda using visual studio
/*C/Cuda program to accept a matrix of order M x N and find the movil average and average of each row and each column of a matrix*/

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <memory>

#define SIZE 1024

//function 1 vectoravg_c column average
__global__ void vectoravg_c(int m, int n, float* data_d, float* avg_d)
{
	const int avg_index = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

	if (avg_index >= n) {
		return;
	}

	float sum = 0;
	for (int i = 0; i < m; ++i)
	{
		sum = sum + data_d[i * n + avg_index];
	}
	avg_d[avg_index] = (sum / m);
}


//function 2 vectormov decrement loop
__global__ void vectormov(int m, int n, int w, float* data_d, float* mov_d)
{
	const int mov_index = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

	if (mov_index >= m * n) {
		return;
	}
	int index = mov_index;
	float sum = data_d[index];
	int count = 1;
	while (index % n != 0 && count < w - 1)
	{
		count++;
		index--;
		sum = sum + data_d[index];
	}
	mov_d[mov_index] = (sum / count);

}

int main()
{

	//Secuential part

	// initialize variables 

	int i, j, m, n;
	float* data = (float*)malloc(1000 * 1000 * sizeof(float));
	float* mov = (float*)malloc(1000 * 1000 * sizeof(float));
	float* avg_c = (float*)malloc(1000 * sizeof(float));
	memset(data, 0, 1000 * 1000 * sizeof(float));
	memset(mov, 0, 1000 * 1000 * sizeof(float));
	memset(avg_c, 0, 1000 * sizeof(float));


	printf("Enter the order of the matrix\n");
	scanf_s("%d %d", &m, &n);




	FILE* myFile;
	fopen_s(&myFile, "ooi_temp7m.txt", "r");
	if (myFile == NULL) {
		printf("cannot open the file\n");
		return 1;
	}
	for (i = 0; i < m; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			int index = i * n + j;
			if (fscanf(myFile, "%f", &data[index]) == EOF) {
				fseek(myFile, 0, SEEK_SET);
			}
		}
	}

	// CUDA part 

	//initialized variables
	float* array_d;
	float* mov_d, * avg_d;

	int size = m * n * sizeof(float);
	int size1 = m * n * sizeof(float);
	int size2 = n * sizeof(float);

	// Allocate memory on device
	cudaMalloc(&array_d, size);
	cudaMalloc(&mov_d, size1);
	cudaMalloc(&avg_d, size2);

	// initilized matrix on device
	cudaMemcpy(array_d, data, size, cudaMemcpyHostToDevice);
	cudaMemset(mov_d, 0, size1);
	cudaMemset(avg_d, 0, size2);

	// call kernel function 1 vectoravg increment loop
	int bloques = (n + 128 - 1) / 128;
	int y_bloques;
	int x_bloques;

	if (bloques > 128) {
		y_bloques = (bloques + 128 - 1) / 128;
		x_bloques = 128;
	}
	else {
		y_bloques = 1;
		x_bloques = 128;

	}
	dim3 tamGrid1(x_bloques, y_bloques); //grid dimensión
	dim3 tamBlock1(128, 1, 1); //glock dimensión

							  // launch the device computation threads!
	vectoravg_c << <tamGrid1, tamBlock1 >> > (m, n, array_d, avg_d);
	cudaDeviceSynchronize();

	// call kernel function 2 vectormov decrement loop
	bloques = (m * n + 128 - 1) / 128;
	if (bloques > 128) {
		y_bloques = (bloques + 128 - 1) / 128;
		x_bloques = 128;
	}
	else {
		y_bloques = 1;
		x_bloques = 128;

	}
	dim3 tamGrid2(x_bloques, y_bloques); //grid dimension
	dim3 tamBlock2(128, 1, 1); //block dimension

	// Launch the device computation threads!
	vectormov << <tamGrid2, tamBlock2 >> > (m, n, 9, array_d, mov_d);

	cudaDeviceSynchronize();

	// bring results;
	cudaMemcpy(avg_c, avg_d, size2, cudaMemcpyDeviceToHost);
	cudaMemcpy(mov, mov_d, size1, cudaMemcpyDeviceToHost);

	// free memory in  device
	cudaFree(avg_d);
	cudaFree(mov_d);
	cudaFree(array_d);



	//	print mov matrix
	FILE* file;
	fopen_s(&file, "mov_avg_cyberinfrastructure.csv", "w"); //if does not  work, try file=fopen("vectors.csv", "w");


	fprintf(file, "MOV[M,N] : \n");
	for (i = 0; i < m; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			int index = i * n + j;
			if (j != n - 1)
				fprintf(file, "%.5f,", mov[index]);
			else
				fprintf(file, "%.5f", mov[index]);
		}

		fprintf(file, "\n");
	}

	//	print mov matrix
	fprintf(file, "AVG[N] : \n");
	for (j = 0; j < n; ++j)
	{
		if (j != n - 1)
			fprintf(file, "%.5f,", avg_c[j]);
		else
			fprintf(file, "%.5f", avg_c[j]);
	}

	fclose(file);



	return 0;
}

