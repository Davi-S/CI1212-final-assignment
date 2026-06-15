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


void multiply_blocked(double *C, double *A, double *B, size_t n, size_t block_size) {
        double tmpa;
        size_t i, j, j2, j2max, k, k2, k2max;

        for (i = 0; i < n; i++) {

                // Zera a linha atual de C
                for (j = 0; j < n; j++) 
                        C[i * n + j] = 0.0;
                
                // Multiplicação em blocos para j e k
                // Observe que estes loops não avançam de um em um, mas dão saltos
                // largos. Em vez de focar em um único número, o código está
                // selecionando uma submatriz de tamanho block_size x block_size
                for (k = 0; k < n; k += block_size) {
                        k2max = (k + block_size <= n ? k + block_size : n);
                        for (j = 0; j < n; j += block_size) {
                                j2max = (j + block_size <= n ? j + block_size : n);

                                // Aqui dentro, nós estamos resolvendo apenas as multiplicações
                                // daquele pequeno bloco selecionado
                                for (k2 = k; k2 < k2max; k2++) {
                                        tmpa = A[i * n + k2];
                                        for (j2 = j; j2 < j2max; j2++) 
                                                C[i * n + j2] += tmpa * B[k2 * n + j2];
                                }
                        }
                }
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