// Justin Ting, 430203826 - Operating Systems Internals Assignment 1
// Monday 10am-12pm lab - Tutor: Jeshua

#ifndef INTERNAL_COMMANDS_H
#define INTERNAL_COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

extern char **environ;
char cwd[1024];
struct termios saved_attributes;

void change_dir (char * str); 			//i
void clear ();							//ii
void dir (char **args);					//iii
void envir_vars ();						//iv
void echo (int arg_count, char **args);	//v
void help (char **args); 				//vi
int shell_pause ();						//vii
void quit ();							//viii

#endif // INTERNAL_COMMANDS_H
