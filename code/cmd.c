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

		while (c->cmd_members_args[i][j] != NULL)
		{

			free(c->cmd_members_args[i][j]);
			nb_members_args--;
			j++;
		}

		j = 0;
		i++;
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

	printf("redirection[%d][STDIN] = %s\n", i, c->redirection[i][STDIN]);
	printf("redirection[%d][STDOUT] = %s\n", i, c->redirection[i][STDOUT]);
	printf("redirection[%d][STDERR] = %s\n", i, c->redirection[i][STDERR]);

	if(c->redirection[i][STDOUT] != NULL)
		printf("redirection_type[%d][STDOUT] = %d\n", i, c->redirection_type[i][STDOUT]);
}

//Frees the memory allocated to store redirection info
void free_redirection(cmd *c){

		unsigned int nb_cmd_members = c->nb_cmd_members;
		int i = 0;

		while (nb_cmd_members > 0) {

			unsigned int nb_members_args = c->nb_members_args[i];

			while (c->redirection[i][STDIN] != NULL && c->redirection[i][STDOUT] != NULL && c->redirection[i][STDERR] != NULL) {

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
			if (member[j] != '>' && member[j] != '<' && (member[j] != '2' && member[j+1] != '>'))
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
		parse_redirection(i, c);

		i++;
		j = 0;
		free(member);
	}


}

// Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s,cmd *c){

	// Variables
  int pointFin = 0, pointDeb = 0, nbrMembre = 0;

  // Insertion de la commande dans la structure
  c->init_cmd = strdup(s);

  // Si la commande est vide alors aucun traitement
  if (strlen(s) == 0)
    return ;

	// Allocation du tableau
	c->cmd_members = (char**) malloc(sizeof(char *));

	if (c->cmd_members == NULL)
	{
		printf("Malloc error : c->cmd_members");
		exit(EXIT_FAILURE);
	}

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

		// Je déplace mon curseur pointFin jusqu'à ce qu'il trouve un autre membre en trouvant : " |"
		while(c->init_cmd[pointFin] != '\0' && c->init_cmd[pointFin + 1] != '|')
			pointFin++;

		// Je place la chaine avant " |" dans un tableau membre disponible
		c->cmd_members[nbrMembre - 1] = subString(c->init_cmd + pointDeb, c->init_cmd + pointFin);

		if(c->init_cmd[pointFin + 1] != '|')
				break;

		// On ajoute + 3 pour " | "
		pointFin += 3;
		// On replace le curseur de début sur la fin pour passer au membre suivant
		pointDeb = pointFin;
	}

	c->nb_cmd_members = nbrMembre;
}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c){

    // Variables
    int start = 0, end = 0;
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

	// Allocation du tableau redirection
	c->redirection_type = (int**) malloc(sizeof(int *));

	if (c->redirection == NULL)
	{
		printf("Malloc error : c->redirection");
		exit(EXIT_FAILURE);
	}

	while (size_command > start + 1)
	{
		// On cherche un espace pour vérifier si le caractère suivant est un opérateur de redirection
		while (command[start] != ' ' && command[start] != '2' && command[start] != '\0')
			start++;

		start++;

		// Si c'est un STDIN
		if (command[start] == '<')
		{
			if (command[start+1] == ' ')
			{
				start += 2;
				end = start;

				// Parcours la chaine a droite de <
				while (command[end] != ' ' && command[end] != '\0')
					end++;

				c->redirection[i][STDIN] = subString(command + start, command + end);
				c->redirection[i][STDOUT] = NULL;
				c->redirection[i][STDERR] = NULL;
			}
			else
			{
				printf("La commande est incorrect. Mauvaise redirection");
				exit(EXIT_FAILURE);
			}
		}
		else if (command[start] == '>') // Si c'est un STDOUT
		{
			if (command[start+1] == ' ' || (command[start+1] == '>' && command[start+2] == ' '))
			{
				// Allocation de la deuxième partie du tableau redirection
				c->redirection_type[i] = (int*) malloc(sizeof(int));

				if (c->redirection_type[i] == NULL)
				{
					printf("Malloc error : c->redirection_type[i]");
					exit(EXIT_FAILURE);
				}

				// Test si c'est un double >>
				if (command[start+1] == '>')
				{
					start += 3;
					c->redirection_type[i][STDOUT] = APPEND;
				}
				else
				{
					start += 2;
					c->redirection_type[i][STDOUT] = OVERRIDE;
				}

				end = start;

				// Parcours la chaine a droite de >
				while (command[end] != '\0')
					end++;

				c->redirection[i][STDIN] = NULL;
				c->redirection[i][STDOUT] = subString(command + start, command + end);
				c->redirection[i][STDERR] = NULL;
			}
			else
			{
				printf("La commande est incorrect. Mauvaise redirection");
				exit(EXIT_FAILURE);
			}
		}
		else if (command[start] == '2') // Si c'est  STDERR
		{
			if (command[start+1] == '>'  && (command[start+2] == ' ' || command[start+2] == '>'))
			{
				if (command[start+2] == '>')
					start += 4;
				else
					start += 3;

				end = start;

				// Parcours la chaine a droite de 2>
				while (command[end] != '\0')
					end++;

				c->redirection[i][STDIN] = NULL;
				c->redirection[i][STDOUT] = NULL;
				c->redirection[i][STDERR] = subString(command + start, command + end);
			}
			else
			{
				printf("La commande est incorrect. Mauvaise redirection");
				exit(EXIT_FAILURE);
			}
		}

	}
	/*print_redirection(c, i);*/
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
/*
* EXEMPLE :
* char * str = "abcdef";
* char * s1 = subString(str,str);
* char * s2 = subString(str,str+strlen(str));
* char * s3 = subString(str+1,str+2);
*
* We get:
*    s1: "";
*    s2: "abcdef";
*    s3: "b";
*/
char * subString(char * start, char * end)
{
    size_t count = 0;
    size_t diffLengthStartEnd = (size_t)end - (size_t)start;
    char * cpyString = (char*)malloc(sizeof(char) * (diffLengthStartEnd + 1));

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
