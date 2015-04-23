// Justin Ting, 430203826 - Operating Systems Internals Assignment 1
// Monday 10am-12pm lab - Tutor: Jeshua

#include "helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void parse_input (int * arg_count, char *** args, char *str)
{
	char str_copy[1024];
	strcpy(str_copy, str);

	//Count items in command
	char * token_count = strtok(str_copy, " \t");
	while (token_count != NULL)
	{
		(*arg_count)++;
		token_count = strtok (NULL, " \t");
	}

	//Allocate memory for holding args
	(*args) = calloc(*arg_count, sizeof(char**));

	for (int i = 0; i < *arg_count; ++i)
	{
		(*args)[i] = calloc(1024, sizeof(char*));
	}
	(*arg_count) = 0;
}

void store_args (int * arg_count, char * str, char *** args)
{
	// Store arguments in array of strings
	char * token = strtok(str, " \t");
	while (token != NULL)
	{
		strcpy((*args)[(*arg_count)++], token);
		token = strtok (NULL, " \t");
	}
}

void execute_commands (int arg_count, char ** args)
{
	char command[1024];
	memcpy(command, args[0], 1024);
	
	if (strcmp(command, "cd") == 0)
	{
		change_dir(args[1]);
	}

    //Clear block
	else if (strcmp(command, "clr") == 0)
	{
		clear();
	}

    //Dir block - needs to support redirection
	else if (strcmp(command, "dir") == 0)
	{
		dir(arg_count, args);
	}

    //Environ block - needs to support redirection
	else if (strcmp(command, "environ") == 0)
	{
		envir_vars(args);
	}

    //Clear block
	else if (strcmp(command, "clr") == 0)
	{
		clear();
	}

    //Echo block - needs to support redirection
	else if (strcmp(command, "echo") == 0)
	{
		echo(arg_count, args);
	}

    //Help block - needs to support redirection
	else if (strcmp(command, "help") == 0)
	{
		help(args);
	}

    //Pause block
	else if (strcmp(command, "pause") == 0)
	{
		shell_pause();
	}

    //Quit block
	else if (strcmp(command, "quit") == 0)
	{
		quit();
	}

    //Shell script block
	else if (strcmp(command, "myshell") == 0)
	{
		shell(args);
	}

	else if (feof(stdin))
	{
		printf("Goodbye!\n");
		exit(0);
	}

	else if (ferror(stdin))
	{
		fprintf(stderr, "Oh noes!\n");
		exit(0);
	}

	else
	{
		external_command (arg_count, args);
	}
}

void set_shell_path_envvar()
{
	getcwd(cwd, sizeof(cwd));
	char shell_path[1024] = "";
	strcat(shell_path, cwd);
	strcat(shell_path, "/myshell");
	setenv("SHELL", shell_path, 1);
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
	{
		free((*args)[i]);
	}

	free(*args);
}
