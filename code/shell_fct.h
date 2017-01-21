#include "cmd.h"
//Your imports come here

//Terminate shell
#define MYSHELL_FCT_EXIT 1

//Execute a command
int exec_command(cmd* my_cmd);

// Traitement pour effectuer les redirections
int redirection(cmd * my_cmd, int ** tube, int i);

// Chien de garde des processus
void alarmHandler();

// Libération de la mémoire mise sur le tas
void destroy(cmd * c, int returnValue);