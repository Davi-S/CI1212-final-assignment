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
//
// Para ver a versão em blocos ser melhor, é preciso forçar as matrizes a
// transbordarem o cache L3. Se todas as três matrizes couberem no cache L3,
// nunca será necessário pegar dados na ram, então não faz diferença usar a
// multiplicação em blocos.
// 1024 deveriam ser o suficiente, mas acreditamos que o pre-fetcher do
// processador estava fazendo com que a multiplicação convencional otimizada
// fosse ainda tão boa quanto a multiplicação em blocos, então aumentamos o
// tamanho para 2048, de modo que a otimização do pre-fetcher não é suficiente,
// e a otimização pela multiplicação em blocos se mostra superior.
#define MATRIX_SIZE 2048 // Tamanho da matriz (N x N)
// O tamanho do bloco define a quantidade de dados que o processador tentará
// manter no cache L1. O block size deve ser o maior possível de modo que as
// três submatrizes de tamanho B×B caibam simultaneamente no cache L1 data
// (L1d). 32 deveriam ser suficientes, mas apenas tivemos melhores resultados
// com a multiplicação em blocos colocando o tamanho em 16.
#define BLOCK_SIZE 16 // Tamanho do bloco para otimização de cache
#define NUM_ROUNDS 5  // Quantidade de vezes que a multiplicação será repetida

// Multiplicação convencional usando a lógica pura da matemática
// c_{ij} = \sum^n_{k=1} a_{ik} \cdot b_{kj}
void multiply_conventional(double *C, double *A, double *B, size_t n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            // Inicializa a posição atual do resultado com zero
            C[i * n + j] = 0.0;
            // Loop interno realizando o somatório
            for (size_t k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}

// Multiplicação convencional, porém otimizada para cache e processador
void multiply_conventional_optimized(double *C, double *A, double *B,
                                     size_t n) {
    double tmpa;

    // Loop da linha i
    for (size_t i = 0; i < n; i++) {

        // Zera a linha inteira de C de uma só vez
        for (size_t j = 0; j < n; j++) {
            C[i * n + j] = 0.0;
        }

        // Loop k (externo; navegação na coluna de A e linha de B) e j (interno)
        for (size_t k = 0; k < n; k++) {
            tmpa = A[i * n + k];
            for (size_t j = 0; j < n; j++) {
                C[i * n + j] += tmpa * B[k * n + j];
            }
        }
    }
}

// Multiplicação em blocos
// c_{ij} = \sum^n_{k=1} a_{ik} \cdot b_{kj}
// Porém a e b são submatrizes
void multiply_blocked(double *C, double *A, double *B, size_t n,
                      size_t block_size) {
    double tmpa;

    for (size_t i = 0; i < n; i++) {

        // Zera a linha atual de C
        for (size_t j = 0; j < n; j++) {
            C[i * n + j] = 0.0;
        }

        // Multiplicação em blocos para j e k
        // Observe que estes loops não avançam de um em um, mas dão saltos
        // largos. Em vez de focar em um único número, o código está
        // selecionando uma submatriz de tamanho block_size x block_size
        for (size_t k = 0; k < n; k += block_size) {
            size_t k2max = (k + block_size <= n ? k + block_size : n);
            for (size_t j = 0; j < n; j += block_size) {
                size_t j2max = (j + block_size <= n ? j + block_size : n);

                // Aqui dentro, nós estamos resolvendo apenas as multiplicações
                // daquele pequeno bloco selecionado
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
