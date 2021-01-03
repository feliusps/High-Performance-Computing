/*mm2 parallel code*/
// same code for different schedule
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <memory.h>
#include <malloc.h>
#include <omp.h>

#define SIZE 1000 // for time analisis use 100, 250, 500, 1000, 2000

int main(int argc, char** argv) {

    float matrixa[SIZE][SIZE], matrixb[SIZE][SIZE], mresult[SIZE][SIZE];
    int i, j, k;

    /* Initialize the Matrix arrays */
    for (i = 0; i < SIZE * SIZE; i++) {
        mresult[0][i] = 0.0;
        matrixa[0][i] = matrixb[0][i] = rand() * (float)1.1;
    }
    // Parallel block start
    // in order to change the schedule the word static must be replaced by  dynamic or guided and chunksize use 1 to 4
#pragma omp parallel for schedule(static,4) shared(SIZE) private(i,j,k)
    /* Matrix-Matrix multiply */
    for (k = 0; k < SIZE; k++)
        for (j = 0; j < SIZE; j++)
            for (i = 0; i < SIZE; i++)
                mresult[i][j] = mresult[i][j] + matrixa[i][k] * matrixb[k][j];

    exit(0);
}
