#ifndef __CACHELATENCY__
#define __CACHELATENCY__

// Número total de acessos realizados ao array durante o benchmark
#define REPETICOES 100000000

/* Salto fixo de 16 inteiros (64 bytes) entre acessos.
   Como cada acesso cai em uma linha de cache diferente,
   reduz o reaproveitamento de dados já carregados.
*/
#define STRIDE_ELEMENTOS 16

// Número de vezes para rodar o teste todo
#define NUM_EXECUCOES 1

/* Mede o tempo médio de acesso para diferentes tamanhos de array,
   permitindo observar o impacto da hierarquia de memória (L1, L2, L3 e RAM).
*/
void executar_teste(size_t tamanho_bytes, int rodada, FILE *arquivo);

#endif