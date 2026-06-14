#ifndef __CACHELATENCY__
#define __CACHELATENCY__

// Repetições de quantas vezes colocaremos dados no array (100 milhões)
#define REPETICOES 100000000

/* Acesso com stride fixo de 16 inteiros (64 bytes),
   para reduzir localidade e forçar diferentes padrões de uso de cache
*/
#define STRIDE_ELEMENTOS 16

// Número de vezes para rodar o teste todo
#define NUM_EXECUCOES 1

void executar_teste(size_t tamanho_bytes, int rodada, FILE *arquivo);

#endif