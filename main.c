#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int tamanhoMatriz;
int numThreads;

typedef struct {
    const char* caminho;
    int **matriz;
    int tamanhoMatriz;
} ThreadLeituraArgs;

typedef struct {
    int *matriz1;
    int *matriz2;
    int *resultado;
    int start;
    int end;
    int tamanhoMatriz;
} ThreadProcessamentoArgs;

typedef struct {
    const char *nomeArquivo;
    int *matriz;
    int tamanhoMatriz;
} ThreadEscritaArgs;

pthread_mutex_t mutex;
int somaReducao = 0;

void* threadLeitura(void *args) {
    ThreadLeituraArgs *leituraArgs = (ThreadLeituraArgs*)args;
    const char* caminho = leituraArgs->caminho;
    int **matriz = leituraArgs->matriz;
    int tamanhoMatriz = leituraArgs->tamanhoMatriz;

    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        perror("Falha ao abrir arquivo");
        pthread_exit((void*)0);
    }

    *matriz = (int*)malloc(tamanhoMatriz * tamanhoMatriz * sizeof(int));
    if (*matriz == NULL) {
        fclose(arquivo);
        perror("Falha na alocação de memória");
        pthread_exit((void*)0);
    }

    for (int i = 0; i < tamanhoMatriz * tamanhoMatriz; i++) {
        if (fscanf(arquivo, "%d", &(*matriz)[i]) != 1) {
            free(*matriz);
            fclose(arquivo);
            perror("Falha na leitura do arquivo");
            pthread_exit((void*)0);
        }
    }

    fclose(arquivo);
    pthread_exit((void*)1);
}

void* threadEscrita(void *args) {
    ThreadEscritaArgs *escritaArgs = (ThreadEscritaArgs*)args;
    const char *nomeArquivo = escritaArgs->nomeArquivo;
    int *matriz = escritaArgs->matriz;
    int tamanhoMatriz = escritaArgs->tamanhoMatriz;

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        perror("Falha ao abrir arquivo para escrita");
        pthread_exit((void*)0);
    }

    for (int i = 0; i < tamanhoMatriz; i++) {
        for (int j = 0; j < tamanhoMatriz; j++) {
            if (fprintf(arquivo, "%d ", matriz[i * tamanhoMatriz + j]) < 0) {
                fclose(arquivo);
                perror("Falha ao escrever no arquivo");
                pthread_exit((void*)0);
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    pthread_exit((void*)1);
}

void* somarMatrizesThread(void *args) {
    ThreadProcessamentoArgs *processamentoArgs = (ThreadProcessamentoArgs*)args;
    int *matriz1 = processamentoArgs->matriz1;
    int *matriz2 = processamentoArgs->matriz2;
    int *resultado = processamentoArgs->resultado;
    int start = processamentoArgs->start;
    int end = processamentoArgs->end;

    for (int i = start; i < end; i++) {
        resultado[i] = matriz1[i] + matriz2[i];
    }
    return NULL;
}

void* multiplicandoMatrizThread(void *args) {
    ThreadProcessamentoArgs *processamentoArgs = (ThreadProcessamentoArgs*)args;
    int *matriz1 = processamentoArgs->matriz1;
    int *matriz2 = processamentoArgs->matriz2;
    int *resultado = processamentoArgs->resultado;
    int start = processamentoArgs->start;
    int end = processamentoArgs->end;
    int tamanhoMatriz = processamentoArgs->tamanhoMatriz;

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

void* reduzindoMatrizThread(void *args) {
    ThreadProcessamentoArgs *processamentoArgs = (ThreadProcessamentoArgs*)args;
    int *matriz = processamentoArgs->matriz1;
    int start = processamentoArgs->start;
    int end = processamentoArgs->end;
    int soma = 0;

    for (int i = start; i < end; i++) {
        soma += matriz[i];
    }

    pthread_mutex_lock(&mutex);
    somaReducao += soma;
    pthread_mutex_unlock(&mutex);

    return NULL;
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

    // Passo 1 - Leitura da Matriz A e Matriz B usando 2 threads do tipo thread leitura
    pthread_t leituraThreads[2];
    ThreadLeituraArgs leituraArgs[2] = {
        {arqA, &matrizA, tamanhoMatriz},
        {arqB, &matrizB, tamanhoMatriz}
    };

    for (int i = 0; i < 2; i++) {
        pthread_create(&leituraThreads[i], NULL, threadLeitura, &leituraArgs[i]);
    }

    for (int i = 0; i < 2; i++) {
        void* success;
        pthread_join(leituraThreads[i], &success);
        if (success == (void*)0) {
            printf("Erro ao abrir os arquivos matrizA.dat ou matrizB.dat.\n");
            pthread_mutex_destroy(&mutex);
            return -1;
        }
    }

    // Passo 2 - Soma das Matrizes A + B = D usando T threads do tipo thread processamento
    matrizD = (int *)malloc(tamanhoMatriz * tamanhoMatriz * sizeof(int));
    if (matrizD == NULL) {
        printf("Falha na alocação de memória para matrizD.\n");
        pthread_mutex_destroy(&mutex);
        return -1;
    }

    pthread_t somaThreads[numThreads];
    ThreadProcessamentoArgs somaArgs[numThreads];
    int elementosPorThread = (tamanhoMatriz * tamanhoMatriz) / numThreads;
    int restante = (tamanhoMatriz * tamanhoMatriz) % numThreads;

    clock_t somaStart = clock();
    for (int i = 0; i < numThreads; i++) {
        somaArgs[i] = (ThreadProcessamentoArgs){matrizA, matrizB, matrizD, i * elementosPorThread, (i + 1) * elementosPorThread, tamanhoMatriz};
        if (i == numThreads - 1) {
            somaArgs[i].end += restante;
        }
        pthread_create(&somaThreads[i], NULL, somarMatrizesThread, &somaArgs[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(somaThreads[i], NULL);
    }
    clock_t somaEnd = clock();
    tempoSoma = ((double) (somaEnd - somaStart)) / CLOCKS_PER_SEC;

    free(matrizA);
    free(matrizB);

    // Passo 3 - Gravação da Matriz D e Leitura da Matriz C
    pthread_t escritaLeituraThreads[2];
    ThreadEscritaArgs escritaArgs = {arqD, matrizD, tamanhoMatriz};
    ThreadLeituraArgs leituraCArgs = {arqC, &matrizC, tamanhoMatriz};

    pthread_create(&escritaLeituraThreads[0], NULL, threadEscrita, &escritaArgs);
    pthread_create(&escritaLeituraThreads[1], NULL, threadLeitura, &leituraCArgs);

    for (int i = 0; i < 2; i++) {
        void* success;
        pthread_join(escritaLeituraThreads[i], &success);
        if (success == (void*)0) {
            printf("Erro ao escrever a matrizD.dat ou ler o arquivo matrizC.dat.\n");
            pthread_mutex_destroy(&mutex);
            return -1;
        }
    }

    // Passo 4 - Multiplicação das Matrizes D x C = E usando T threads do tipo thread processamento
    matrizE = (int *)malloc(tamanhoMatriz * tamanhoMatriz * sizeof(int));
    if (matrizE == NULL) {
        printf("Falha na alocação de memória para matrizE.\n");
        pthread_mutex_destroy(&mutex);
        return -1;
    }

    pthread_t multThreads[numThreads];
    ThreadProcessamentoArgs multArgs[numThreads];
    int linhasPorThread = tamanhoMatriz / numThreads;
    restante = tamanhoMatriz % numThreads;

    clock_t multStart = clock();
    for (int i = 0; i < numThreads; i++) {
        multArgs[i] = (ThreadProcessamentoArgs){matrizD, matrizC, matrizE, i * linhasPorThread, (i + 1) * linhasPorThread, tamanhoMatriz};
        if (i == numThreads - 1) {
            multArgs[i].end += restante;
        }
        pthread_create(&multThreads[i], NULL, multiplicandoMatrizThread, &multArgs[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(multThreads[i], NULL);
    }
    clock_t multEnd = clock();
    tempoMultiplicacao = ((double) (multEnd - multStart)) / CLOCKS_PER_SEC;

    free(matrizD);
    free(matrizC);

    // Passo 5 - Gravação da Matriz E e Redução da Matriz E
    ThreadEscritaArgs escritaEArgs = {arqE, matrizE, tamanhoMatriz};
    pthread_create(&escritaLeituraThreads[0], NULL, threadEscrita, &escritaEArgs);
    pthread_join(escritaLeituraThreads[0], NULL);

    pthread_t reducaoThreads[numThreads];
    ThreadProcessamentoArgs reducaoArgs[numThreads];
    elementosPorThread = (tamanhoMatriz * tamanhoMatriz) / numThreads;
    restante = (tamanhoMatriz * tamanhoMatriz) % numThreads;

    somaReducao = 0;
    clock_t reducaoStart = clock();
    for (int i = 0; i < numThreads; i++) {
        reducaoArgs[i] = (ThreadProcessamentoArgs){matrizE, NULL, NULL, i * elementosPorThread, (i + 1) * elementosPorThread, tamanhoMatriz};
        if (i == numThreads - 1) {
            reducaoArgs[i].end += restante;
        }
        pthread_create(&reducaoThreads[i], NULL, reduzindoMatrizThread, &reducaoArgs[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(reducaoThreads[i], NULL);
    }
    clock_t reducaoEnd = clock();
    tempoReducao = ((double) (reducaoEnd - reducaoStart)) / CLOCKS_PER_SEC;

    free(matrizE);

    end = clock();
    tempoTotal = ((double) (end - start)) / CLOCKS_PER_SEC;

    pthread_mutex_destroy(&mutex);

    printf("Redução: %d\n", somaReducao);
    printf("Tempo soma: %.3f segundos.\n", tempoSoma);
    printf("Tempo multiplicação: %.3f segundos.\n", tempoMultiplicacao);
    printf("Tempo redução: %.3f segundos.\n", tempoReducao);
    printf("Tempo total: %.3f segundos.\n", tempoTotal);

    return 0;
}
