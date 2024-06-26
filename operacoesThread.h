
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef OPERACOESTHREAD_H
#define OPERACOESTHREAD_H

typedef struct {
    int *matriz1;
    int *matriz2;
    int *resultado;
    int start;
    int end;
    int tamanhoMatriz;
} ThreadArgs;

extern pthread_mutex_t mutex;
extern int somaReducao;

void* somarMatrizesThread(void *args);

void* multiplicandoMatrizThread(void *args);

void* reduzindoMatrizThread(void *args);

#endif // OPERACOESTHREAD_H