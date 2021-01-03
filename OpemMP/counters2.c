//counters2 HPC Felix Feliu modified


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <memory.h>
#include <malloc.h>
#include <papi.h>

#define SIZE 1000

int main(int argc, char** argv) {

    float matrixa[SIZE][SIZE], matrixb[SIZE][SIZE], mresult[SIZE][SIZE];
    int i, j, k;
    int events[2] = { PAPI_TOT_INS, PAPI_TOT_IIS, PAPI_LD_INS, PAPI_FP_OPS, PAPI_FP_INS, PAPI_SR_INS }, ret; // hardware counter used
    long long values[2];

    if (PAPI_num_counters() < 2) {
        fprintf(stderr, "No hardware counters here, or PAPI not supported.\n");
        exit(1);
    }

    if ((ret = PAPI_start_counters(events, 2)) != PAPI_OK) {
        fprintf(stderr, "PAPI failed to start counters: %s\n", PAPI_strerror(ret));
        exit(1);
    }

    /* Initialize the Matrix arrays */
    for (i = 0; i < SIZE * SIZE; i++) {
        mresult[0][i] = 0.0;
        matrixa[0][i] = matrixb[0][i] = rand() * (float)1.1;
    }

    /* Matrix-Matrix multiply */
    for (k = 0; k < SIZE; k++)
        for (j = 0; j < SIZE; j++)
            for (i = 0; i < SIZE; i++)
                mresult[i][j] = mresult[i][j] + matrixa[i][k] * matrixb[k][j];

    if ((ret = PAPI_read_counters(values, 2)) != PAPI_OK) {
        fprintf(stderr, "PAPI failed to read counters: %s\n", PAPI_strerror(ret));
        exit(1);
    }

    //printing for different harware counters
    printf("strore instructions = %lld\n", values[5]);
    printf("floating point instructions = %lld\n", values[4]);
    printf("floating point operations = %lld\n", values[3]);
    printf("load instructions = %lld\n", values[2]);
    printf("instructions issued = %lld\n", values[1]);
    printf("instructions completed = %lld\n", values[0]);

    exit(0);
}
