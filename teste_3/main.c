#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix_mult.h"

int main() {
        struct timespec start, end;
        double time_conventional, time_conventional_optimized, time_blocked, speedup, speedup_total, *matrix_A, *matrix_B, *matrix_C;
        int i;

        // Alocação das matrizes
        matrix_A = malloc(sizeof(double) * MATRIX_SIZE * MATRIX_SIZE);
        matrix_B = malloc(sizeof(double) * MATRIX_SIZE * MATRIX_SIZE);
        matrix_C = malloc(sizeof(double) * MATRIX_SIZE * MATRIX_SIZE);

        if (!matrix_A || !matrix_B || !matrix_C) {
                printf("Falha na alocacao de memoria.\n");
                return 1;
        }

        printf("Iniciando teste de multiplicacao de matrizes\n");
        printf("Tamanho da Matriz: %d x %d\n", MATRIX_SIZE, MATRIX_SIZE);
        printf("Tamanho do Bloco:  %d\n", BLOCK_SIZE);
        printf("Rodadas:           %d\n\n", NUM_ROUNDS);

        initialize_matrix(matrix_A, MATRIX_SIZE);
        initialize_matrix(matrix_B, MATRIX_SIZE);

        // Teste multiplicação convencional
        printf("Executando Convencional\n");
        timespec_get(&start, TIME_UTC);
        for (i = 0; i < NUM_ROUNDS; i++) 
                multiply_conventional(matrix_C, matrix_A, matrix_B, MATRIX_SIZE);
        
        timespec_get(&end, TIME_UTC);
        time_conventional = calcular_tempo(start, end);
        printf("Concluido em %f segundos.\n", time_conventional);

        // Teste multiplicação convencional otimizada
        printf("Executando Convencional Otimizada\n");
        timespec_get(&start, TIME_UTC);
        for (i = 0; i < NUM_ROUNDS; i++) 
                multiply_conventional_optimized(matrix_C, matrix_A, matrix_B, MATRIX_SIZE);
        
        timespec_get(&end, TIME_UTC);
        time_conventional_optimized = calcular_tempo(start, end);
        printf("Concluido em %f segundos.\n", time_conventional_optimized);

        // Teste multiplicação em blocos
        printf("Executando em Blocos\n");
        timespec_get(&start, TIME_UTC);
        for (i = 0; i < NUM_ROUNDS; i++) 
                multiply_blocked(matrix_C, matrix_A, matrix_B, MATRIX_SIZE, BLOCK_SIZE);
        
        timespec_get(&end, TIME_UTC);
        time_blocked = calcular_tempo(start, end);
        printf("Concluido em %f segundos.\n\n", time_blocked);

        // resultados
        printf("--- Resultados ---\n");
        if (time_blocked < time_conventional_optimized) {
                speedup = time_conventional_optimized / time_blocked;
                printf("A multiplicacao em blocos foi %.2fx mais rapida.\n", speedup);
        } else {
                printf("Nao houve ganho de performance.\n");
        }
        
        speedup_total = time_conventional / time_blocked;
        printf("Comparado a versao inicial ingenua, "
                "a versao em blocos foi %.2fx mais rapida.\n",
                speedup_total);

        free(matrix_A);
        free(matrix_B);
        free(matrix_C);

        return 0;
}
