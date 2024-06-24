#include <stdio.h>
#include <stdlib.h>

#ifndef MATRIZ_H
#define MATRIZ_H

int abrirArquivos(const char* caminho, int** matriz, int tamanhoMatriz);
int escreverMatrizNoArquivo(int *matriz, int tamanhoMatriz, const char *nomeArquivo);
int somarMatrizes(int *matriz1, int *matriz2, int tamanhoMatriz, const char *nomeArquivo);
int reduzindoMatriz(int *matriz, int tamanhoMatriz);

#endif // MATRIZ_H