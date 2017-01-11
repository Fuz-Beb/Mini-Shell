#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"

//Your includes come here

//Prints the contents of members_args to the console
void print_members_args(cmd *c){
    //your implementation comes here
}

//Frees the memory allocated to store member arguments
void free_members_args(cmd *c){
    //your implementation comes here
}

//Prints the contents of members to the console
void print_members(cmd *c){
    //your implementation comes here
}

//Frees the memory allocated to store member information
void free_members(cmd *c){
    //your implementation comes here
}

//Prints the redirection information for member i
void print_redirection(cmd *c, int i){
    //your implementation comes here
}

//Frees the memory allocated to store redirection info
void free_redirection(cmd *c){
    //your implementation comes here
}

//Remplit les champs cmd_args et nb_args_membres
void parse_members_args(cmd *c){

	// Variables
	unsigned int i = 0, j = 0, z = 0, size_args = 0, nbr_args = 0;
	char * command = NULL;
	unsigned int nb_cmd_members = c->nb_cmd_members;

	// Allocation du tableau à deux dimensions
	c->cmd_members_args = (char***) malloc(sizeof(char **) * c->nb_cmd_members);

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
	while(nb_cmd_members > 0)
	{
		command = strdup(c->cmd_members[i]);
		z = 0;

		// Recherche le nombre de caractère du premier mot de la chaine 
		while (command[j] != ' ' || command[j] != '\0')
			j++;

		size_args = j;
		c->cmd_members_args[i][z] = subString(command+size_args, command+j);
		z++;
		nbr_args++;

		// Recherche des autres arguments de la chaine
		while (command[j] != '\0')
		{
			while (command[j] != ' ')
				j++;

			// Test si c'est un argument, une redirection, ...
			if (command[j+1] != '>' || command[j+1] != '<')
			{
				j++;
				size_args = j;
				while (command[j] != ' ')
					j++;

				c->cmd_members_args[i][z] = subString(command+size_args, command+j);
				z++;
				nbr_args++;
			}
			else
			{
				c->cmd_members_args[i][z] = strdup("NULL");
				break;
			}
		}

		c->nb_members_args[i] = nbr_args;

		i++;
		nb_cmd_members--;
		free(command);
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
		while(c->init_cmd[pointFin  +1] != '\0')
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

				pointDeb = pointFin + 3;
				pointFin = pointDeb;

		}

		c->nb_cmd_members = nbrMembre;

}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c){
    //your implementation comes here
}

// Permet de retourner une partie d'une chaine
char * subString(char * start, char * end)
{
    size_t count = 0;
    size_t diffLengthStartEnd = (size_t)end-(size_t)start;
    char * cpyString = (char*)malloc(diffLengthStartEnd + 1);

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
