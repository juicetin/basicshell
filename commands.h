// Justin Ting, 430203826 - Operating Systems Internals Assignment 1
// Monday 10am-12pm lab - Tutor: Jeshua

#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <termios.h>

void change_dir (char * str);
void clear ();
void dir (char **args);
void envir_vars ();
void echo (int arg_count, char **args);
void shell(char **args);
void help (char **args);
int shell_pause ();
void quit ();
void parse_input (int * arg_count, char *** args, char *str);
void store_args (int * arg_count, char * str, char *** args);
void execute_commands (int arg_count, char ** args);
void print_prompt_line ();
void free_args(int *arg_count, char ***args);

#endif // COMMAND_H
