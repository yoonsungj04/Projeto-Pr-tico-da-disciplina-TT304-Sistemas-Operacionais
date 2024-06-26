#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "operacoesThread.h"

int numThreads;

int somarMatrizes(int *matriz1, int *matriz2, int tamanhoMatriz, const char *nomeArquivo) {
    int *matrizResultado = (int *)malloc(tamanhoMatriz * tamanhoMatriz * sizeof(int));
    if (matrizResultado == NULL) {
        printf("Falha na alocação de memória.\n");
        return 0;
    }

    pthread_t threads[numThreads];
    ThreadArgs args[numThreads];
    int elementosPorThread = (tamanhoMatriz * tamanhoMatriz) / numThreads;
    int restante = (tamanhoMatriz * tamanhoMatriz) % numThreads;

    for (int i = 0; i < numThreads; i++) {
        args[i].matriz1 = matriz1;
        args[i].matriz2 = matriz2;
        args[i].resultado = matrizResultado;
        args[i].start = i * elementosPorThread;
        args[i].end = (i + 1) * elementosPorThread;
        args[i].tamanhoMatriz = tamanhoMatriz;
        if (i == numThreads - 1) {
            args[i].end += restante;
        }
        pthread_create(&threads[i], NULL, somarMatrizesThread, &args[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    int sucessoEscrita = escreverMatrizNoArquivo(matrizResultado, tamanhoMatriz, nomeArquivo);

    free(matrizResultado);

    return sucessoEscrita;
}

int multiplicandoMatriz(int *matriz1, int *matriz2, int tamanhoMatriz, const char *nomeArquivo) {
    int *matrizResultado = (int *)malloc(tamanhoMatriz * tamanhoMatriz * sizeof(int));
    if (matrizResultado == NULL) {
        printf("Falha na alocação de memória para a matriz resultado.\n");
        return 0;
    }

    pthread_t threads[numThreads];
    ThreadArgs args[numThreads];
    int linhasPorThread = tamanhoMatriz / numThreads;
    int restante = tamanhoMatriz % numThreads;

    for (int i = 0; i < numThreads; i++) {
        args[i].matriz1 = matriz1;
        args[i].matriz2 = matriz2;
        args[i].resultado = matrizResultado;
        args[i].start = i * linhasPorThread;
        args[i].end = (i + 1) * linhasPorThread;
        args[i].tamanhoMatriz = tamanhoMatriz;
        if (i == numThreads - 1) {
            args[i].end += restante;
        }
        pthread_create(&threads[i], NULL, multiplicandoMatrizThread, &args[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    int sucessoEscrita = escreverMatrizNoArquivo(matrizResultado, tamanhoMatriz, nomeArquivo);

    free(matrizResultado);

    return sucessoEscrita;
}

int reduzindoMatriz(int *matriz, int tamanhoMatriz) {
    somaReducao = 0;

    pthread_t threads[numThreads];
    ThreadArgs args[numThreads];
    int elementosPorThread = (tamanhoMatriz * tamanhoMatriz) / numThreads;
    int restante = (tamanhoMatriz * tamanhoMatriz) % numThreads;

    for (int i = 0; i < numThreads; i++) {
        args[i].matriz1 = matriz;
        args[i].start = i * elementosPorThread;
        args[i].end = (i + 1) * elementosPorThread;
        args[i].tamanhoMatriz = tamanhoMatriz;
        if (i == numThreads - 1) {
            args[i].end += restante;
        }
        pthread_create(&threads[i], NULL, reduzindoMatrizThread, &args[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    return somaReducao;
}
