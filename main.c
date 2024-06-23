#include <stdio.h>
#include <stdlib.h>

int abrindoarquivos(const char* path, int** matrix, int matrixSize) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return 0;
    }

    *matrix = (int*)malloc(matrixSize * sizeof(int));
    if (*matrix == NULL) {
        fclose(file);
        return 0;
    }

    for (int i = 0; i < matrixSize; i++) {
        fscanf(file, "%d", &(*matrix)[i]);
    }

    fclose(file);
    return 1;
}

int somaMatrizesAeB(int *matrixA, int *matrixB, int matrixSize, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return 0;
    }

    for (int i = 0; i < matrixSize; i++) {
        int sum = matrixA[i] + matrixB[i];
        fprintf(file, "%d\n", sum);
    }

    fclose(file);
    return 1;
}

int main() {
    int *matrixA, *matrixB;
    int matrixSize = 1000;

    if (!abrindoarquivos("arquivos_matrizes/matrizA.dat", &matrixA, matrixSize) ||
        !abrindoarquivos("arquivos_matrizes/matrizB.dat", &matrixB, matrixSize)) {
        printf("Erro ao abrir um ou mais arquivos.\n");
    } else {

        if (!somaMatrizesAeB(matrixA, matrixB, matrixSize, "arquivos_matrizes/matrizD.dat")) {
            printf("Erro ao somar as matrizes e escrever no arquivo.\n");
        }
    }

    free(matrixA);
    free(matrixB);

    return 0;
}