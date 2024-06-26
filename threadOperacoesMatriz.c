#include "threadOperacoesMatriz.h"

void* somarMatrizesThread(void *args) {
    ThreadProcessamentoArgs *processamentoArgs = (ThreadProcessamentoArgs*)args;
    int *matriz1 = processamentoArgs->matriz1;
    int *matriz2 = processamentoArgs->matriz2;
    int *resultado = processamentoArgs->resultado;
    int start = processamentoArgs->start;
    int end = processamentoArgs->end;

    for (int i = start; i < end; i++) {
        resultado[i] = matriz1[i] + matriz2[i];
    }
    return NULL;
}

void* multiplicandoMatrizThread(void *args) {
    ThreadProcessamentoArgs *processamentoArgs = (ThreadProcessamentoArgs*)args;
    int *matriz1 = processamentoArgs->matriz1;
    int *matriz2 = processamentoArgs->matriz2;
    int *resultado = processamentoArgs->resultado;
    int start = processamentoArgs->start;
    int end = processamentoArgs->end;
    int tamanhoMatriz = processamentoArgs->tamanhoMatriz;

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
    ThreadProcessamentoArgs *processamentoArgs = (ThreadProcessamentoArgs*)args;
    int *matriz = processamentoArgs->matriz1;
    int start = processamentoArgs->start;
    int end = processamentoArgs->end;
    int soma = 0;

    for (int i = start; i < end; i++) {
        soma += matriz[i];
    }

    pthread_mutex_lock(&mutex);
    somaReducao += soma;
    pthread_mutex_unlock(&mutex);

    return NULL;
}