#include "threadManipularMatriz.h"

void* threadLeitura(void *args) {
    ThreadLeituraArgs *leituraArgs = (ThreadLeituraArgs*)args;
    const char* caminho = leituraArgs->caminho;
    int **matriz = leituraArgs->matriz;
    int tamanhoMatriz = leituraArgs->tamanhoMatriz;

    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        perror("Falha ao abrir arquivo");
        pthread_exit((void*)0);
    }

    *matriz = (int*)malloc(tamanhoMatriz * tamanhoMatriz * sizeof(int));
    if (*matriz == NULL) {
        fclose(arquivo);
        perror("Falha na alocação de memória");
        pthread_exit((void*)0);
    }

    for (int i = 0; i < tamanhoMatriz * tamanhoMatriz; i++) {
        if (fscanf(arquivo, "%d", &(*matriz)[i]) != 1) {
            free(*matriz);
            fclose(arquivo);
            perror("Falha na leitura do arquivo");
            pthread_exit((void*)0);
        }
    }

    fclose(arquivo);
    pthread_exit((void*)1);
}

void* threadEscrita(void *args) {
    ThreadEscritaArgs *escritaArgs = (ThreadEscritaArgs*)args;
    const char *nomeArquivo = escritaArgs->nomeArquivo;
    int *matriz = escritaArgs->matriz;
    int tamanhoMatriz = escritaArgs->tamanhoMatriz;

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        perror("Falha ao abrir arquivo para escrita");
        pthread_exit((void*)0);
    }

    for (int i = 0; i < tamanhoMatriz; i++) {
        for (int j = 0; j < tamanhoMatriz; j++) {
            if (fprintf(arquivo, "%d ", matriz[i * tamanhoMatriz + j]) < 0) {
                fclose(arquivo);
                perror("Falha ao escrever no arquivo");
                pthread_exit((void*)0);
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    pthread_exit((void*)1);
}
