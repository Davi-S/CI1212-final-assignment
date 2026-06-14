#ifndef __CACHELINE__
#define __CACHELINE__

/* Array de 8 MB utilizado para que os acessos não fiquem restritos
   às caches menores do processador, tornando as diferenças de
   desempenho mais visíveis durante o teste */
#define TAMANHO_ARRAY (8 * 1024 * 1024)

// Repetições de quantas vezes colocaremos dados no array (100 milhões)
#define REPETICOES 100000000

void testar_linha(int stride_bytes, FILE *arquivo);

#endif