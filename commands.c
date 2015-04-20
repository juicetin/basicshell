// Justin Ting, 430203826 - Operating Systems Internals Assignment 1
// Monday 10am-12pm lab - Tutor: Jeshua

#include "commands.h"

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
		printf("No directory entered. Current directory is: %s\n", cwd);
	else if (chdir(str) != 0)
		fprintf(stderr, "Directory does not exist. Please enter a valid directory.\n");
	else {
		//Change PWD environment variable
		getcwd(cwd, sizeof(cwd));
		setenv("PWD", cwd, 1);
	}
}

/////////////////////////////
//// ii - Clear terminal ////
/////////////////////////////
void clear () {
	printf("\033[2J\033[0;0H");
}

////////////////////////////////////////////////////
//// iii - List contents of specified directory ////
////////////////////////////////////////////////////

//NEEDS TO SUPPORT TWO DIRECTORIES
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

/////////////////////////////////////////////
//// iv - List all environment variables ////
/////////////////////////////////////////////
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
}

//////////////////////////
//// v - Echo command ////
//////////////////////////
void echo (int arg_count, char **args)
{
	char str[1024] = "";
	int stdin_chk = 0, stdout_chk = 0;
	for (int i = 1; i < arg_count; ++i)
	{
		//Truncating files
		if (strcmp(args[i], ">") == 0 && stdout_chk == 0)
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

//////////////////////////
//// vi - Show manual ////
//////////////////////////
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

//////////////////////////////
//// vii - Pause terminal ////
//////////////////////////////
void reset_input_mode (void) {
	tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void set_input_mode (void) {
	struct termios tattr;

	/* Make sure stdin is a terminal. */
	if (!isatty (STDIN_FILENO)) {
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

int shell_pause () //Broke naming convention to avoid conflict with stdlib.h library
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

		if (strcmp(args[arg_count-1], "&") == 0)
		{
			args[arg_count-1] = NULL;
		}
		execvp(args[0], args);

		//Print error if execvp fails
		fprintf(stderr, "myshell: command not found: %s\n", args[0]);
	}
	else
	{
		//Background execution
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

//////////////////////////
//// Script execution ////
//////////////////////////
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
