#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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
    //your implementation comes here
}

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s,cmd *c){
/*
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
*/
}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c){
    //your implementation comes here
}
