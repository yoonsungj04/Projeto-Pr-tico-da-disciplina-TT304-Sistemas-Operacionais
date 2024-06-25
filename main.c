#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "manipuladorMatriz.c"
#include "operacoesMatriz.c"

int tamanhoMatriz;

int main(int argc, char *argv[]) {
    if (argc != 8) {
        printf("Uso: %s <numThreads> <tamanhoMatriz> <arquivoMatrizA> <arquivoMatrizB> <arquivoMatrizC> <arquivoMatrizD> <arquivoMatrizE>\n", argv[0]);
        return -1;
    }

    numThreads = atoi(argv[1]);
    tamanhoMatriz = atoi(argv[2]);
    const char *arqA = argv[3];
    const char *arqB = argv[4];
    const char *arqC = argv[5];
    const char *arqD = argv[6];
    const char *arqE = argv[7];

    pthread_mutex_init(&mutex, NULL);

    clock_t start, end;
    double tempoTotal, tempoSoma, tempoMultiplicacao, tempoReducao;

    start = clock();

    int *matrizA = NULL, *matrizB = NULL, *matrizC = NULL, *matrizD = NULL, *matrizE = NULL;

    if (!abrirArquivos(arqA, &matrizA, tamanhoMatriz) || !abrirArquivos(arqB, &matrizB, tamanhoMatriz)) {
        printf("Erro ao abrir os arquivos matrizA.dat ou matrizB.dat.\n");
        pthread_mutex_destroy(&mutex);
        return -1;
    }

    clock_t somaStart = clock();
    if (!somarMatrizes(matrizA, matrizB, tamanhoMatriz, arqD)) {
        printf("Erro ao somar as matrizes A e B e escrever no arquivo matrizD.dat.\n");
    } else {
        clock_t somaEnd = clock();
        tempoSoma = ((double) (somaEnd - somaStart)) / CLOCKS_PER_SEC;
    }

    free(matrizA);
    free(matrizB);

    if (!abrirArquivos(arqC, &matrizC, tamanhoMatriz) || !abrirArquivos(arqD, &matrizD, tamanhoMatriz)) {
        printf("Erro ao abrir os arquivos matrizC.dat ou matrizD.dat.\n");
        pthread_mutex_destroy(&mutex);
        return -1;
    }

    clock_t multStart = clock();
    if (!multiplicandoMatriz(matrizC, matrizD, tamanhoMatriz, arqE)) {
        printf("Erro ao multiplicar as matrizes C e D e escrever no arquivo matrizE.dat.\n");
    } else {
        clock_t multEnd = clock();
        tempoMultiplicacao = ((double) (multEnd - multStart)) / CLOCKS_PER_SEC;
    }

    free(matrizC);
    free(matrizD);

    if (!abrirArquivos(arqE, &matrizE, tamanhoMatriz)) {
        printf("Erro ao abrir o arquivo matrizE.dat.\n");
        pthread_mutex_destroy(&mutex);
        return -1;
    }

    clock_t reducaoStart = clock();
    int resultado = reduzindoMatriz(matrizE, tamanhoMatriz);
    clock_t reducaoEnd = clock();
    tempoReducao = ((double) (reducaoEnd - reducaoStart)) / CLOCKS_PER_SEC;

    free(matrizE);

    end = clock();
    tempoTotal = ((double) (end - start)) / CLOCKS_PER_SEC;

    pthread_mutex_destroy(&mutex);

    printf("Redução: %d\n", resultado);
    printf("Tempo soma: %.3f segundos.\n", tempoSoma);
    printf("Tempo multiplicação: %.3f segundos.\n", tempoMultiplicacao);
    printf("Tempo redução: %.3f segundos.\n", tempoReducao);
    printf("Tempo total: %.3f segundos.\n", tempoTotal);

    return 0;
}
