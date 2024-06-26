#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "operacoesThread.h"
#include "manipuladorMatriz.h"

#ifndef OPERACOESMATRIZ_H
#define OPERACOESMATRIZ_H

extern int numThreads;
int somarMatrizes(int *matriz1, int *matriz2, int tamanhoMatriz, const char *nomeArquivo);

int multiplicandoMatriz(int *matriz1, int *matriz2, int tamanhoMatriz, const char *nomeArquivo);

int reduzindoMatriz(int *matriz, int tamanhoMatriz);

#endif 