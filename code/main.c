#include <pwd.h>
#include <stdio.h>
#include <readline/readline.h>
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

		// Initialise la structure
		command = init();

		// Ajoute les membres
		parse_members(readlineptr, command);
		print_members(command);

		// Ajoute les arguments des membres
		parse_members_args(command);

		// Affichage des membres
		//print_members_args(command);

		// Affichage des paramètres des membres
		/*print_members(command);*/

		// Affichage des redirections
		print_redirection(command, 1);
	}
	// return 0
	return 0;
}
