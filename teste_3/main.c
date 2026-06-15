#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix_mult.h"

int main() {
        size_t n = MATRIX_SIZE;
        struct timespec start, end;
        double time_conventional = 0.0;
        double time_conventional_optimized = 0.0;
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
        printf("Executando Convencional\n");
        timespec_get(&start, TIME_UTC);
        for (int r = 0; r < NUM_ROUNDS; r++) {
                multiply_conventional(C, A, B, n);
        }
        timespec_get(&end, TIME_UTC);
        time_conventional = (double)(end.tv_sec - start.tv_sec) +
                                (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Concluido em %f segundos.\n", time_conventional);

        // Teste multiplicação convencional otimizada
        printf("Executando Convencional Otimizada\n");
        timespec_get(&start, TIME_UTC);
        for (int r = 0; r < NUM_ROUNDS; r++) {
                multiply_conventional_optimized(C, A, B, n);
        }
        timespec_get(&end, TIME_UTC);
        time_conventional_optimized =
                (double)(end.tv_sec - start.tv_sec) +
                (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Concluido em %f segundos.\n", time_conventional_optimized);

        // Teste multiplicação em blocos
        printf("Executando em Blocos\n");
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
        if (time_blocked < time_conventional_optimized) {
                double speedup = time_conventional_optimized / time_blocked;
                printf("A multiplicacao em blocos foi %.2fx mais rapida.\n", speedup);
        } else {
                printf("Nao houve ganho de performance.\n");
        }
        double speedup_total = time_conventional / time_blocked;
        printf("Comparado a versao inicial ingenua, a versao em blocos foi %.2fx "
                "mais rapida.\n",
                speedup_total);

        free(A);
        free(B);
        free(C);

        return 0;
}
