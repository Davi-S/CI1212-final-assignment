#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Array fixo em X MB para garantir que os dados caiam na L3 ou RAM,
// assim s falhas de cache ficam bem visíveis no tempo final.
#define TAMANHO_ARRAY (8 * 1024 * 1024)
#define REPETICOES 100000000

void testar_linha(int stride_bytes, FILE *arquivo) {
    int stride_elementos = stride_bytes / sizeof(int);
    size_t num_elementos = TAMANHO_ARRAY / sizeof(int);

    int *array = (int *)calloc(num_elementos, sizeof(int));
    if (!array)
        return;

    size_t mascara = num_elementos - 1;

    // Aquecimento da memória
    for (size_t i = 0; i < num_elementos; i++) {
        array[i] = 0;
    }

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    size_t idx = 0;
    for (size_t r = 0; r < REPETICOES; r++) {
        array[idx]++;
        idx = (idx + stride_elementos) & mascara;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo_total =
        (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    double tempo_por_acesso = (tempo_total / REPETICOES) * 1e9;

    // Mostra e salva no arquivo
    printf("Salto: %3d bytes # Tempo médio por acesso: %6.2f ns\n",
           stride_bytes, tempo_por_acesso);
    fprintf(arquivo, "%d,%.2f\n", stride_bytes, tempo_por_acesso);

    free(array);
}

int main() {
    printf("Teste de Linha de Cache\n");
    printf("Array fixado em %d MB\n", TAMANHO_ARRAY / (1024 * 1024));

    FILE *arquivo = fopen("dados_linha_cache.csv", "w");
    if (!arquivo)
        return 1;
    fprintf(arquivo, "Salto_Bytes,Latencia_ns\n");

    // Varia o salto de 4 bytes (1 int) até 512 bytes
    for (int stride = 4; stride <= 512; stride *= 2) {
        testar_linha(stride, arquivo);
    }

    fclose(arquivo);

    return 0;
}

// gcc -O0 teste_linha_cache.c -o teste_linha
