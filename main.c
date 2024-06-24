#include <stdio.h>
#include <stdlib.h>
#include "matriz.c"

int main() {
    int *matrizA, *matrizB, *matrizC, *matrizD, *matrizE;
    int tamanhoMatriz = 5;

    if (!abrirArquivos("arquivos_matrizes/matrizA.dat", &matrizA, tamanhoMatriz) ||
        !abrirArquivos("arquivos_matrizes/matrizB.dat", &matrizB, tamanhoMatriz)) {
        printf("Erro ao abrir um ou mais arquivos de matrizA e matrizB.\n");
    } else {
        if (!somarMatrizes(matrizA, matrizB, tamanhoMatriz, "arquivos_matrizes/matrizD.dat")) {
            printf("Erro ao somar as matrizes A e B e escrever no arquivo matrizD.dat.\n");
        }
    }

    free(matrizA);
    free(matrizB);

    if (!abrirArquivos("arquivos_matrizes/matrizC.dat", &matrizC, tamanhoMatriz) ||
        !abrirArquivos("arquivos_matrizes/matrizD.dat", &matrizD, tamanhoMatriz)) {
        printf("Erro ao abrir um ou mais arquivos de matrizC e matrizD.\n");
    } else {
        if (!somarMatrizes(matrizC, matrizD, tamanhoMatriz, "arquivos_matrizes/matrizE.dat")) {
            printf("Erro ao somar as matrizes C e D e escrever no arquivo matrizE.dat.\n");
        }
    }

    free(matrizC);
    free(matrizD);

    if (!abrirArquivos("arquivos_matrizes/matrizE.dat", &matrizE, tamanhoMatriz)) {
        printf("Erro ao abrir o arquivo matrizE.dat.\n");
        return -1;
    }

    int resultado = reduzindoMatriz(matrizE, tamanhoMatriz);
    printf("A soma de todos os elementos da matriz E é: %d\n", resultado);

    free(matrizE);

    return 0;
}