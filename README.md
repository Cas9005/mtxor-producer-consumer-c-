# MTXOR -- Producer / Consumer com Threads (C / POSIX)

## Descrição

Projeto académico desenvolvido em linguagem C que implementa o problema
clássico Produtor-Consumidor utilizando threads POSIX (pthreads).

O programa calcula o resultado da operação XOR sobre uma sequência de
números inteiros aleatórios usando processamento concorrente com
múltiplas tarefas consumidoras e uma tarefa produtora.

------------------------------------------------------------------------

## Funcionalidades

-   Buffer circular partilhado\
-   1 thread produtora (main)\
-   Múltiplas threads consumidoras\
-   Sincronização com mutex\
-   Controlo de acesso concorrente ao buffer\
-   Espera ativa com libertação de CPU (`sched_yield()`)\
-   Processamento paralelo com cálculo XOR

------------------------------------------------------------------------

## Conceitos Aplicados

-   Programação concorrente\
-   Sincronização de threads\
-   Regiões críticas\
-   Gestão de memória dinâmica\
-   Comunicação entre tarefas\
-   Sistemas Operativos (POSIX Threads)

------------------------------------------------------------------------

##  Tecnologias Utilizadas

-   Linguagem C\
-   POSIX Threads (pthread)\
-   GCC\
-   API POSIX / Linux

------------------------------------------------------------------------

## Compilação

``` bash
gcc -Wall -o mtxor mtxor.c -lpthread
```

------------------------------------------------------------------------

## Execução

``` bash
./mtxor dimbuf N nt
```

### Parâmetros

-   **dimbuf** → tamanho do buffer\
-   **N** → número total de itens a processar\
-   **nt** → número de threads consumidoras

### Exemplo

``` bash
./mtxor 21 22 5
```

------------------------------------------------------------------------

##  Contexto Académico

Projeto desenvolvido no âmbito da unidade curricular **Sistemas
Operativos**\
Licenciatura em Engenharia Informática

------------------------------------------------------------------------


Este projeto foi desenvolvido para fins académicos e demonstra conceitos
de programação concorrente e sincronização de threads.

------------------------------------------------------------------------

## 👨‍💻 Autor

Carlos Sousa
