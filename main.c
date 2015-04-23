// Justin Ting, 430203826 - Operating Systems Internals Assignment 1
// Monday 10am-12pm lab - Tutor: Jeshua

#include "helpers.h"
#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

static volatile int run = 1;

void intHandler(int does_nothing)
{
	run = 1;
}

int main (int argc, char * argv[])
{
	set_shell_path_envvar();
	signal(SIGINT, intHandler);
	while (run)
	{
		//Prints user, domain, current directory
		print_prompt_line();

		FILE * input = fopen(argv[1], "r");
		
		//Account for file vs. argument
		int command_check = 0;
		if (input == NULL)
		{
			command_check = 1;
			input = stdin;
		}

		//Read command/script
		char str[1024];
		while (fgets(str, 1024, input) != NULL)
		{
			if (*str == '\n') break; //Do nothing on empty input
			str[strlen(str) - 1] = '\0'; //Remove newline

    		//Split string and store arguments
			int arg_count = 0;
			char **args;	
			parse_input(&arg_count, &args, str);
			store_args(&arg_count, str, &args);
			execute_commands(arg_count, args);

			free_args(&arg_count, &args);

			if (command_check == 1)	break; //Escape if not reading script
		}
		printf("\n");
		
		if (command_check == 0)
		{
			exit(0);
		}
	}
	return 0;
}