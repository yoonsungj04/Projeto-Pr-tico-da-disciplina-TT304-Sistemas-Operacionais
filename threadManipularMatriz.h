#ifndef THREADMANIPULARMATRIZ_H
#define THREADMANIPULARMATRIZ_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Estrutura para argumentos da thread de leitura
typedef struct {
    const char* caminho; // Caminho do arquivo a ser lido
    int **matriz;        // Ponteiro para a matriz de inteiros
    int tamanhoMatriz;   // Tamanho da matriz
} ThreadLeituraArgs;

// Estrutura para argumentos da thread de escrita
typedef struct {
    const char* nomeArquivo; // Nome do arquivo para escrita
    int *matriz;             // Matriz de inteiros
    int tamanhoMatriz;       // Tamanho da matriz
} ThreadEscritaArgs;

// Protótipos das funções das threads
void* threadLeitura(void *args);
void* threadEscrita(void *args);

#endif // THREADMANIPULARMATRIZ_H