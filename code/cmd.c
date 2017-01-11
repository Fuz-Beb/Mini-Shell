#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"

//Your includes come here
#define STDIN 0
#define STDOUT 1
#define STDERR 2


//Prints the contents of members_args to the console
void print_members_args(cmd *c){

	unsigned int nb_cmd_members = c->nb_cmd_members;
	int i = 0, j = 0;

	while (nb_cmd_members > 0)
	{
		unsigned int nb_members_args = c->nb_members_args[i];

		// Affichage du nombre d'arguments pour chaque membre
		printf("nb_members_args[%d] = %d\n", i, c->nb_members_args[i]);

		while (nb_members_args >= 0)
		{
			// Affichage de chaque argument
			printf("cmd_members_args[%d][%d] = %s\n",i, j, c->cmd_members_args[i][j]);
			nb_members_args--;
			j++;
		}
		j = 0;
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

		parse_redirection(i, c);
		c->nb_members_args[i] = nbr_args;

		i++;
		nb_cmd_members--;
		free(command);
	}


}

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s,cmd *c){
    int i = 0, numeroArg = 0;
    char buffer[40];

    memset(buffer,'\0',40);

    // Insertion de la commande dans la structure
    c->init_cmd = strdup(s);

    while(c->init_cmd[i] != '\0')
    {
        while(c->init_cmd[i] != '|' && c->init_cmd[i] != '\0')
        {
            buffer[i] = c->init_cmd[i];
            i++;
        }

        if(buffer[i-1] == ' ')
        {
            buffer[i-1] = '\0';
        }

        c->cmd_members[numeroArg] = strdup(buffer);
        numeroArg++;
    }

    c->nb_cmd_members = numeroArg;
}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c){
    
    // Variables
    int j = 0, z = 0;
    char * command = NULL;
    command = strdup(c->cmd_members[i]);

    // Allocation du tableau à deux dimensions
    if (c->redirection == NULL)
    {
    	c->redirection = (char ***) malloc(sizeof(char **) * c->nb_cmd_members);

    	if (c->redirection == NULL)
		{
			printf("Malloc error : c->redirection");
			exit(EXIT_FAILURE);
		}
    }

	if (strstr(command, "<"))
	{
		// Parcours du membre
		while(command[j] != '<')
			j++;

		z = j;

		while (command[z + 2] != ' ')
			z++;

		c->redirection[i][STDIN] = subString(command + (j+2), command + z);
		c->redirection[i][STDOUT] = "NULL";
		c->redirection[i][STDERR] = "NULL";
	}
	else if (strstr(command, ">"))
	{
		// Parcours du membre
		while(command[j] != '>')
			j++;

		z = j;

		while (command[z + 3] != ' ')
			z++;

		c->redirection[i][STDIN] = subString(command + (j+3), command + z);
		c->redirection[i][STDOUT] = "NULL";
		c->redirection[i][STDERR] = "NULL";
	}
	else
	{
		c->redirection[i][STDIN] = "NULL";
		c->redirection[i][STDOUT] = "NULL";
		c->redirection[i][STDERR] = "NULL";
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
