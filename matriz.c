#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"

int abrirArquivos(const char* caminho, int** matriz, int tamanhoMatriz) {
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        return 0;
    }

    *matriz = (int*)malloc(tamanhoMatriz * tamanhoMatriz * sizeof(int));
    if (*matriz == NULL) {
        fclose(arquivo);
        return 0;
    }

    for (int i = 0; i < tamanhoMatriz * tamanhoMatriz; i++) {
        if (fscanf(arquivo, "%d", &(*matriz)[i]) != 1) {
            free(*matriz);
            fclose(arquivo);
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
                return 0;
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    return 1;
}

int somarMatrizes(int *matriz1, int *matriz2, int tamanhoMatriz, const char *nomeArquivo) {
    int *matrizResultado = (int *)malloc(tamanhoMatriz * tamanhoMatriz * sizeof(int));
    if (matrizResultado == NULL) {
        printf("Falha na alocação de memória.\n");
        return 0;
    }

    for (int i = 0; i < tamanhoMatriz; i++) {
        for (int j = 0; j < tamanhoMatriz; j++) {
            matrizResultado[i * tamanhoMatriz + j] = matriz1[i * tamanhoMatriz + j] + matriz2[i * tamanhoMatriz + j];
        }
    }

    int sucessoEscrita = escreverMatrizNoArquivo(matrizResultado, tamanhoMatriz, nomeArquivo);

    free(matrizResultado);

    return sucessoEscrita;
}

int reduzindoMatriz(int *matriz, int tamanhoMatriz) {
    int soma = 0;
    for (int i = 0; i < tamanhoMatriz * tamanhoMatriz; i++) {
        soma += matriz[i];
    }
    return soma;
}