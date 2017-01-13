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

int exec_command(cmd* my_cmd){
    
	//Déclaration des variables
	int i = 0, j = 0, status;
	char buffer[300];

	// Si la commande saisie est vide alors aucun traitement
	if (strlen(my_cmd->init_cmd) == 0)
		return 0;

	// Allocation mémoire des pipes
	pid_t * pidProcess = (pid_t*) malloc (sizeof(pid_t) * my_cmd->nb_cmd_members);

	if (pidProcess == NULL)
	{
		printf("Malloc error : pidProcess");
		exit(EXIT_FAILURE);
	}

	int ** tube = (int**) malloc (sizeof(int*));

	if (tube == NULL)
	{
		printf("Malloc error : tube");
		exit(EXIT_FAILURE);
	}
    

	// Parcours l'ensemble des membres. A chaque membre, un processus sera créer pour executer la commande du membre
    while (my_cmd->nb_cmd_members > i)
    {
    	// Augmentation de la mémoire du tube pour un autre pipe
	    tube[i] = (int*) malloc(sizeof(int) * 2);

		if (tube[i] == NULL)
		{
			printf("Malloc error : tube[i]");
			exit(EXIT_FAILURE);
		}

		//Ouverture du tube
    	pipe(tube[i]);

    	// Création du processus fils
    	pidProcess[i] = fork();

    	// Traitement du processus fils
	    if(pidProcess[i] == 0)
	    {
	    	if (i != 0)
	    	{
	    		// Redirection de l'entrée si la commande à plus de 1 membre
	            close(tube[i-1][1]);
	            dup2(tube[i-1][0], 0);
	            close(tube[i-1][0]);
	    	}

	        // Redirection de la sortie
	        close(tube[i][0]);
	        dup2(tube[i][1], 1);
	        close(tube[i][1]);
	        
	        //Execute la commande
	        if (my_cmd->cmd_members_args[i][j] != NULL)
	        {
	        	if(execvp(my_cmd->cmd_members_args[i][j], my_cmd->cmd_members_args[i]) == -1) {
	            	perror("execlp");
	            	exit(errno);
	        	}

	        	// Auto kill du processus fils 
	        	kill(0, SIGTERM);
	        }
	    }
	    else // Traitement processus parent
	    {
	    	// Synchronisation avec le fils
	    	waitpid(pidProcess[i],&status,0);

	    	// Passage au membre suivant
	    	i++;

	    	// Fermeture des tubes non utilisés
	    	if (i != 0 && i != 1)
	    	{
	    		close(tube[i-2][0]);
	    		close(tube[i-2][1]);
	    	}
	    		
		}
	}

	// Redirection de l'entrée pour récupérer le résultat avec fgets
    close(tube[i-1][1]);
    dup2(tube[i-1][0], 0);
    close(tube[i-1][0]);

    // Affichage du résultat de la commande
    while(fgets(buffer, 300, stdin) != NULL)
        printf("%s", buffer);

    // Fermture du dernier pipe après affichage
    close(tube[i-1][0]);
	close(tube[i-1][1]);

	return 0;
}