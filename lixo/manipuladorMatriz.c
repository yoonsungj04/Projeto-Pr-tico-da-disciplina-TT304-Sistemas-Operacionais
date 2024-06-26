#include <stdio.h>
#include <stdlib.h>

int abrirArquivos(const char* caminho, int** matriz, int tamanhoMatriz) {
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        perror("Falha ao abrir arquivo");
        return 0;
    }

    *matriz = (int*)malloc(tamanhoMatriz * tamanhoMatriz * sizeof(int));
    if (*matriz == NULL) {
        fclose(arquivo);
        perror("Falha na alocação de memória");
        return 0;
    }

    for (int i = 0; i < tamanhoMatriz * tamanhoMatriz; i++) {
        if (fscanf(arquivo, "%d", &(*matriz)[i]) != 1) {
            free(*matriz);
            fclose(arquivo);
            perror("Falha na leitura do arquivo");
            return 0;
        }
    }

    fclose(arquivo);
    return 1;
}
                                                                                            
int escreverMatrizNoArquivo(int *matriz, int tamanhoMatriz, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        perror("Falha ao abrir arquivo para escrita");
        return 0;
    }

    for (int i = 0; i < tamanhoMatriz; i++) {
        for (int j = 0; j < tamanhoMatriz; j++) {
            if (fprintf(arquivo, "%d ", matriz[i * tamanhoMatriz + j]) < 0) {
                fclose(arquivo);
                perror("Falha ao escrever no arquivo");
                return 0;
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    return 1;
}
