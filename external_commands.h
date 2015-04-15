// Justin Ting, 430203826 - Operating Systems Internals Assignment 1
// Monday 10am-12pm lab - Tutor: Jeshua

#ifndef EXTERNAL_COMMANDS_H
#define EXTERNAL_COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int n_children;

void external_command (int arg_count, char **args);
void shell(char **args);

#endif // EXTERNAL_COMMANDS_H