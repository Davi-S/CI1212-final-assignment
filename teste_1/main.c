#include <stdio.h>
#include "cache_line.h"

int main() {
        printf("Teste de Linha de Cache\n");
        printf("Array fixado em %d MB\n", TAMANHO_ARRAY / (1024 * 1024));

        FILE *arquivo = fopen("dados.csv", "w");
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

