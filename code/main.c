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

	// Initialise la structure
	command = init();

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

		sleep(2);

		// Ajoute les membres
		parse_members(readlineptr, command);
		/*print_members(command);*/

		// Ajoute les arguments des membres
		parse_members_args(command);
		/*print_members_args(command);*/

<<<<<<< HEAD
		// Affichage des redirections
		/*print_redirection(command, 1);*/

		free_members_args(command);
		free_members(command);
		free_redirection(command);

=======
		// Appel de la fonction permettant de créer les pipes, les fork et les execs
		exec_command(command);
>>>>>>> d787adeef6ff49f1d8821b0ca3c39a5bae5084c9
	}
	// return 0
	return 0;
}
