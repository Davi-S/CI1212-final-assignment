#ifndef __MATRIXMULT__
#define __MATRIXMULT__

/*
   Matrizes maiores tendem a exceder a capacidade das caches do processador,
   tornando mais visível o impacto dos cache misses e os benefícios da
   multiplicação em blocos. O tamanho utilizado foi definido
   experimentalmente a partir dos resultados obtidos durante os testes.
*/
#define MATRIX_SIZE 2048 

/* Tamanho dos blocos utilizados na multiplicação em blocos.
   A técnica de blocking divide a matriz em submatrizes menores para aumentar
   o reaproveitamento dos dados já carregados na cache. O valor foi escolhido
   experimentalmente a partir dos melhores resultados observados.
*/
#define BLOCK_SIZE 16

// Quantidade de execuções utilizadas para calcular os tempos médios.
#define NUM_ROUNDS 5 

// Multiplicação convencional de matrizes
void multiply_conventional(double *C, double *A, double *B, size_t n);

/* Multiplicação convencional otimizada: Reorganiza a ordem dos acessos 
   à memória para melhorar a localidade espacial e reduzir a quantidade 
   de acessos custosos à cache.
*/
void multiply_conventional_optimized(double *C, double *A, double *B, size_t n);

/* Multiplicação em blocos (blocking): Divide as matrizes em submatrizes 
   menores, permitindo que os dados sejam reutilizados por mais tempo 
   na cache antes de serem substituídos.
*/
void multiply_blocked(double *C, double *A, double *B, size_t n, size_t block_size);

// Função auxiliar para inicializar a matriz com valores sequenciais
void initialize_matrix(double *matrix, size_t n);

// Calcula o tempo decorrido entre dois instantes em segundos.
double calcular_tempo(struct timespec inicio, struct timespec fim); 


#endif