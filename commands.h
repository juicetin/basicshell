// Justin Ting, 430203826 - Operating Systems Internals Assignment 1
// Monday 10am-12pm lab - Tutor: Jeshua

#ifndef COMMANDS_H
#define COMMANDS_H

extern char **environ;
extern int n_children;
extern char cwd[1024];

//Internal
void change_dir (char * str); 			//i
void clear ();							//ii
void dir (int arg_count, char **args);	//iii Note - supports infinite directory arguments
void envir_vars ();						//iv
void echo (int arg_count, char **args);	//v
void help (char **args); 				//vi
int shell_pause ();						//vii
void quit ();							//viii

//External
void external_command (int arg_count, char **args);		// Note - prevents zombie child processes using sigaction
void shell(char **args);

#endif // COMMANDS_H
