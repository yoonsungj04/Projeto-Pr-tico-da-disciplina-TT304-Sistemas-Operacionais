#ifndef THREADOPERACOESMATRIZ_H
#define THREADOPERACOESMATRIZ_H

#include <pthread.h>

// Estrutura para argumentos das threads de processamento
typedef struct {
    int *matriz1;
    int *matriz2;
    int *resultado;
    int start;
    int end;
    int tamanhoMatriz;
} ThreadProcessamentoArgs;

// Protótipos das funções das threads
void* somarMatrizesThread(void *args);
void* multiplicandoMatrizThread(void *args);
void* reduzindoMatrizThread(void *args);

// Variável global para soma na redução
extern int somaReducao;

// Mutex para controle de acesso à variável global somaReducao
extern pthread_mutex_t mutex;

#endif // THREADOPERACOESMATRIZ_H