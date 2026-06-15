#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cache_size.h"

void executar_teste(size_t tamanho_bytes, int rodada, FILE *arquivo) {
        size_t num_elementos, mascara, idx, i;
        struct timespec inicio, fim;
        double tempo_total, tempo_por_acesso;
        int *array;

        /* Quantidade de inteiros do array.
        Deve ser potência de 2 para que a máscara funcione corretamente.
        */
        num_elementos = tamanho_bytes / sizeof(int);

        array = malloc(num_elementos * sizeof(int));
        if (!array)
                return;

        // Mascara para garantir que o índice rotacione dentro do array.
        mascara = num_elementos - 1;

        /* Aquecimento da memória para garantir que as páginas já foram mapeadas.
           Evita page fault durante o benchmark.
        */
        for (i = 0; i < num_elementos; i++) {
                array[i] = 0;
        }

        clock_gettime(CLOCK_MONOTONIC, &inicio);

        /* Realiza REPETICOES acessos ao array utilizando um salto fixo.
           A operação array[idx]++ gera uma leitura seguida de uma escrita,
           permitindo medir o tempo médio de acesso para diferentes tamanhos de conjunto de dados.
        */
        idx = 0;
        for (i = 0; i  < REPETICOES; i++) {
                array[idx]++;
                idx = (idx + STRIDE_ELEMENTOS) & mascara; // equivalente a idx = (idx + stride) % num_elementos, mas mais eficiente
        }

        clock_gettime(CLOCK_MONOTONIC, &fim);

        tempo_total = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
        tempo_por_acesso = (tempo_total / REPETICOES) * 1e9;

        // Exibe os resultados e salva os dados para geração de gráficos
        printf("Tamanho do Array: %7zu KB # Tempo médio por acesso: %6.2f ns\n", tamanho_bytes / 1024, tempo_por_acesso);
        fprintf(arquivo, "%zu,%d,%.2f\n", tamanho_bytes / 1024, rodada, tempo_por_acesso);

        free(array);
}

