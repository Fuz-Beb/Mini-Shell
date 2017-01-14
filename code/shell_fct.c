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

// Variable globale pour le chien de garde
pid_t pid;

// Chien de garde
void alarmHandler()
{
		kill(pid, SIGKILL);
}

// Création des pipes et processus fils pour l'execution de la commande 
int exec_command(cmd * my_cmd)
{
	// Variable
	int i = 0, j = 0;
	int ** tube = NULL;

	// Retourne -1 si la commande est vide
	if (my_cmd->nb_cmd_members == 0)
		return -1;

	// Inutile d'utiliser des tubes si un seul membre dans la commande
	if (my_cmd->nb_cmd_members > 1)
	{
		// Allocation du tube
		int ** tube = (int**) malloc(sizeof(int*) * 2);

		if(tube == NULL)
		{
			printf("Malloc error : int ** tube");
			exit(EXIT_FAILURE);
		}
	}
	
	printf("AA");

	// Parcours de l'ensemble des membres un par un
	while(my_cmd->nb_cmd_members > j)
	{	
		// Inutile d'utiliser des tubes si un seul membre dans la commande
		// Utilisation de deux tubes maximum j < 2
		if (my_cmd->nb_cmd_members > 1 && j < 2)
		{
			// Allocation de la deuxième dimension du tube
			tube[i] = (int*) malloc(sizeof(int) * 2);
				
			if(tube[i] == NULL)
			{
				printf("Malloc error : tube[i]");
				exit(EXIT_FAILURE);
			}
			
			// Ouverture d'un tube
			pipe(tube[i]);
		}

		// Création du processus fils
		pid = fork();
		if(pid == 0) // Traitement du processus fils
		{
			// Inutile d'utiliser des tubes si un seul membre dans la commande
			if (my_cmd->nb_cmd_members > 1)
			{
				// Pas de données en input si il y a un seul membre
				// La deuxième condition permet d'aller dans la boucle même si i = 0
				// Je repasserai i à 0 dès qu'il arrive à 2 car nous avons besoin que de deux pipes.
				if (i != 0 || j > 0)
				{
					if (i == 0 && j != 0)
					{
						close(tube[i + 1][1]);
						dup2(tube[i + 1][0], 0);
						close(tube[i + 1][0]);
					}
					else
					{

						close(tube[i-1][1]);
						dup2(tube[i-1][0], 0);
						close(tube[i-1][0]);
					}

				}
				exit(0);
				// Pas besoin de redirigé le résultat de la commande si c'est le dernier membre
				// Il sera affiché directement sur l'écran
				if (my_cmd->nb_cmd_members > j + 1)
				{
					close(tube[i][0]);
					dup2(tube[i][1], 1);
					close(tube[i][1]);
				}
			}

			// Protection au cas ou la commande serai NULL
			if(my_cmd->cmd_members_args[j][0] != NULL)
			{
				// Execution de la commande
				if((execvp(my_cmd->cmd_members_args[j][0], my_cmd->cmd_members_args[j]))==-1)
				{
					printf("Commande incorrecte !!\n");
					exit(EXIT_FAILURE);
				}
			}
			// Valeur de retour du processus fils
			return 0;
		}

		// Incrémentation de i : Indice des tubes (0 ou 1)
		// Incrémentation de j : Indice pour le parcours des membres
		i++;
		j++;

		// Fermeture de tube si i dépasse 1.
		// Remise à zéro de i car utilisation de seulement deux tubes. Réouverture du tube 0 en haut
		if(i == 2)
		{
			close(tube[i-2][0]);
			close(tube[i-2][1]);
			i = 0;
		}
	}

	signal(SIGALRM, alarmHandler);

	//déclenchement de l'alarme
	alarm(10);

	//attente de la fin des processus fils
	for (i=1;i<=my_cmd->nb_cmd_members;i++)
		wait(NULL);

	//fin des processus fils, on annule l'alarme
	alarm(0);

	//desallocation
	/*if(my_cmd->nb_cmd_members>1)
	{
		for(i=0;i<my_cmd->nb_cmd_members-1;i++)
		{
			free(tube[i]);
		}
		free(tube);
	}*/
	return 0;
}