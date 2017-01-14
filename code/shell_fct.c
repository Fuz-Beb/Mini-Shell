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
	int i = 0, status;
	int ** tube = NULL;

	// Retourne -1 si la commande est vide
	if (my_cmd->nb_cmd_members == 0)
		return -1;

	// Inutile d'utiliser des tubes si un seul membre dans la commande
	if (my_cmd->nb_cmd_members > 1)
	{
		// Allocation du tube
		tube = (int**) malloc(sizeof(int*) * (my_cmd->nb_cmd_members - 1));

		if(tube == NULL)
		{
			printf("Malloc error : int ** tube");
			exit(EXIT_FAILURE);
		}
	}

	// Parcours de l'ensemble des membres un par un
	while(my_cmd->nb_cmd_members > i)
	{
		// Inutile d'utiliser des tubes si un seul membre dans la commande
		if (my_cmd->nb_cmd_members > 1)
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
				if (i > 0)
				{
                    close(tube[i-1][1]);
                    dup2(tube[i-1][0], 0);
					close(tube[i-1][0]);
				}
				// Pas besoin de redirigé le résultat de la commande si c'est le dernier membre
				// Il sera affiché directement sur le terminal
				if (my_cmd->nb_cmd_members > i + 1)
				{
					close(tube[i][0]);
					dup2(tube[i][1], 1);
					close(tube[i][1]);
				}
			}

			// Protection au cas ou la commande serai égale à NULL
			if(my_cmd->cmd_members_args[i][0] != NULL)
			{
				// Execution de la commande avec les arguments de la structure
				if((execvp(my_cmd->cmd_members_args[i][0], my_cmd->cmd_members_args[i])) == -1)
				{
					printf("Commande incorrecte !!\n");
					exit(EXIT_FAILURE);
				}
			}
			// Valeur de retour du processus fils
			return 0;
		}

		// Incrémentation de i : Passage au membre suivant
		i++;

		// Fermeture des tubes au fur et à mesure de leur création
		// Ils sont fermés après leur utilisation
		// Le deuxième argument permet d'éviter de close un tube non initialisé
		if(i > 1 && my_cmd->nb_cmd_members > 1)
		{
			close(tube[i-2][0]);
			close(tube[i-2][1]);
		}
	}

	// Mise en place du signal
	signal(SIGALRM, alarmHandler);

	// Activation de l'alarme
	alarm(5);

	// Synhronisation avec les processus fils
	waitpid(pid,&status,0);

	// Libération de la mémoire mise sur le tas
	if (my_cmd->nb_cmd_members > 1)
	{
		i = 0;

		while (my_cmd->nb_cmd_members-1 > i)
		{
			free(tube[i]);
			i++;
		}
		free(tube);
	}

	// Valeur de retour du processus père
	return 0;
}
