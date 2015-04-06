#include "commands.h"

#define GRN  "\x1B[32m"
#define RESET "\033[0m"
#define MAGENTA "\x1b[35m"
#define RED "\x1b[31m"

extern char **environ;
char cwd[1024];

/*Change directory*/
void change_dir (char * str) {
	
	if (str == NULL)
		printf("No directory entered. Current directory is: %s\n", cwd);
	else if (chdir(str) != 0)
		printf("Directory does not exist. Please enter a valid directory.\n");
	else {
		//Change PWD environment variable
		getcwd(cwd, sizeof(cwd));
		setenv("PWD", cwd, 1);
	}
}

/*Clear terminal*/
void clear () {
	printf("\033[2J\033[0;0H");
}

/*List contents of specified directory*/
void dir (char *str)
{

	DIR *dp;
	struct dirent *ep;

	if (str == NULL)
		printf("Please enter a directory to list the files of.\n");
	else {
		dp = opendir (str);
		if (dp != NULL) {
			while ((ep = readdir (dp)))
				puts (ep->d_name);

			(void) closedir (dp);
		}
		else
			perror ("Couldn't open the directory");
	}
}

/*List all environment variables*/
void envir_vars ()
{
	int i = 0;
	while (environ[i]) 
		printf("%s\n", environ[i++]);
}

/*Echo command*/
void echo (int arg_count, char **str)
{
	for (int i = 1; i < arg_count; ++i)
		printf("%s ", str[i]);
	printf("\n");
}

/*Run shell script*/
void shell(char **args)
{
	if (args[1] == NULL)
	{
		char * str;
		str = calloc(1024, 1);
		while (*str == '\n' || *str == '\0')
		{
			printf("Please enter a valid script or type 'exit' to return to the shell.\n");
			fgets(str, 1024, stdin);

			if (strcmp(str, "exit") == 0)
			{
				free(str);
				return;
			}
		}
		str[strlen(str)-1] = '\0';
		execlp("myshell", "myshell", str, NULL);
		strcpy(args[1], str);
		free(str);
	}
	execlp("./myshell", "./myshell", args[1], NULL);
}

/*Show manual*/
void help ()
{
	int pid = fork();
	if (pid > 0)
	{
		execlp("more", "more", "readme");
	}
}

/*Pause terminal*/
//Naming to avoid conflict with stdlib.h library
void shell_pause ()
{
	for(;;) {
		char c = getchar();
		if (c == '\n')
			break;
	}
}

/*Quit terminal*/
void quit ()
{
	exit(0);
}

void parse_input (int * arg_count, char *** args, char *str)
{
	char str_copy[1024];
	strcpy(str_copy, str);
	char * token_count = strtok(str_copy, " ");
	while (token_count != NULL) {
		(*arg_count)++;
		token_count = strtok (NULL, " ");
	}

	//Allocate memory for holding args
	(*args) = calloc(*arg_count, sizeof(char**));
	for (int i = 0; i < *arg_count; ++i)
		(*args)[i] = calloc(1024, sizeof(char*));
	(*arg_count) = 0;
}

void store_args (int * arg_count, char * str, char *** args)
{
	// Store arguments in array of strings
	char * token = strtok(str, " ");
	while (token != NULL) {
		strcpy((*args)[(*arg_count)++], token);
		token = strtok (NULL, " ");
	}

	    	//Count arguments up to redirection
	for (int i = 0; i < *arg_count; ++i) {
		if (strcmp((*args)[i], "<") == 0 || strcmp((*args)[i], ">") == 0 || strcmp((*args)[i], ">>") == 0)
			*arg_count = i; 
	}
}

void execute_commands (int arg_count, char ** args)
{
	char command[1024];
	memcpy(command, args[0], 1024);

	if (strcmp(command, "cd") == 0)
		change_dir(args[1]);

    	//Clear block
	else if (strcmp(command, "clr") == 0)
		clear();

    	//Dir block - needs to support redirection
	else if (strcmp(command, "dir") == 0)
		dir(args[1]);

    	//Environ block - needs to support redirection
	else if (strcmp(command, "environ") == 0)
		envir_vars();

    	//Echo block - needs to support redirection
	else if (strcmp(command, "echo") == 0)
		echo(arg_count, args);

    	//Help block - needs to support redirection
	else if (strcmp(command, "help") == 0)
		help();

    	//Pause block
	else if (strcmp(command, "pause") == 0)
		shell_pause();

    	//Quit block
	else if (strcmp(command, "quit") == 0)
		quit();

    	//Shell script block
	else if (strcmp(command, "myshell") == 0)
		shell(args);

	else if (feof(stdin)) {
		printf("Goodbye!\n");
		exit(0);
	}

	else if (ferror(stdin)) {
		printf("Oh noes!\n");
		exit(0);
	}

	else {
		printf("Command not found: %s\n", command);
	}

	// printf("\n");
}

void print_prompt_line ()
{
	//Get current directory - stored in global variable for use in dir
	getcwd(cwd, sizeof(cwd));

    //Prints -user- in -directory-, colour coded with escape characters
	char domain_name[1024];
	gethostname(domain_name, 1024);
	printf("%s%s %sat %s%s %sin %s%s\n%s> ", MAGENTA, getenv("USER"), 
    	  RESET, RED, domain_name, RESET, GRN, cwd, RESET); // Also has domain host name
}

void free_args(int *arg_count, char ***args)
{
	for (int i = 0; i < *arg_count; ++i)
		free((*args)[i]);
	free(*args);
}
