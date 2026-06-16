#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix_mult.h"

void multiply_conventional(double *C, double *A, double *B, size_t n) {
        size_t i, j, k;

        for (i = 0; i < n; i++) 
                for (j = 0; j < n; j++) {
                        // Inicializa a posição atual do resultado com zero
                        C[i * n + j] = 0.0;

                        // Loop interno realizando o somatório
                        for (k = 0; k < n; k++) {
                                C[i * n + j] += A[i * n + k] * B[k * n + j];
                        }
                }
}

void multiply_conventional_optimized(double *C, double *A, double *B, size_t n) {
        double tmpa;
        size_t i, j, k;

        // Loop da linha i
        for (i = 0; i < n; i++) {

                // Zera a linha inteira de C de uma só vez
                for (j = 0; j < n; j++) 
                        C[i * n + j] = 0.0;
        
                // Loop k (externo; navegação na coluna de A e linha de B) e j (interno)
                for (k = 0; k < n; k++) {
                        tmpa = A[i * n + k];
                        for (j = 0; j < n; j++) 
                                C[i * n + j] += tmpa * B[k * n + j];
                }
        }
}

void multiply_blocked(double *C, double *A, double *B, size_t n, size_t bs) {
    size_t i, j, k;
    size_t ii, jj, kk;

    // Zera C
    for (i = 0; i < n * n; i++)
        C[i] = 0.0;

    for (ii = 0; ii < n; ii += bs)
        for (kk = 0; kk < n; kk += bs)
            for (jj = 0; jj < n; jj += bs)
                for (i = ii; i < ii + bs && i < n; i++) 
                    for (k = kk; k < kk + bs && k < n; k++) { 
                        double tmpa = A[i * n + k];
                        for (j = jj; j < jj + bs && j < n; j++)
                            C[i * n + j] += tmpa * B[k * n + j];
                    }
                
}

void initialize_matrix(double *matrix, size_t n) {
        double num = 1.0;
        size_t i;

        for (i = 0; i < n * n; i++) {
                matrix[i] = num;
                num += 1.0;
        }
}

double calcular_tempo(struct timespec inicio, struct timespec fim) {
    return (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
}