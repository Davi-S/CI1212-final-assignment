#ifndef __CACHELINE__
#define __CACHELINE__

/* Array de 8 MB utilizado para que os acessos não fiquem restritos
   às caches menores do processador, tornando as diferenças de
   desempenho mais visíveis durante o teste */
#define TAMANHO_ARRAY (8 * 1024 * 1024)

// Número total de acessos realizados ao array durante o benchmark
#define REPETICOES 100000000

/* Executa um benchmark variando o salto entre acessos consecutivos
   ao array, permitindo analisar o efeito da localidade espacial e
   do comportamento da cache no tempo de acesso.
*/
void testar_linha(int stride_bytes, FILE *arquivo);

#endif