// Justin Ting, 430203826 - Operating Systems Internals Assignment 1
// Monday 10am-12pm lab - Tutor: Jeshua

#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "internal_commands.h"
#include "external_commands.h"

#define GRN  "\x1B[32m"
#define RESET "\033[0m"
#define MAGENTA "\x1b[35m"
#define RED "\x1b[31m"

void parse_input (int * arg_count, char *** args, char *str);
void store_args (int * arg_count, char * str, char *** args);
void execute_commands (int arg_count, char ** args);
void set_shell_path_envvar();
void print_prompt_line ();
void free_args(int *arg_count, char ***args);

#endif // HELPERS_H