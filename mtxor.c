/*
** UC: 21111 - Sistemas Operativos
** e-fólio B 2023-24 (mtxor.c)
**
** Aluno: 2300971 - Carlos Sousa
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

// Buffer e variáveis de controle
int *buffer;

int bufc = 0;
int bufe = 0;
int bufs = 0;
int top = 0;
int fxor = 0;

// Função verifica se é um número e se é inteiro
int inteiro(const char *arg) 
{
    while (*arg) 
    {
        if (*arg < '0' || *arg > '9') 
        {
            return 0;
        }
            arg++;
        }
    return 1;
}

pthread_mutex_t mtx_buf = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t nvazio = PTHREAD_COND_INITIALIZER;
pthread_cond_t ncheio = PTHREAD_COND_INITIALIZER;

struct argt
{
    int id;
    int nitems;
    int dimbuf;
    int nt;
};

// Função da tarefa consumidora
void *tcon (void *args) 
{
    struct argt *tcon = (struct argt *)args;
    int id = tcon->id;
    int lxor = 0;
    int lop = 0;
    int itemsc;

    while (1) {
        pthread_mutex_lock(&mtx_buf);

        while (bufc == 0 && tcon->nitems > 0) 
        {
            pthread_cond_wait(&nvazio, &mtx_buf);
        }

        if (tcon->nitems <= 0 && bufc == 0) 
        
        {
            pthread_mutex_unlock(&mtx_buf);
            break;
        }

        itemsc = tcon->dimbuf / tcon->nt;
        if (bufc < itemsc) 
        {
            itemsc= bufc;
        }

        for (int i = 0; i < itemsc; i++) 
        {
            int item = buffer[bufs];
            bufs = (bufs + 1) % tcon->dimbuf;
            bufc--;
            lxor ^= item;
            lop++;
            tcon->nitems--;
        }

        pthread_cond_signal(&ncheio);
        pthread_mutex_unlock(&mtx_buf);
    }

    printf("T%d: %d operações realizadas\n", id, lop);

    pthread_mutex_lock(&mtx_buf);
    top += lop;
    fxor ^= lxor;
    pthread_mutex_unlock(&mtx_buf);

    return NULL;
}

// Função da tarefa produtora (main)
int main(int argc, char *argv[]) 
{
    if (argc != 4) 
    {
        printf("Número errado de argumentos\n");
        exit(EXIT_FAILURE);
    }

    if (!inteiro(argv[1]) || !inteiro(argv[2]) || !inteiro(argv[3])) 
    {
        printf("Argumentos não são números inteiros positivos\n");
        exit(EXIT_FAILURE);
    }

    int dimbuf = atoi(argv[1]);
    int N = atoi(argv[2]);
    int nt = atoi(argv[3]);

    if (dimbuf < 1 || N < 1 || nt < 1) 
    {
        printf("Os valores dos argumentos devem ser maiores que 1\n");
        exit(EXIT_FAILURE);
    }

    printf("Cálculo XOR de sequência com %d itens, %d tarefas e buffer de %d itens\n", N, nt, dimbuf);

    buffer = (int *)malloc(dimbuf * sizeof(int));

    pthread_t threads[nt];
    struct argt args[nt];

    srand(737);

    // Preencher buffer inicialmente
    for (int i = 0; i < dimbuf && N > 0; i++) 
    {
        buffer[bufe] = rand();
        bufe = (bufe + 1) % dimbuf;
        bufc++;
        N--;
    }

    // Criar tarefas consumidoras
    for (int i = 0; i < nt; i++) 
    {
        args[i].id = i;
        args[i].nitems = N;
        args[i].dimbuf = dimbuf;
        args[i].nt = nt;
        pthread_create(&threads[i], NULL, tcon, (void *)&args[i]);
    }

    // Produtor gera e insere itens no buffer
    while (N > 0) 
    {
        pthread_mutex_lock(&mtx_buf);

        if (bufc == dimbuf) 
        {
            pthread_mutex_unlock(&mtx_buf);
            sched_yield(); // Liberar CPU e permitir que outras threads executem
            continue;
        }

        buffer[bufe] = rand();
        bufe = (bufe + 1) % dimbuf;
        bufc++;
        N--;

        pthread_cond_signal(&nvazio);
        pthread_mutex_unlock(&mtx_buf);
    }

    // Sinalizar para consumidores terminarem
    pthread_mutex_lock(&mtx_buf);
    for (int i = 0; i < nt; i++) 
    {
        args[i].nitems = 0;
    }
    pthread_cond_broadcast(&nvazio);
    pthread_mutex_unlock(&mtx_buf);

    // Aguardar tarefas consumidoras terminarem
    for (int i = 0; i < nt; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    // Imprimir resultados finais
    printf("Número total de operações: %d\n", top);
    printf("Resultado final do XOR: %d\n", fxor);

    free(buffer);
    return EXIT_SUCCESS;
}

