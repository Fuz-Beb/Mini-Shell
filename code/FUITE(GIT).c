#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "shell_fct.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define RAPPEND 1
#define NRAPPEND 0

pid_t pid;

//fonction chien de garde
void alarmHandler()
{
		printf("Processus fils trop long : Arrêt forcé du processus fils\n");
		kill(pid, SIGKILL);
}

void redirection(char valeur[7], char nomFichier[1024], int type)
{
	int fd;
	// redirection <
	if (strcmp(valeur, "STDIN")==0)
	{
		fd=open(nomFichier, O_RDONLY);
		if(fd==-1)
			printf("Erreur \n");
		if(dup2(fd, 0) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(fd);
	}
	//meme cas pour stdout et stderr, sauf redirection
	else if (strcmp(valeur,"STDOUT")==0 || strcmp(valeur,"STDERR")==0)
	{
		//redirection > ou 2>
		if(type==0)
		{
			fd=open(nomFichier,O_RDWR|O_TRUNC|O_CREAT, S_IRWXU);
			if(fd==-1)
				printf("Erreur \n");
		}
		//redirection >> ou 2>>
		else if (type==1)
		{
			fd=open(nomFichier,O_RDWR| O_APPEND);
			if(fd==-1)
				printf("Erreur \n");
		}
		if (strcmp(valeur,"STDOUT")==0)
		{
			if(dup2(fd, 1) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if(dup2(fd, 2) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
		}
		close(fd);
	}
}

int exec_command(cmd * c)
{
	int ** tube=NULL;
	unsigned int i=0;

	if (c->nb_cmd_members==0)
		return -1;

	if (c->nb_cmd_members>1)
	{
		tube=(int **)malloc(sizeof(int*)*c->nb_cmd_members-1);
		if(tube==NULL)
		{
			perror("allocation raté // tube");
			exit(EXIT_FAILURE);
		}
	}
	if ((strcmp(c->init_cmd,"exit"))==0)
		exit(0);

	while(i<c->nb_cmd_members)
	{
		i++;
		//quand i sera à 3, on ne fait plus le pipe
		if (i<c->nb_cmd_members && c->nb_cmd_members>1)
		{
			//allocation des tubes
			tube[i-1]=(int *)malloc(2*sizeof(int));
			if(tube[i-1]==NULL)
			{
				perror("allocation raté // tube[i]");
				exit(EXIT_FAILURE);
			}

			pipe(tube[i-1]);
		}

		pid = fork();
		//code du processus fils
		if(pid == 0)
		{
			//redirections
			if(c->redirection[i-1][STDIN]!=NULL)
				redirection("STDIN", c->redirection[i-1][STDIN], 0);
			if(c->redirection[i-1][STDOUT]!=NULL)
			{
				if (c->redirection_type[i-1][STDOUT]==NRAPPEND)
					redirection("STDOUT", c->redirection[i-1][STDOUT], 0);
				else if(c->redirection_type[i-1][STDOUT]==RAPPEND)
					redirection("STDOUT", c->redirection[i-1][STDOUT], 1);
			}
			if (c->redirection[i-1][STDERR]!=NULL)
			{
				if (c->redirection_type[i-1][STDERR]==NRAPPEND)
					redirection("STDERR",c->redirection[i-1][STDERR], 0);
				else if(c->redirection_type[i-1][STDERR]==RAPPEND)
					redirection("STDERR", c->redirection[i-1][STDERR], 1);
			}

			//tubes
			if (c->nb_cmd_members>1)
			{
				//ne passe pas la première fois car on ne fait que de l'écriture
				if (i>1)
				{
					close(tube[i-2][1]);
					dup2(tube[i-2][0], 0);
					close(tube[i-2][0]);
				}
				//ne passe pas la dernière fois, on n'écrie plus
				if (i<c->nb_cmd_members)
				{
					close(tube[i-1][0]);
					dup2(tube[i-1][1], 1);
					close(tube[i-1][1]);
				}
			}

			if(c->cmd_members_args[i-1][0]!=NULL)
			{ // execution locale
				if((execvp(c->cmd_members_args[i-1][0], c->cmd_members_args[i-1]))==-1)
				{
					perror("Commande inconnue \n");
					exit(EXIT_FAILURE);
				}
			}
			
			return 0;
		}

		if(i>1)
		{
			close(tube[i-2][0]);
			close(tube[i-2][1]);
		}

	}
	signal(SIGALRM, alarmHandler);

	//déclenchement de l'alarme
	alarm(10);

	//attente de la fin des processus fils
	for (i=1;i<=c->nb_cmd_members;i++)
		wait(NULL);

	//fin des processus fils, on annule l'alarme
	alarm(0);

	//desallocation
	if(c->nb_cmd_members>1)
	{
		for(i=0;i<c->nb_cmd_members-1;i++)
		{
			free(tube[i]);
		}
		free(tube);
	}
	return 0;
}