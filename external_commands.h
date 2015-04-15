#ifndef EXTERNAL_COMMANDS_H
#define EXTERNAL_COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void external_command (int arg_count, char **args);
void shell(char **args);

#endif // EXTERNAL_COMMANDS_H