#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define GRN  "\x1B[32m"
#define RESET "\033[0m"
#define MAGENTA "\x1b[35m"
#define RED "\x1b[31m"

static const char run_script[] = "./";
extern char **environ;
char cwd[1024];

/*Split string*/
void split (int arg_count, char **args, char * token) {
  while (token != NULL) {

      args[arg_count++] = token;
      token = strtok (NULL, " ");
      printf("%s\n", args[arg_count-1]);
    }

    for (int i = 0; i < arg_count; ++i) {
      if (strcmp(args[i], "<") == 0 ||
        strcmp(args[i], ">") == 0 ||
        strcmp(args[i], ">>") == 0)
        arg_count = i; 
    }
    printf("%d\n", arg_count);
}

/*Change directory*/
void change_dir (char * str) {
  if (str == NULL)
    printf("No directory entered. Current directory is: %s\n", cwd);
  else if (chdir(str) != 0)
    printf("Directory does not exist. Please enter a valid directory.\n");
}

/*Clear terminal*/
void clear () {
  printf("\033[2J\033[0;0H");
}

/*List contents of specified directory*/
void dir (char *str) {

  DIR *dp;
  struct dirent *ep;

  if (str == NULL)
    printf("Please enter a directory to list the files of.\n");
  else {
    dp = opendir (str);
    if (dp != NULL) {
      while ((ep = readdir (dp)))
        puts (ep->d_name);

      (void) closedir (dp);
    }
    else
      perror ("Couldn't open the directory");
  }
}

/*List all environment variables*/
void envir_vars () {
  int i = 0;
  while (environ[i]) 
    printf("%s\n", environ[i++]);
}

/*Echo command*/
void echo (int arg_count, char **str) {
  for (int i = 1; i < arg_count; ++i)
    printf("%s ", str[i]);
  printf("\n");

}

/*Run shell script*/
void shell(char str[]) {

  if (str == NULL) {
    while (str == NULL) {
      printf("Please enter a valid script or type exit to return to the shell.\n");
      fgets(str, 1024, stdin);
      //Remove newline
      str[strlen(str) - 1] = '\0';
      if (strcmp(str, "exit") == 0)
        return;
    }
    printf("derp\n");
  }

  char script[1024];
  strcpy(script, run_script);
  strcat(script, str);
  system(script);
}

/*Show manual*/
void help () {
  system("more readme");

}

/*Pause terminal*/
//Naming to avoid conflict with stdlib.h library
void shell_pause () {
  for(;;) {
    char c = getchar();
    if (c == '\n')
      break;
  }
}

/*Quit terminal*/
void quit () {
  exit(0);
}

int main (int argc, char * argv[]) {

  // // Get domain - seems to cause crashes when executing certain commands...
  // char domain_name[1024];
  // gethostname(domain_name, 1024);

  while (1) {

    //Get current directory
    getcwd(cwd, sizeof(cwd));

    // //Prints -user- in -directory-, colour coded with escape characters
    // printf("%s%s at %s%s %sin %s%s\n%s$ ", MAGENTA, getenv("USER"), 
    //   RED, domain_name, RESET, GRN, cwd, RESET); 
    printf("%s%s %sin %s%s\n%s$ ", MAGENTA, getenv("USER"), 
      RESET, GRN, cwd, RESET);  

    //Read user input
    char str[1024];
    fgets(str, 1024, stdin);
    //Remove newline
    str[strlen(str) - 1] = '\0';

    //Split string
    int arg_count = 0;
    char **args;
    char * token = strtok(str, " ");

    char **stdout_list;

    while (token != NULL) {
      args[arg_count++] = token;
      token = strtok (NULL, " ");
    }

    for (int i = 0; i < arg_count; ++i) {
      if (strcmp(args[i], "<") == 0 ||
        strcmp(args[i], ">") == 0 ||
        strcmp(args[i], ">>") == 0)
        arg_count = i; 
    }
    // split(arg_count, args, token);

    /**
    ** Commands
    **/
    char command[1024];
    memcpy(command, args[0], 1024);
    //Change directory block
    if (strcmp(command, "cd") == 0)
      change_dir(args[1]);

    //Clear block
    else if (strcmp(command, "clr") == 0)
      clear();

    //Dir block - needs to support redirection
    else if (strcmp(command, "dir") == 0)
      dir(args[1]);

    //Environ block - needs to support redirection
    else if (strcmp(command, "environ") == 0)
      envir_vars();

    //Echo block - needs to support redirection
    else if (strcmp(command, "echo") == 0)
      echo(arg_count, args);

    //Help block - needs to support redirection
    else if (strcmp(command, "help") == 0)
      help();

    //Pause block
    else if (strcmp(command, "pause") == 0)
      shell_pause();

    //Quit block
    else if (strcmp(command, "quit") == 0)
      quit();

    //Shell script block
    else if (strcmp(command, "myshell") == 0)
      shell(args[1]);

    else if (feof(stdin)) {
      printf("Goodbye!\n");
      break;
    }

    else if (ferror(stdin)) {
      printf("Oh noes!\n");
      return 1;
    }

    else {
      printf("Command not found: %s\n", command);
    }
    
    printf("\n");
  }
  return 0;
}
