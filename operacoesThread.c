#include "operacoesThread.h"

pthread_mutex_t mutex;
int somaReducao = 0;

void* somarMatrizesThread(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs*)args;
    int *matriz1 = threadArgs->matriz1;
    int *matriz2 = threadArgs->matriz2;
    int *resultado = threadArgs->resultado;
    int start = threadArgs->start;
    int end = threadArgs->end;

    for (int i = start; i < end; i++) {
        resultado[i] = matriz1[i] + matriz2[i];
    }
    return NULL;
}



void* multiplicandoMatrizThread(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs*)args;
    int *matriz1 = threadArgs->matriz1;
    int *matriz2 = threadArgs->matriz2;
    int *resultado = threadArgs->resultado;
    int start = threadArgs->start;
    int end = threadArgs->end;
    int tamanhoMatriz = threadArgs->tamanhoMatriz;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < tamanhoMatriz; j++) {
            resultado[i * tamanhoMatriz + j] = 0;
            for (int k = 0; k < tamanhoMatriz; k++) {
                resultado[i * tamanhoMatriz + j] += matriz1[i * tamanhoMatriz + k] * matriz2[k * tamanhoMatriz + j];
            }
        }
    }
    return NULL;
}



void* reduzindoMatrizThread(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs*)args;
    int *matriz = threadArgs->matriz1;
    int start = threadArgs->start;
    int end = threadArgs->end;
    int soma = 0;

    for (int i = start; i < end; i++) {
        soma += matriz[i];
    }

    pthread_mutex_lock(&mutex);
    somaReducao += soma;
    pthread_mutex_unlock(&mutex);

    return NULL;
}