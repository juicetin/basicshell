// Justin Ting, 430203826 - Operating Systems Internals Assignment 1
// Monday 10am-12pm lab - Tutor: Jeshua

#include "external_commands.h"

void external_command (int arg_count, char **args)
{
	int pid = fork();
	if (pid == 0)
	{
		//Add parent to environment variable in child
		char parent[1024];
		strcpy(parent, getenv("SHELL"));
		setenv("PARENT", parent, 1);

		int stdin_chk = 0, stdout_chk = 0;
		for (int i = 0; i < arg_count; ++i)
		{
			//Take stdin
			if (strcmp(args[i], "<") == 0 && stdin_chk == 0)
			{
				args[i] = NULL;
				freopen(args[i+1], "r", stdin);
				stdin_chk = 1;
			}

			//Truncate file
			else if (strcmp(args[i], ">") == 0 && stdout_chk == 0)
			{
				args[i] = NULL;
				freopen(args[i+1], "w+", stdout);
				stdout_chk = 1;
			}

			//Append to file
			else if (strcmp(args[i], ">>") == 0 && stdout_chk == 0)
			{
				args[i] = NULL;
				freopen(args[i+1], "a+", stdout);
				stdout_chk = 1;
			}
		}
		execvp(args[0], args);

		//Print error if execvp fails
		fprintf(stderr, "myshell: command not found: %s\n", args[0]);
	}
	else
	{
		//Run in background
		if (strcmp(args[arg_count-1], "&") == 0)
		{
			printf("[%d] %d\n", n_children++, pid);
		}
		//Wait for child to finish
		else
		{
			waitpid(pid, NULL, 0);
		}
	}
}

/*Run script using myshell*/
void shell(char **args)
{
	if (args[1] == NULL)
	{
		char * str;
		str = calloc(1024, 1);
		while (*str == '\n' || *str == '\0')
		{
			fprintf(stderr, "Please enter a valid script or type 'exit' to return to the shell.\n");
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