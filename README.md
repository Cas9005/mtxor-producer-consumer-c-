# fdup — deteção de ficheiros duplicados por nome (via processos + exec)

Projeto da UC **21111 — Sistemas Operativos** (e-fólio A)  
Ficheiro: `fdup.c`

## Objetivo

Este programa procura **ficheiros com o mesmo nome** (duplicados *por nome*, não por conteúdo) dentro de um diretório dado, usando uma sequência de **processos** (A, B, C, D, E) e chamadas `fork()` + `exec*()` para executar comandos do sistema.

No final, imprime no terminal os nomes de ficheiros que aparecem em **mais do que uma pasta**, no formato:

```
<nome_ficheiro> <diretorio>
```

## Como funciona (pipeline)

O processo **A** (main) cria e coordena os processos:

- **B**: executa `find` e grava em `tmp1.txt`
  - Comando equivalente:
    ```bash
    find <dir> -type f -name "<nome>" -printf "%h %f\n"
    ```
  - Produz linhas no formato:
    ```
    <diretorio> <nome_ficheiro>
    ```

- **C**: executa `sort` sobre `tmp1.txt` e grava em `tmp2.txt`
  - Ordena pela **2ª coluna** (nome do ficheiro):
    ```bash
    sort -t " " -k2,2 tmp1.txt
    ```

- **D**: executa `uniq` sobre `tmp2.txt` e grava em `tmp3.txt`
  - Identifica **nomes repetidos**, ignorando a 1ª coluna (diretório) e mantendo apenas grupos duplicados:
    ```bash
    uniq -f1 -D tmp2.txt
    ```

- **E**: executa `awk` sobre `tmp3.txt` e escreve no terminal
  - Troca a ordem dos campos para:
    ```
    <nome_ficheiro> <diretorio>
    ```
    Comando:
    ```bash
    awk "{print $2,$1}" tmp3.txt
    ```

### Nota sobre os prints de PID/PPID

O programa imprime no terminal mensagens com PID/PPID:
- O processo A imprime sempre.
- Os processos B/C/D imprimem antes do `freopen()`, por isso aparecem no terminal.
- O processo E imprime o resultado final do `awk` no terminal.

## Ficheiros temporários

O programa cria estes ficheiros **no diretório indicado** (porque faz `chdir(<dir>)`):

- `tmp1.txt` — saída do `find`
- `tmp2.txt` — saída do `sort`
- `tmp3.txt` — saída do `uniq`

> O código **não remove** estes ficheiros no fim.

## Requisitos

Precisas de ter disponíveis no sistema (Linux/WSL):
- `find`
- `sort`
- `uniq`
- `awk`
- `gcc`

## Compilação

```bash
gcc -Wall -Wextra -o fdup fdup.c
```

## Execução

```bash
./fdup <diretorio> <nome_ficheiro>
```

### Exemplo

Procurar ficheiros com nome `dados.txt` dentro de `/home/user/projeto`:

```bash
./fdup /home/user/projeto dados.txt
```

Saída típica (exemplo):

```
Processo A: PID=... PPID=...
Processo B: PID=... PPID=...
Processo C: PID=... PPID=...
Processo D: PID=... PPID=...
Processo E: PID=... PPID=...
dados.txt /home/user/projeto/pasta1
dados.txt /home/user/projeto/pasta7
```

## Estrutura de argumentos e erros

- Se não forem passados **exatamente 2 argumentos**, o programa termina com:
  ```
  Erro numero errado de argumentos
  ```
- Se o diretório não existir / não for acessível:
  ```
  Erro: Não foi possível acessar o diretório especificado.
  ```

## 👨‍💻 Autor

cas9005
