// Justin Ting, 430203826 - Operating Systems Internals Assignment 1
// Monday 10am-12pm lab - Tutor: Jeshua

#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

/*
Note that the behaviour of the child process count
here does not emulate that of a full-featured shell.
The jtin2945-shell does not wait on signals to
determine when a child process has completed to
appropriately assign indices to new child processes,
instead keeping a 'dumb' count of all new children
that are open.
*/
int n_children = 1;
char cwd[1024];
struct termios saved_attributes;

/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////
/////////// INTERNAL COMMANDS ///////////
/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////

//////////////////////////////
//// i - Change directory ////
//////////////////////////////
void change_dir (char * str) {
	
	if (str == NULL)
	{
		printf("No directory entered. Current directory is: %s\n", cwd);
	}
	else if (chdir(str) != 0)
	{
		fprintf(stderr, "Directory does not exist. Please enter a valid directory.\n");
	}
	else
	{
		//Change PWD environment variable
		getcwd(cwd, sizeof(cwd));
		setenv("PWD", cwd, 1);
	}
}

/////////////////////////////
//// ii - Clear terminal ////
/////////////////////////////
void clear ()
{
	printf("\033[2J\033[0;0H");
}

////////////////////////////////////////////////////
//// iii - List contents of specified directory ////
////////////////////////////////////////////////////

void dir (int arg_count, char **args)
{
	int pid = fork();
	if (pid == 0)
	{	
		for (int i = 0; i < arg_count; ++i)
		{
			//stdout redirection
			if (strcmp(args[i], ">") == 0)
			{
				args[i] = NULL;
				freopen(args[i+1], "w", stdout);
				arg_count = i;
				break;
			}
			else if (strcmp(args[i], ">>") == 0)
			{
				args[i] = NULL;
				freopen(args[i+1], "a", stdout);
				arg_count = i;
				break;
			}
		}
		args[arg_count] = NULL;

		//Modify args to use ls -al to fit requirements
		char **args_lsal = calloc(arg_count+1, sizeof(char**));
		for (int i = 0; i < arg_count+1; ++i)
		{
			args_lsal[i] = calloc(256, sizeof(char*));
		}
		for (int i = arg_count; i > 1; --i)
		{
			strcpy(args_lsal[i], args[i-1]);
		}
		strcpy(args_lsal[0], "ls");
		strcpy(args_lsal[1], "-al");
		args_lsal[arg_count+1] = NULL;

		execvp(args_lsal[0], args_lsal);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

/////////////////////////////////////////////
//// iv - List all environment variables ////
/////////////////////////////////////////////
void print(char *** args, char * file_mode)
{
	int i = 0;
	FILE *fp = fopen((*args)[2], file_mode);
	while (environ[i])
	{
		fprintf(fp, "%s\n", environ[i++]);
	}
	fclose(fp);
}

void envir_vars (char **args)
{
	if (args[1] != NULL)
	{
		//Truncate file with env variables
		if (strcmp(args[1], ">") == 0)
		{
			print(&args, "w+");
		}

		//Append env variables to file
		else if (strcmp(args[1], ">>") == 0)
		{
			print(&args, "a+");
		}
	}
	else
	{
		int i = 0;
		while (environ[i])
		{
			printf("%s\n", environ[i++]);
		}
	}
}

//////////////////////////
//// v - Echo command ////
//////////////////////////
void redirect_echo(char ***args, char *file_mode, char *str, int *std_chk, int i)
{
	(*args)[i] = NULL;
	FILE *fp = fopen((*args)[i+1], file_mode);
	fprintf(fp, "%s\n ", str);
	fclose(fp);
	(*std_chk) = 1;
}

void echo (int arg_count, char **args)
{
	char str[1024] = "";
	int stdin_chk = 0, stdout_chk = 0;
	for (int i = 1; i < arg_count; ++i)
	{
		//Truncating files
		if (strcmp(args[i], ">") == 0 && stdout_chk == 0)
		{
			redirect_echo(&args, "w+", str, &stdout_chk, i);
		}

		//Appending to files
		else if (strcmp(args[i], ">>") == 0 && stdout_chk == 0)
		{
			redirect_echo(&args, "a+", str, &stdout_chk, i);
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
		{
			printf("%s ", args[i]);
		}

		if (arg_count > 1)
		{
			printf("\n");
		}
	}
}

//////////////////////////
//// vi - Show manual ////
//////////////////////////
void redirect_help(char ***args, char *file_mode, char *paths, FILE *fp)
{
	FILE *output = fopen((*args)[2], file_mode);
	while (fgets(paths, 1024, fp) != NULL)
	{
		fprintf(output, "%s", paths);
	}
	fclose(output);
}

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
				redirect_help(&args, "w+", paths, fp);
			}

			//Append output to file
			else if (strcmp(args[1], ">>") == 0)
			{
				redirect_help(&args, "a+", paths, fp);
			}
			pclose(fp);
		}
		else
		{
			char readme_path[1024];
			strcpy(readme_path, getenv("SHELL"));
			strcat(readme_path, "/readme");
			execlp("more", "more", readme_path, NULL);
		}
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

//////////////////////////////
//// vii - Pause terminal ////
//////////////////////////////

//REFERENCE THIS BLOCK OF CODE (the pause stuff)!
void reset_input_mode (void)
{
	tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void set_input_mode (void)
{
	struct termios tattr;

	/* Make sure stdin is a terminal. */
	if (!isatty (STDIN_FILENO))
	{
		fprintf (stderr, "Not a terminal.\n");
		exit (EXIT_FAILURE);
	}

	/* Save the terminal attributes so we can restore them later. */
	tcgetattr (STDIN_FILENO, &saved_attributes);
	atexit (reset_input_mode);

	/* Set the funny terminal modes. */
	tcgetattr (STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 0;
	tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}

//Broke naming convention to avoid conflict with stdlib.h library
int shell_pause () 
{
	char c;
	FILE * null_file = NULL;

	set_input_mode ();

	null_file = fopen("/dev/null", "w");

	printf("Press enter to continue...\n");
	fflush(stdin);

	while (1)
	{
		read (STDIN_FILENO, &c, 1);
		if (c == '\004') break; /* Catch control-d */
		else if (c == '\n') break; /* Catch Enter */
		else
		{
			fprintf (null_file, "%c", c); /* Discard other input */
			fflush (null_file);
		}
	}
	if (null_file != NULL)
	{
		fclose (null_file);
	}
	reset_input_mode();
	return EXIT_SUCCESS;
}

//////////////////////////////
//// viii - Quit terminal ////
//////////////////////////////
void quit ()
{
	exit(0);
}

/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////
/////////// EXTERNAL COMMANDS ///////////
/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////

///////////////////////////
//// External commands ////
///////////////////////////
void redirect_external (char *** args, FILE ** std, int * std_chk, char * file_mode, int i)
{
	(*args)[i] = NULL;
	freopen((*args)[i+1], file_mode, (*std));
	(*std_chk) = 1;
}

struct sigaction sigchld_action = {
	//pointer to the SIG_DFL macro, default signal handling
	.sa_handler = SIG_DFL,		

	//when sig === SIGCHLD, prevents zombie proc on termination
	.sa_flags = SA_NOCLDWAIT	
};

void external_command (int arg_count, char **args)
{
	int pid = fork();
	if (pid == 0)
	{
		//Set location of parent shell in child environment
		char parent[1024];
		strcpy(parent, getenv("SHELL"));
		setenv("PARENT", parent, 1);

		int stdin_chk = 0, stdout_chk = 0;
		for (int i = 0; i < arg_count; ++i)
		{
			//Take stdin
			if (strcmp(args[i], "<") == 0 && stdin_chk == 0)
			{
				redirect_external(&args, &stdin, &stdin_chk, "r", i);
			}

			//Truncate file
			else if (strcmp(args[i], ">") == 0 && stdout_chk == 0)
			{
				redirect_external(&args, &stdout, &stdout_chk, "w+", i);
			}

			//Append to file
			else if (strcmp(args[i], ">>") == 0 && stdout_chk == 0)
			{
				redirect_external(&args, &stdout, &stdout_chk, "a+", i);
			}
		}

		execvp(args[0], args);

		//Print error if execvp fails
		fprintf(stderr, "myshell: command not found: %s\n", args[0]);
		exit(0);	//Prevent extra myshell process spawning on fail
	}
	else
	{
		if (strcmp(args[arg_count-1], "&") == 0)
		{
			printf("[%d] %d\n", n_children++, pid);

			//prevent zombie procs
			sigaction(SIGCHLD, &sigchld_action, NULL);	
		}
		else 
		{
			waitpid(pid, NULL, 0);
		}
	}
}

//////////////////////////
//// Script execution ////
//////////////////////////

//Note: will not process shell scripts 
//correctly without a terminating newline at EOF
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
