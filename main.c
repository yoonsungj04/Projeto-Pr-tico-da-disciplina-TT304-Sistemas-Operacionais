#include <stdio.h>
#include <stdlib.h>

int abrirArquivos(const char* caminho, int** matriz, int tamanhoMatriz) {
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        return 0;
    }

    *matriz = (int*)malloc(tamanhoMatriz * tamanhoMatriz * sizeof(int));
    if (*matriz == NULL) {
        fclose(arquivo);
        return 0;
    }

    for (int i = 0; i < tamanhoMatriz * tamanhoMatriz; i++) {
        if (fscanf(arquivo, "%d", &(*matriz)[i]) != 1) {
            free(*matriz);
            fclose(arquivo);
            return 0;
        }
    }

    fclose(arquivo);
    return 1;
}

int escreverMatrizNoArquivo(int *matriz, int tamanhoMatriz, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        perror("Falha ao abrir arquivo para escrita");
        return 0;
    }

    for (int i = 0; i < tamanhoMatriz; i++) {
        for (int j = 0; j < tamanhoMatriz; j++) {
            if (fprintf(arquivo, "%d ", matriz[i * tamanhoMatriz + j]) < 0) {
                fclose(arquivo);
                return 0;
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    return 1;
}

int somarMatrizes(int *matriz1, int *matriz2, int tamanhoMatriz, const char *nomeArquivo) {
    int *matrizResultado = (int *)malloc(tamanhoMatriz * tamanhoMatriz * sizeof(int));
    if (matrizResultado == NULL) {
        printf("Falha na alocação de memória.\n");
        return 0;
    }

    for (int i = 0; i < tamanhoMatriz; i++) {
        for (int j = 0; j < tamanhoMatriz; j++) {
            matrizResultado[i * tamanhoMatriz + j] = matriz1[i * tamanhoMatriz + j] + matriz2[i * tamanhoMatriz + j];
        }
    }

    int sucessoEscrita = escreverMatrizNoArquivo(matrizResultado, tamanhoMatriz, nomeArquivo);

    free(matrizResultado);

    return sucessoEscrita;
}

int reduzindoMatriz(int *matriz, int tamanhoMatriz) {
    int soma = 0;
    for (int i = 0; i < tamanhoMatriz * tamanhoMatriz; i++) {
        soma += matriz[i];
    }
    return soma;
}

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