#include <pwd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "shell_fct.h"

//To complete
int main(int argc, char** argv)
{
	// Variables
	int ret = MYSHELL_CMD_OK;
	char* readlineptr;
	struct passwd* infos;
	char str[1024];
	char hostname[256];
	char workingdirectory[256];
	cmd * command = NULL;

	//..........
	while(ret != MYSHELL_FCT_EXIT)
	{
		//Get your session info
        infos=getpwuid(getuid());
		gethostname(hostname, 256);
		getcwd(workingdirectory, 256);
        //Print it to the console

		sprintf(str, "\n{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);
		readlineptr = readline(str);

		// Permet d'ajouter un historique avec les fleches (ne pas oublier le include en haut)
		add_history(readlineptr);

		// Initialise la structure
		command = init();

		// Ajoute les membres
		parse_members(readlineptr, command);
		print_members(command);

		// Ajoute les arguments des membres
		parse_members_args(command);
		print_members_args(command);

		// Appel de la fonction permettant de créer les pipes, les fork et les execs
		destroy(command, exec_command(command));
		free(readlineptr);
	}
	// return 0
	return 0;
}
