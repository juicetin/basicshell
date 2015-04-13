// Justin Ting, 430203826 - Operating Systems Internals Assignment 1
// Monday 10am-12pm lab - Tutor: Jeshua

#include "commands.h"

int main (int argc, char * argv[]) {
	
	//Prevent infinite execution of the same script,
	//i.e. until "too many directories" are open
	
	while (1) {

		//Print username, domain location, 
		//and current directory with user prompt
		print_prompt_line();

		FILE * input = NULL;
		input = fopen(argv[1], "r");
		int command_check = 0;
		
		if (input == NULL)
		{
			command_check = 1;
			input = stdin;
		}

    	//Read user input and remove newline character
		char str[1024];
		while (fgets(str, 1024, input) != NULL)
		{

			//Do nothing if no command entered
			if (*str == '\n') break;

			//Remove newline character
			str[strlen(str) - 1] = '\0';

    		//Split string and store arguments
			int arg_count = 0;
			char **args;	
			parse_input(&arg_count, &args, str);
			store_args(&arg_count, str, &args);
			execute_commands(arg_count, args);

			//Free allocated memory
			free_args(&arg_count, &args);

			//Prompt user command if only a command
			//(non-file) was entered
			if (command_check == 1)	break;
		}
		printf("\n");
		if (command_check == 0) exit(0);
	}
	return 0;
}