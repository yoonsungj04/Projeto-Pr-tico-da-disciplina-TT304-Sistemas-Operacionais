#include <stdio.h>
#include <stdlib.h>

#ifndef MANIPULADORMATRIZ_H
#define MANIPULADORMATRIZ_H

int abrirArquivos(const char* caminho, int** matriz, int tamanhoMatriz);

int escreverMatrizNoArquivo(int *matriz, int tamanhoMatriz, const char *nomeArquivo);

#endif 