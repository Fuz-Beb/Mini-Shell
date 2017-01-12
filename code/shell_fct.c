#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
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
	    		// Redirection de l'entrée
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
	        	kill(pidProcess[i], SIGTERM);
	        }
	        else
	        {
	        	printf("AAAAAAAAAAAAAAAAAAAAAAAAAAA");
	        }
	    }
	    else // Traitement processus parent
	    {
	    	i++;

	    	if (i != 0 && i != 1)
	    	{ // A REVOIR CAR LE -2 NE FERME PAS l'AVANT DERNIER
	    		close(tube[i-2][0]);
	    		close(tube[i-2][1]);
	    	}
	    	waitpid(pidProcess[i],&status,0);	
		}
	}

	//Redirect input (from pipeC3P)
    close(tube[i-1][1]);
    dup2(tube[i-1][0], 0);
    close(tube[i-1][0]);

    while(fgets(buffer, 300, stdin) != NULL)
        printf("%s", buffer);

	return 0;
}