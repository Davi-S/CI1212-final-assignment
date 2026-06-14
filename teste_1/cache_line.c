#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cache_line.h"

void testar_linha(int stride_bytes, FILE *arquivo) {
        int stride_elementos;
        int* array;
        double tempo_total, tempo_por_acesso;
        size_t idx, mascara, num_elementos, i;
        struct timespec inicio, fim;

        /*  Stride (salto) : 
                Quantidade de posições avançadas no vetor a cada acesso.
                Se o stride_bytes for 16 bytes, o stride de inteiros 
                será de 4 inteiros por acesso (array[0], array[4], ...) 
        */
        stride_elementos = stride_bytes / sizeof(int);

        // Quantidade de inteiros dentro do array, deve ser potência de 2 para a var mascara funcionar
        num_elementos = TAMANHO_ARRAY / sizeof(int); // 2.097.152 de inteiros  

        array = malloc(num_elementos * sizeof(int));
        if (!array)
                return;

        // Mascara para garantir que o índice rotacione dentro do array. 
        mascara = num_elementos - 1;     

        /* Aquecimento da memoria para garantir que as páginas já foram mapeadas.
           Evita page fault durante o benchmark.
        */
        for (i = 0; i < num_elementos; i++) {
                array[i] = 0;
        }

        clock_gettime(CLOCK_MONOTONIC, &inicio);

        /* Laço que realiza REPETICOES acessos ao array seguindo o salto definido em stride_elementos.
           O incremento força operações de leitura e escrita na memória,
           permitindo medir o impacto da cache para diferentes strides. 
        */
        idx = 0;
        for (i = 0; i < REPETICOES; i++) {
                array[idx]++;
                idx = (idx + stride_elementos) & mascara; // equivalente a idx = (idx + stride) % num_elementos, mas mais eficiente
        }

        clock_gettime(CLOCK_MONOTONIC, &fim);

        tempo_total = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
        tempo_por_acesso = (tempo_total / REPETICOES) * 1e9;

        // Mostra na tela e grava no arquivo
        printf("Salto: %3d bytes # Tempo médio por acesso: %6.2f ns\n", stride_bytes, tempo_por_acesso);
        fprintf(arquivo, "%d,%.2f\n", stride_bytes, tempo_por_acesso);

        free(array);
}
