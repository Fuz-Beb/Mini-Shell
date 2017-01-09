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
	unsigned int i = 0, j = 0, z = 0, size_args = 0;
	char * command = NULL;
	unsigned int nb_cmd_members = c->nb_cmd_members;

	// Allocation du tableau à deux dimensions
	c->cmd_members_args = (char***) malloc(sizeof(char **) * c->nb_cmd_members);

	if (c->cmd_members_args == NULL)
	{
		printf("Malloc error : c->cmd_members_args");
		exit(EXIT_FAILURE);
	}

	// Parcours des membres
	while(nb_cmd_members > 0)
	{
		command = strdup(c->cmd_members[i]);
		z = 0;

		while (command[j] != ' ' || command[j] != '\0')
			j++;

		size_args = j;
		c->cmd_members_args[i][z] = subString(command+size_args, command+j);
		z++;

		while (command[j] != '\0')
		{
			while (command[j] != ' ')
				j++;

			if (command[j+1] != '>' || command[j+1] != '<')
			{
				j++;
				size_args = j;
				while (command[j] != ' ')
					j++;

				c->cmd_members_args[i][z] = subString(command+size_args, command+j);
				z++;
			}
			else
			{
				c->cmd_members_args[i][z] = strdup("NULL");
				break;	
			}
		}

		i++;
		nb_cmd_members--;
		free(command);
	}


}

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s,cmd *c){
    //your implementation comes here
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
