#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"

//Your includes come here
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define APPEND 0
#define OVERRIDE 1

//Prints the contents of members_args to the console
void print_members_args(cmd *c){

	unsigned int nb_cmd_members = c->nb_cmd_members;
	int i = 0, j = 0;

	while (nb_cmd_members > 0)
	{
		unsigned int nb_members_args = c->nb_members_args[i];

		// Affichage du nombre d'arguments pour chaque membre
		printf("nb_members_args[%d] = %d\n", i, c->nb_members_args[i]);

		while (nb_members_args > 0)
		{
			// Affichage de chaque argument
			printf("cmd_members_args[%d][%d] = %s\n",i, j, c->cmd_members_args[i][j]);
			nb_members_args--;
			j++;
		}
		j = 0;
		i++;
		nb_cmd_members--;
	}
}

//Frees the memory allocated to store member arguments
void free_members_args(cmd *c){

    unsigned int nb_cmd_members = c->nb_cmd_members;
	int i = 0, j = 0;

	while (nb_cmd_members > 0)
	{
		unsigned int nb_members_args = c->nb_members_args[i];
		while (nb_members_args >= 0)
		{
			free(c->cmd_members_args[i][j]);
			nb_members_args--;
			j++;
		}
		j = 0;
		nb_cmd_members--;
	}
	free(c->cmd_members_args);
	free(c->nb_members_args);
}

//Prints the contents of members to the console
void print_members(cmd *c){

	unsigned int nb_cmd_members = c->nb_cmd_members;
	int i = 0;

	printf("nb_cmd_members = %d\n", c->nb_cmd_members);

	while (nb_cmd_members > 0)
	{
		printf("cmd_members_args[%d]= %s\n",i, c->cmd_members[i]);
		i++;
		nb_cmd_members--;
	}
}

//Frees the memory allocated to store member information
void free_members(cmd *c){

    unsigned int nb_cmd_members = c->nb_cmd_members;
	int i = 0;

	while (nb_cmd_members > 0)
	{
		free(c->cmd_members[i]);
		i++;
		nb_cmd_members--;
	}
	free(c->cmd_members);
}

//Prints the redirection information for member i
void print_redirection(cmd *c, int i){

	if(c->redirection[i][STDIN] != NULL)
	{
		printf("redirection[%d][STDIN] = %s\n", i, c->redirection[i][STDIN]);
		printf("redirection_type[%d][STDIN] = %d\n", i, c->redirection_type[i][STDIN]);
	}

	if(c->redirection[i][STDOUT] != NULL)
	{
		printf("redirection[%d][STDOUT] = %s\n", i, c->redirection[i][STDOUT]);
		printf("redirection_type[%d][STDOUT] = %d\n", i, c->redirection_type[i][STDOUT]);
	}

	if(c->redirection[i][STDERR] != NULL)
	{
		printf("redirection[%d][STDERR] = %s\n", i, c->redirection[i][STDERR]);
		printf("redirection_type[%d][STDERR] = %d\n", i, c->redirection_type[i][STDERR]);
	}

}

//Frees the memory allocated to store redirection info
void free_redirection(cmd *c){

		unsigned int nb_cmd_members = c->nb_cmd_members;
		int i = 0;

		while (nb_cmd_members > 0) {
			
			unsigned int nb_members_args = c->nb_members_args[i];

			while (nb_members_args >= 0) {

				if(c->redirection[i][STDIN] != NULL)
						free(c->redirection[i][STDIN]);

				if(c->redirection[i][STDOUT] != NULL)
						free(c->redirection[i][STDOUT]);

				if(c->redirection[i][STDERR] != NULL)
						free(c->redirection[i][STDERR]);

				nb_members_args--;
				i++;
			}

			nb_cmd_members--;
		}

		free(c->redirection);

}

//Remplit les champs cmd_args et nb_args_membres
void parse_members_args(cmd *c){

	// Variables
	unsigned int i = 0, j = 0, z = 0, size_args = 0;
	size_t size_member = 0;
	char * member = NULL;

	// Allocation du tableau à deux dimensions
	c->cmd_members_args = (char***) malloc(sizeof(char **));

	if (c->cmd_members_args == NULL)
	{
		printf("Malloc error : c->cmd_members_args");
		exit(EXIT_FAILURE);
	}

	// Allocation du tableau de int
	c->nb_members_args = (unsigned int*) malloc (sizeof(int) * c->nb_cmd_members);

	if (c->nb_members_args == NULL)
	{
		printf("Malloc error : c->nb_members_args");
		exit(EXIT_FAILURE);
	}



	// Parcours des membres
	while(c->nb_cmd_members != i)
	{
		// Initialisation
		member = strdup(c->cmd_members[i]);
		size_member = strlen(member);
		z = 0;

		// Recherche le nombre de caractère du premier mot de la chaine
		while (member[j] != ' ' && member[j] != '\0')
			j++;

		c->nb_members_args[i] = 1;
		c->cmd_members_args[i] = (char**)malloc(sizeof(char * ) * c->nb_members_args[i]);

		if (c->cmd_members_args[i] == NULL)
		{
			printf("Malloc error : c->cmd_members_args[%d]", i);
			exit(EXIT_FAILURE);
		}

		// Ajout du premier membre
		c->cmd_members_args[i][z] = subString(member, member+j); 
		z++;

		// Recherche des autres arguments de la chaine
		while (size_member > j)
		{
			// Avance tant qu'il y a des espaces
			while (member[j] != ' ' && member[j] != '\0')
				j++;

			j++;

			// Test si c'est un argument, une redirection, ...
			if (member[j] != '>' && member[j] != '<')
			{
				// Augmente la mémoire de une case du tableau
				c->nb_members_args[i] += 1;
				c->cmd_members_args[i] = realloc(c->cmd_members_args[i], sizeof(char * ) * c->nb_members_args[i]);

				if (c->cmd_members_args[i] == NULL)
				{
					printf("Malloc error : c->cmd_members_args[%d]", i);
					exit(EXIT_FAILURE);
				}
				
				size_args = j;

				// Cherche le prochain espace
				while (member[j] != ' ' && member[j] != '\0')
					j++;

				// Ajoute la chaine avec la bonne taille dans le tableau
				c->cmd_members_args[i][z] = subString(member+size_args, member+j);
				z++;
			} 
			else // Sort de la boucle WHILE si c'est une redirection
				break;
		}

		// Augmente la mémoire du tableau
		c->nb_members_args[i] += 1;
		c->cmd_members_args[i] = realloc(c->cmd_members_args[i], sizeof(char * ) * c->nb_members_args[i]);

		if (c->cmd_members_args[i] == NULL)
		{
			printf("Malloc error : c->cmd_members_args[%d]", i);
			exit(EXIT_FAILURE);
		}

		// Mise à NULL du dernier argument
		c->cmd_members_args[i][z] = NULL;
		c->nb_members_args[i] -= 1;

		// Appel la fonction parse_redirection 
		/*parse_redirection(i, c);*/
		
		i++;
		j = 0;
		free(member);
	}


}

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s,cmd *c){

    int pointFin = 0, pointDeb = 0, nbrMembre = 0;

	// Allocation du tableau
	c->cmd_members = (char**) malloc(sizeof(char *));

	if (c->cmd_members == NULL)
	{
		printf("Malloc error : c->cmd_members");
		exit(EXIT_FAILURE);
	}

    // Insertion de la commande dans la structure
    c->init_cmd = strdup(s);

	// Calcul du nombre de membre
	while(c->init_cmd[pointFin + 1] != '\0')
	{
		nbrMembre++;
		c->cmd_members = realloc(c->cmd_members, sizeof(char *) * nbrMembre);

		if (c->cmd_members == NULL)
		{
			printf("Realloc error : c->cmd_members");
			exit(EXIT_FAILURE);
		}

		// Je verrif si à la fin il y a bien espace et a +1 un pipe
		while(c->init_cmd[pointFin] != '\0' && c->init_cmd[pointFin + 1] != '|')
			pointFin++;

		c->cmd_members[nbrMembre - 1] = subString(c->init_cmd + pointDeb, c->init_cmd + pointFin);

		if(c->init_cmd[pointFin + 1] != '|')
				break;

		pointDeb = pointFin + 3;
		pointFin = pointDeb;
	}

	c->nb_cmd_members = nbrMembre;
}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c){

    // Variables
    int j = 0, z = 0;
    char * command = strdup(c->cmd_members[i]);
    size_t size_command = strlen(command);

    // Allocation du tableau à deux dimensions
    if (c->redirection == NULL)
    {
    	// Allocation du tableau à deux dimensions
		c->redirection = (char***) malloc(sizeof(char **));

		if (c->redirection == NULL)
		{
			printf("Malloc error : c->redirection");
			exit(EXIT_FAILURE);
		}
    }

    // Allocation de la deuxième partie du tableau
	c->redirection[i] = (char**) malloc(sizeof(char *) * 3);

	if (c->redirection == NULL)
	{
		printf("Malloc error : c->redirection");
		exit(EXIT_FAILURE);
	}

	while (size_command > j)
	{
		// On cherche un espace pour vérifier si le caractère suivant est un opérateur de redirection
		while (command[j] != ' ' && command[j] != '2' && command[j] != '\0')
			j++;

		j++;

		// Si c'est un STDIN
		if (command[j] == '<')
		{
			if (command[j+1] == ' ')
			{
				c->redirection[i][STDIN] = subString(???);
				c->redirection[i][STDOUT] = NULL;
				c->redirection[i][STDERR] = NULL;
			}
			else
			{
				printf("La commande est incorrect. Mauvaise redirection");
				exit(EXIT_FAILURE);
			}
		}
		else if (command[j] == '>') // Si c'est un STDOUT
		{
			if (command[j+1] == ' ' || command[j+1] == '>')
			{
				// Traitement OK
			}
			else
			{
				printf("La commande est incorrect. Mauvaise redirection");
				exit(EXIT_FAILURE);	
			}
		}
		else if (command[j] == '2') // Si c'est  STDERR
		{
			if (command[j+1] == '>' || (command[j+1] == '>' && command[j+2] == '>'))
			{
				// Traitement OK
			}
			else
			{
				printf("La commande est incorrect. Mauvaise redirection");
				exit(EXIT_FAILURE);	
			}
		}
	}

	












	if (strstr(command, "<"))
	{
		// Parcours du membre
		while(c->init_cmd[j] != '<')
			j++;

		z = j;

		while (c->init_cmd[z + 2] != ' ')
			z++;

		c->redirection[i][STDIN] = subString(c->init_cmd + (j+2), c->init_cmd + z);
		c->redirection[i][STDOUT] = NULL;
		c->redirection[i][STDERR] = NULL;

	}
	else if (strstr(c->init_cmd, ">"))
	{
		// Parcours du membre
		while(c->init_cmd[j] != '>')
			j++;

		z = j;

		while (c->init_cmd[z + 3] != ' ')
			z++;

		c->redirection[i][STDIN] = subString(c->init_cmd + (j+3), c->init_cmd + z);
		c->redirection[i][STDOUT] = NULL;
		c->redirection[i][STDERR] = NULL;

	}
	else
	{
		c->redirection[i][STDIN] = NULL;
		c->redirection[i][STDOUT] = NULL;
		c->redirection[i][STDERR] = NULL;
	}
}

// Initialise l'ensemble des champs de la structure
cmd * init(){

	cmd * command = (cmd*) malloc (sizeof(cmd));

	if (command == NULL)
	{
		printf("Malloc error with allocation of struc");
		exit(EXIT_FAILURE);
	}

	command->init_cmd = NULL;
	command->nb_cmd_members = 0;
	command->cmd_members = NULL;
	command->cmd_members_args = NULL;
	command->nb_members_args = 0;
	command->redirection = NULL;
	command->redirection_type = 0;

	return command;
}

// Permet de retourner une partie d'une chaine
char * subString(char * start, char * end)
{
    size_t count = 0;
    int diffLengthStartEnd = strlen(start) - strlen(end);
    char * cpyString = (char*)malloc((sizeof(char) * diffLengthStartEnd + 1));
    printf("DIFF : %d\n", diffLengthStartEnd);

    if (cpyString == NULL)
    {
        printf("Malloc error : cpyString");
		exit(EXIT_FAILURE);
	}

    while (count != diffLengthStartEnd)
    {
        cpyString[count] = start[count];
        count++;
    }
    cpyString[count] = '\0';
    return cpyString;
}
