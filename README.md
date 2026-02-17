# MTXOR — Produtor/Consumidor com Threads (C / POSIX)

##  Descrição

Projeto académico em C que implementa o problema clássico **Produtor–Consumidor** usando **POSIX threads (pthreads)**.

O programa gera uma sequência de **inteiros pseudo-aleatórios** (com `rand()` e semente fixa) e calcula o **XOR** global desses valores através de **múltiplas threads consumidoras** e **uma tarefa produtora** (o `main`).

---

##  Funcionalidades (conforme `mtxor.c`)

- **Buffer circular** partilhado (vetor dinâmico) com índices `bufe` (entrada) e `bufs` (saída)
- **1 produtora** (função `main`) e **nt consumidoras** (`tcon`)
- Sincronização com:
  - **Mutex** (`pthread_mutex_t mtx_buf`) para região crítica do buffer
  - **Variáveis de condição**:
    - `nvazio` (consumidor espera quando o buffer está vazio)
    - `ncheio` (sinalização quando há espaço/consumo)
- Quando o buffer está cheio, o produtor faz **espera ativa** com libertação de CPU via `sched_yield()`
- Cada consumidora acumula um **XOR local** e no fim atualiza o XOR global (`fxor`) e o total de operações (`top`)

---

##  Conceitos aplicados

- Programação concorrente
- Exclusão mútua (regiões críticas)
- Sincronização com *condition variables*
- Buffer circular
- Gestão de memória dinâmica

---

##  Tecnologias

- Linguagem C
- POSIX Threads (`pthread`)
- GCC / Linux (API POSIX)

---

##  Compilação

Recomendado (mais portátil/“correto” do que `-lpthread` em alguns ambientes):

```bash
gcc -Wall -pthread -o mtxor mtxor.c
```

(Em muitos sistemas isto também funciona: `gcc -Wall -o mtxor mtxor.c -lpthread`.)

---

##  Execução

```bash
./mtxor dimbuf N nt
```

### Parâmetros

- **dimbuf** → tamanho do buffer (inteiro positivo)
- **N** → número total de itens a processar (inteiro positivo)
- **nt** → número de threads consumidoras (inteiro positivo)

### Exemplo

```bash
./mtxor 21 22 5
```

---

## Notas importantes sobre o funcionamento

- O programa valida que existem **3 argumentos** e que são **inteiros positivos**.
- A semente do gerador é fixa (`srand(737)`), logo a sequência gerada é reprodutível.
- O buffer é **pré-preenchido** inicialmente com até `dimbuf` itens antes de criar as consumidoras.
- As consumidoras retiram itens do buffer em blocos até `dimbuf/nt` por iteração (limitado por `bufc`).
- **Nota de coerência**: o código aceita valores `>= 1` (condição `dimbuf < 1 || N < 1 || nt < 1`), mas a mensagem de erro diz “maiores que 1”.

---

## Contexto académico

Unidade curricular: **Sistemas Operativos**  
(e-fólio B 2023–24)

---

## 👨‍💻 Autor

Carlos Sousa
