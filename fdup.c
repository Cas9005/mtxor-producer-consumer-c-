/*
** UC: 21111 - Sistemas Operativos
** e-fólio A 2023-24 (fdup.c)
**
** Aluno: 2300971 - Carlos Sousa
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int narg, char *argv[]) 
{
	if (narg != 3)
	{
        	printf("Erro numero errado de argumentos \n");
        	exit(EXIT_FAILURE);
		}
 
 // testa se o programa existe a pasta
	if (chdir(argv[1]) == -1)
	{
		printf("Erro: Não foi possível acessar o diretório especificado.\n");
		return 1;
		}

    printf("Processo A: PID=%d PPID=%d \n",getpid() ,getppid());

 // Cria o processo B
	pid_t pid_b = fork();
	if (pid_b == -1)
	{
		printf("Erro ao criar processo B");
		exit(EXIT_FAILURE);
		} 
	else if (pid_b == 0)
	{
 // Processo B 
		printf("Processo B: PID=%d PPID=%d\n", getpid(), getppid());
		freopen("tmp1.txt", "w", stdout);
 // cria a matriz pb para poder ser chamada com a função execvp
		char *pb[] = {"find", argv[1], "-type", "f", "-name", argv[2], "-printf", "%h %f\n", NULL};
    
 // Executo o comando find com função execvp
		execvp("find", pb);
		exit(EXIT_FAILURE);
		}

 // Espera o processo B acabar para criar o processo C   
	wait(NULL);

 // Cria do processo C
	pid_t pid_c = fork();
	if (pid_c == -1)
	{
		printf("Erro ao criar processo C");
		exit(EXIT_FAILURE);
		} 
	else if (pid_c == 0) 
	{
 // Processo C 
		printf("Processo C: PID=%d PPID=%d\n", getpid(), getppid());
		freopen("tmp2.txt", "w", stdout);
  // Executa o comando sort com função execl
		execl("/bin/sort", "sort", "-t", " ", "-k2,2", "tmp1.txt", NULL);
		exit(EXIT_FAILURE);
		}

   // Espera o processo C acabar para criar o processo D
	wait(NULL);

   // Cria do processo D
	pid_t pid_d = fork();
		if (pid_d == -1) 
	{
        	printf("Erro ao criar processo D");
        	exit(EXIT_FAILURE);
		}
	else if (pid_d == 0)
	{
  // Processo filho D
		printf("Processo D: PID=%d PPID=%d\n", getpid(), getppid());
		freopen("tmp3.txt", "w", stdout);
 // Executa o comando uniq com função execlp
		execlp("uniq", "uniq", "-f1", "-D", "tmp2.txt", NULL);
		exit(EXIT_FAILURE);
		}

  // Espera o processo D acabar para criar o processo E
	wait(NULL);

 // Cria do processo E
	pid_t pid_e = fork();
	if (pid_e == -1)
	{
		printf("Erro ao criar processo E");
		exit(EXIT_FAILURE);
		}
	else if (pid_e == 0)
	{
 // Processo filho E 
		printf("Processo E: PID=%d PPID=%d\n", getpid(), getppid());
 // Executa o comando awk com função execl
		execl("/bin/awk", "awk", "{print $2,$1}", "tmp3.txt", NULL);
		exit(EXIT_FAILURE);
		}

 // Aguarda a conclusão do processo E antes de continuar
	wait(NULL);
    	return 0;
}