//Command is well-formed
#define MYSHELL_CMD_OK 0

typedef struct command {
    //the command originally inputed by the user
    char *init_cmd;

    //number of members
    unsigned int nb_cmd_members;

    //each position holds a command member
    char **cmd_members;

    //cmd_members_args[i][j] holds the jth argument of the ith member
    char ***cmd_members_args;

    //number of arguments per member
    unsigned int *nb_members_args;

    //the path to the redirection file
    char ***redirection;

    //the redirecction type (append vs. override)
    int **redirection_type;
} cmd;

//Prints the contents of members_args to the console
void print_members_args(cmd *c);

//Frees the memory allocated to store member arguments
void free_members_args(cmd *c);

//Prints the contents of members to the console
void print_members(cmd *c);

//Frees the memory allocated to store member information
void free_members(cmd *c);

//Prints the redirection information for member i
void print_redirection(cmd *c, int i);

//Frees the memory allocated to store redirection info
void free_redirection(cmd *c);

//Remplit les champs cmd_args et nb_args_membres
void parse_members_args(cmd *c);

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s,cmd *c);

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c);
