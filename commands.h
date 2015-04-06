#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

void change_dir (char * str);
void clear ();
void dir (char *str);
void envir_vars ();
void echo (int arg_count, char **str);
void shell(char **args);
void help ();
void shell_pause ();
void quit ();
void parse_input (int * arg_count, char *** args, char *str);
void store_args (int * arg_count, char * str, char *** args);
void execute_commands (int arg_count, char ** args);
void print_prompt_line ();
void free_args(int *arg_count, char ***args);

#endif // COMMAND_H
