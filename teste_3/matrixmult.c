#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// A maior parte do código foi copiado do repositório
// https://github.com/cubiclesoft/matrix-multiply
// (https://github.com/cubiclesoft/matrix-multiply/blob/master/matrixmult.cpp).
// Esse repositório fez um ótimo trabalho implementando várias multiplicações de
// matrix e fazendo um trabalho comparativo entre elas. Porém esse repositório
// não focou no uso da memória cache. O nosso trabalho se diferencia por focar
// no uso da memória cache.

// Parâmetros para controle do teste
#define MATRIX_SIZE 512 // Tamanho da matriz (N x N)
#define BLOCK_SIZE 32   // Tamanho do bloco para otimização de cache
#define NUM_ROUNDS 5    // Quantidade de vezes que a multiplicação será repetida

// Multiplicação convencional
void multiply_conventional(double *C, double *A, double *B, size_t n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            C[i * n + j] = 0.0;
            for (size_t k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}

// Multiplicação em blocos
void multiply_blocked(double *C, double *A, double *B, size_t n,
                      size_t block_size) {
    double tmpa;

    for (size_t i = 0; i < n; i++) {
        // Zera a linha atual de C
        for (size_t j = 0; j < n; j++) {
            C[i * n + j] = 0.0;
        }

        // Multiplicação em blocos para j e k
        for (size_t k = 0; k < n; k += block_size) {
            size_t k2max = (k + block_size <= n ? k + block_size : n);

            for (size_t j = 0; j < n; j += block_size) {
                size_t j2max = (j + block_size <= n ? j + block_size : n);

                for (size_t k2 = k; k2 < k2max; k2++) {
                    tmpa = A[i * n + k2];

                    for (size_t j2 = j; j2 < j2max; j2++) {
                        C[i * n + j2] += tmpa * B[k2 * n + j2];
                    }
                }
            }
        }
    }
}

// Função auxiliar para inicializar a matriz com valores sequenciais
void initialize_matrix(double *matrix, size_t n) {
    double num = 1.0;
    for (size_t i = 0; i < n * n; i++) {
        matrix[i] = num;
        num += 1.0;
    }
}

int main() {
    size_t n = MATRIX_SIZE;
    struct timespec start, end;
    double time_conventional = 0.0;
    double time_blocked = 0.0;

    printf("Iniciando teste de multiplicacao de matrizes\n");
    printf("Tamanho da Matriz: %zu x %zu\n", n, n);
    printf("Tamanho do Bloco:  %d\n", BLOCK_SIZE);
    printf("Rodadas:           %d\n\n", NUM_ROUNDS);

    // Alocação de memória (arrays contíguos)
    double *A = (double *)malloc(sizeof(double) * n * n);
    double *B = (double *)malloc(sizeof(double) * n * n);
    double *C = (double *)malloc(sizeof(double) * n * n);

    if (A == NULL || B == NULL || C == NULL) {
        printf("Falha na alocacao de memoria.\n");
        return 1;
    }

    initialize_matrix(A, n);
    initialize_matrix(B, n);

    // Teste multiplicação convencional
    printf("Executando Convencional");
    timespec_get(&start, TIME_UTC);
    for (int r = 0; r < NUM_ROUNDS; r++) {
        multiply_conventional(C, A, B, n);
    }
    timespec_get(&end, TIME_UTC);
    time_conventional = (double)(end.tv_sec - start.tv_sec) +
                        (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Concluido em %f segundos.\n", time_conventional);

    // Teste multiplicação em blocos
    printf("Executando em Blocos");
    timespec_get(&start, TIME_UTC);
    for (int r = 0; r < NUM_ROUNDS; r++) {
        multiply_blocked(C, A, B, n, BLOCK_SIZE);
    }
    timespec_get(&end, TIME_UTC);
    time_blocked = (double)(end.tv_sec - start.tv_sec) +
                   (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Concluido em %f segundos.\n\n", time_blocked);

    // resultados
    printf("--- Resultados ---\n");
    if (time_blocked < time_conventional) {
        double speedup = time_conventional / time_blocked;
        printf("A multiplicacao em blocos foi %.2fx mais rapida.\n", speedup);
    } else {
        printf("Nao houve ganho de performance. Tente aumentar MATRIX_SIZE.\n");
    }

    free(A);
    free(B);
    free(C);

    return 0;
}
