// Justin Ting, 430203826 - Operating Systems Internals Assignment 1
// Monday 10am-12pm lab - Tutor: Jeshua

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
void dir (char **args)
{
	int pid = fork();
	if (pid == 0)
	{
		if (args[2] != NULL)
		{
			char paths[1024];
			FILE *fp = popen("ls -al", "r");

			//Truncate file with output
			if (strcmp(args[2], ">") == 0)
			{
				FILE *output = fopen(args[3], "w+");
				while (fgets(paths, 1024, fp) != NULL)
					fprintf(output, "%s", paths);
				fclose(output);
				// pclose(fp);
			}

			//Append output to file
			else if (strcmp(args[2], ">>") == 0)
			{
				FILE *output = fopen(args[3], "a+");
				while (fgets(paths, 1024, fp) != NULL)
					fprintf(output, "%s", paths);
				fclose(output);
			}
			pclose(fp);
		}
		else
			execlp("ls", "ls", "-al", args[1], NULL);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

/*List all environment variables*/
void envir_vars (char **args)
{
	int i = 0;

	if (args[1] != NULL)
	{
		//Truncate file with env variables
		if (strcmp(args[1], ">") == 0)
		{
			FILE *fp = fopen(args[2], "w+");
			while (environ[i])
				fprintf(fp, "%s\n", environ[i++]);
			fclose(fp);
		}

		//Append env variables to file
		else if (strcmp(args[1], ">>") == 0)
		{
			FILE *fp = fopen(args[2], "a+");
			while (environ[i])
				fprintf(fp, "%s\n", environ[i++]);
			fclose(fp);
		}
	}
	else
	{
		while (environ[i]) 
			printf("%s\n", environ[i++]);
	}

	// printf("%s %s %s\n", args[0], args[1], args[2]);
}

/*Echo command*/
void echo (int arg_count, char **args)
{
	char str[1024] = "";
	int stdin_chk = 0, stdout_chk = 0;
	for (int i = 1; i < arg_count; ++i)
	{
		if (strcmp(args[i], "<") == 0 && stdin_chk == 0)
		{
			args[i] = NULL;
			freopen(args[i+1], "r", stdin);
			stdin_chk = 1;
		}

		//Truncating files
		else if (strcmp(args[i], ">") == 0 && stdout_chk == 0)
		{
			args[i] = NULL;
			FILE *fp = fopen(args[i+1], "w+");
			fprintf(fp, "%s", str);
			fclose (fp);
			stdout_chk = 1;

		}

		//Appending to files
		else if (strcmp(args[i], ">>") == 0 && stdout_chk == 0)
		{
			args[i] = NULL;
			FILE *fp = fopen(args[i+1], "a+");
			fprintf(fp, "%s", str);
			fclose (fp);
			stdout_chk = 1;
		}

		//Append args to string until redirection occurs
		if (stdin_chk == 0 && stdout_chk == 0)
		{
			strcat(str, args[i]);
			strcat(str, " ");
		}
	}

	//Print if there is no redirection
	if (stdin_chk == 0 && stdout_chk == 0)
	{
		for (int i = 1; i < arg_count && args[i] != NULL; ++i)
			printf("%s ", args[i]);

		if (arg_count > 1)
			printf("\n");
	}
	
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
void help (char **args)
{
	int pid = fork();
	if (pid == 0)
	{
		if (args[2] != NULL)
		{
			char paths[1024];
			FILE *fp = popen("more readme", "r");

			//Truncate file with output
			if (strcmp(args[1], ">") == 0)
			{
				FILE *output = fopen(args[2], "w+");
				while (fgets(paths, 1024, fp) != NULL)
					fprintf(output, "%s", paths);
				fclose(output);
			}

			//Append output to file
			else if (strcmp(args[1], ">>") == 0)
			{
				FILE *output = fopen(args[2], "a+");
				while (fgets(paths, 1024, fp) != NULL)
					fprintf(output, "%s", paths);
				fclose(output);
			}
			pclose(fp);
		}
		else
			execlp("more", "more", "readme", NULL);
	}
	else
	{
		waitpid(pid, NULL, 0);
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

void external_command (int arg_count, char **args)
{
	int pid = fork();
	if (pid == 0)
	{
		//TODO - add PARENT=<pathname>/myshell to environment vars (putenv)


		int stdin_chk = 0, stdout_chk = 0;
		for (int i = 0; i < arg_count; ++i)
		{
			if (strcmp(args[i], "<") == 0 && stdin_chk == 0)
			{
				args[i] = NULL;
				freopen(args[i+1], "r", stdin);
				stdin_chk = 1;
			}

			else if (strcmp(args[i], ">") == 0 && stdout_chk == 0)
			{
				args[i] = NULL;
				freopen(args[i+1], "w+", stdout);
				stdout_chk = 1;
			}

			else if (strcmp(args[i], ">>") == 0 && stdout_chk == 0)
			{
				args[i] = NULL;
				freopen(args[i+1], "a+", stdout);
				stdout_chk = 1;
			}
		}
		execvp(args[0], args);
		printf("myshell: command not found: %s\n", args[0]);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

void parse_input (int * arg_count, char *** args, char *str)
{
	char str_copy[1024];
	strcpy(str_copy, str);
	char * token_count = strtok(str_copy, " \t");
	while (token_count != NULL) {
		(*arg_count)++;
		token_count = strtok (NULL, " \t");
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
	char * token = strtok(str, " \t");
	while (token != NULL) {
		strcpy((*args)[(*arg_count)++], token);
		token = strtok (NULL, " \t");
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
		dir(args);

    	//Environ block - needs to support redirection
	else if (strcmp(command, "environ") == 0)
		envir_vars(args);

    	//Echo block - needs to support redirection
	else if (strcmp(command, "echo") == 0)
		echo(arg_count, args);

    	//Help block - needs to support redirection
	else if (strcmp(command, "help") == 0)
		help(args);

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
		external_command (arg_count, args);
	}
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
