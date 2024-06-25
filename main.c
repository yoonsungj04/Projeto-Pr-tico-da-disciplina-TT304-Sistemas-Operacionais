#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int tamanhoMatriz;
int numThreads;

typedef struct {
    int *matriz1;
    int *matriz2;
    int *resultado;
    int start;
    int end;
    int tamanhoMatriz;
} ThreadArgs;

pthread_mutex_t mutex;
int somaReducao = 0;

int abrirArquivos(const char* caminho, int** matriz, int tamanhoMatriz) {
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        perror("Falha ao abrir arquivo");
        return 0;
    }

    *matriz = (int*)malloc(tamanhoMatriz * tamanhoMatriz * sizeof(int));
    if (*matriz == NULL) {
        fclose(arquivo);
        perror("Falha na alocação de memória");
        return 0;
    }

    for (int i = 0; i < tamanhoMatriz * tamanhoMatriz; i++) {
        if (fscanf(arquivo, "%d", &(*matriz)[i]) != 1) {
            free(*matriz);
            fclose(arquivo);
            perror("Falha na leitura do arquivo");
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
                perror("Falha ao escrever no arquivo");
                return 0;
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    return 1;
}

void* somarMatrizesThread(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs*)args;
    int *matriz1 = threadArgs->matriz1;
    int *matriz2 = threadArgs->matriz2;
    int *resultado = threadArgs->resultado;
    int start = threadArgs->start;
    int end = threadArgs->end;

    for (int i = start; i < end; i++) {
        resultado[i] = matriz1[i] + matriz2[i];
    }
    return NULL;
}

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

void* multiplicandoMatrizThread(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs*)args;
    int *matriz1 = threadArgs->matriz1;
    int *matriz2 = threadArgs->matriz2;
    int *resultado = threadArgs->resultado;
    int start = threadArgs->start;
    int end = threadArgs->end;
    int tamanhoMatriz = threadArgs->tamanhoMatriz;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < tamanhoMatriz; j++) {
            resultado[i * tamanhoMatriz + j] = 0;
            for (int k = 0; k < tamanhoMatriz; k++) {
                resultado[i * tamanhoMatriz + j] += matriz1[i * tamanhoMatriz + k] * matriz2[k * tamanhoMatriz + j];
            }
        }
    }
    return NULL;
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

void* reduzindoMatrizThread(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs*)args;
    int *matriz = threadArgs->matriz1;
    int start = threadArgs->start;
    int end = threadArgs->end;
    int soma = 0;

    for (int i = start; i < end; i++) {
        soma += matriz[i];
    }

    pthread_mutex_lock(&mutex);
    somaReducao += soma;
    pthread_mutex_unlock(&mutex);

    return NULL;
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
