#include <stdio.h>
#include "cache_size.h"

int main() {
        FILE *arquivo = fopen("dados.csv", "w");
        if (!arquivo) {
                printf("Erro ao abrir o arquivo.\n");
                return 1;
        }

        // Escreve o cabeçalho do arquivo CSV
        fprintf(arquivo, "Tamanho_KB,Rodada,Latencia_ns\n");

        printf("Iniciando bateria de testes estruturados...\n");

        for (int rodada = 1; rodada <= NUM_EXECUCOES; rodada++) {
                printf("Executando rodada %d de %d...\n", rodada, NUM_EXECUCOES);
                // Testa arrays de 4 KB até 16 MB, dobrando o tamanho a cada execução
                for (size_t tam = 4 * 1024; tam <= 16 * 1024 * 1024; tam *= 2) {
                        executar_teste(tam, rodada, arquivo);
                }
        }

        fclose(arquivo);
        printf("Dados salvos.\n");
        return 0;
}