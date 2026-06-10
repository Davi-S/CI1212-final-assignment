#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 100 milhões de acessos para tornar o tempo medível
#define REPETICOES 100000000
// Salto de 64 bytes (tamanho padrão de uma linha de cache)
// Como cada int possui 4 bytes, saltamos de 16 em 16 elementos
#define STRIDE_ELEMENTOS 16
// Número de vezes para rodar o teste todo
#define NUM_EXECUCOES 1

void executar_teste(size_t tamanho_bytes, int rodada, FILE *arquivo) {
    size_t num_elementos = tamanho_bytes / sizeof(int);
    int *array = (int *)calloc(num_elementos, sizeof(int));
    if (!array) {
        printf("Erro ao alocar memória para %zu KB\n", tamanho_bytes / 1024);
        return;
    }

    // Máscara binária para garantir que o índice rotacione dentro do array.
    // É a mesma coisa que uma operação de módulo, mas bem mais rápida. funciona
    // quando o número de elementos é potência de 2
    size_t mascara = num_elementos - 1;

    // Aquecimento da cache
    for (size_t i = 0; i < num_elementos; i++) {
        array[i] = 0;
    }

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Esse laço serve pra aumentar o valor do que está no idx. Ele vai
    // atualizando o idx da seguinte forma: array[0] -> array[16]. Dessa forma
    // pq a linha do cache è 64 bytes.
    // Coloca um inteiro no array[0], e vai pro array[16]. Isso porque no
    // array[1], array[2], etc (até o 15) vai ficar os vizinhos que a CPU pegou
    // da cache. Isso força a sempre ter um cache novo.
    size_t idx = 0;
    for (size_t r = 0; r < REPETICOES; r++) {
        // Realiza uma operação de leitura e escrita
        array[idx]++;
        idx = (idx + STRIDE_ELEMENTOS) & mascara;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo_total =
        (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    // Convertido para nanosegundos
    double tempo_por_acesso = (tempo_total / REPETICOES) * 1e9;

    // Mostra na tela e grava no arquivo
    printf("Tamanho do Array: %7zu KB # Tempo médio por acesso: %6.2f ns\n",
           tamanho_bytes / 1024, tempo_por_acesso);
    fprintf(arquivo, "%zu,%d,%.2f\n", tamanho_bytes / 1024, rodada,
            tempo_por_acesso);

    free(array);
}

int main() {
    FILE *arquivo = fopen("dados_cache.csv", "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Escreve o cabeçalho do arquivo CSV
    fprintf(arquivo, "Tamanho_KB,Rodada,Latencia_ns\n");

    printf("Iniciando bateria de testes estruturados...\n");

    for (int rodada = 1; rodada <= NUM_EXECUCOES; rodada++) {
        printf("Executando rodada %d de %d...\n", rodada, NUM_EXECUCOES);

        for (size_t tam = 4 * 1024; tam <= 16 * 1024 * 1024; tam *= 2) {
            executar_teste(tam, rodada, arquivo);
        }
    }

    fclose(arquivo);
    printf("Dados salvos em 'dados_cache.csv'.\n");
    return 0;
}

// gcc -O0 teste_cache.c -o teste_cache
// O parâmetro -O0 impede otimizações do compilador
