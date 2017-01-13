#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "shell_fct.h"

int exec_command(cmd* my_cmd){
    
    //Variables
    int i = 0, j = 0;
    pid_t pid;
    int status;
    int tube[2];
    char buffer[20];
    
    //Open pipe
    pipe(tube);
    
    pid = fork();
    if(pid == 0)
    {
        //Redirect child process input and ouputs
        close(tube[0]);  //Close std input
        dup2(tube[1], 1);//Dup pipe output to std output
        close(tube[1]);  //Close pipe output
        
        //Execute ls
        if(execvp(my_cmd->cmd_members_args[i][j], my_cmd->cmd_members_args[i]) == -1) {
            perror("execlp");
            exit(errno);
        }
        
    }else{
        
        //Reconfigure parent's input
        close(tube[1]);  //Close pipe output
        dup2(tube[0], 0);//Dup pipe input to std input
        close(tube[0]);  //Close pipe input
        
        //Synchronize parent and child processes
        wait(&status);
        
        //Generate output (si EOF -> retourne NULL)
        while(fgets(buffer, 20, stdin) != NULL)
            printf("%s", buffer);

        close(tube[1]); 
        close(tube[0]);
    }
    return 0;
}