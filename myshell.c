#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

static const char run_script[] = "./";
char cwd[1024];

/*Change directory*/
void change_dir (char * str) {
  if (str == NULL)
    printf("No directory entered. Current directory is: %s\n", cwd);
  else if (chdir(str) != 0)
    printf("Directory does not exist. Please enter a valid directory.\n");
}

/*Clear terminal*/
void clear () {
  printf("\033[2J\033[1;1H");
}

/*List contents of specified directory*/
void dir (char * str) {

  DIR *dp;
  struct dirent *ep;

  if (str == NULL)
    printf("Please enter a directory to list the files of.\n");
  else {
    dp = opendir (str);
    if (dp != NULL) {
      while (ep = readdir (dp))
        puts (ep->d_name);

      (void) closedir (dp);
    }
    else
      perror ("Couldn't open the directory");
  }
}

/*List all environment variables*/
void environ () {

}

/*Echo command*/
void echo (char * str) {
  printf ("%s\n", str);

}

/*Run shell script*/
void shell(char str[]) {
  char script[1024];
  strcpy(script, run_script);
  strcat(script, str);
  system(script);
}

/*Show manual*/
void help () {

}

/*Pause terminal*/
//Naming to avoid conflict with stdlib.h library
void shell_pause () {
  for(;;) {
    char c = getchar();
    c = getchar();
    if (c == '\n')
      break;
  }
}

/*Quit terminal*/
void quit () {
  exit(0);
}

int main (int argc, char * argv[]) {

  for (;;) {

    //Get current directory
    getcwd(cwd, sizeof(cwd));
    printf("%s ~ \n$ ", cwd); 

    //Read user input
    char str[1024];
    fgets(str, 100, stdin);
    //scanf("%s", str);

    //Remove newline
    str[strlen(str) - 1] = '\0';

    //Split string
    char str_copy[1024];
    strcpy(str_copy, str);
    char * arg;
    char command[1024];
    arg = strtok(str_copy, " ");
    strcpy(command, arg);
    arg = strtok(NULL, " "); 

    //Change directory block
    if (strcmp(command, "cd") == 0)
      change_dir(arg);

    //Clear block
    if (strcmp(command, "clr") == 0)
      clear();

    //Dir block
    else if (strcmp(command, "dir") == 0)
      dir(arg);

    //Environ block
    else if (strcmp(command, "environ") == 0);

    //Echo block
    else if (strcmp(command, "echo") == 0)
      echo(arg);

    //Help block
    else if (strcmp(command, "help") == 0);

    //Pause block
    else if (strcmp(command, "pause") == 0)
      shell_pause();

    //Quit block
    else if (strcmp(command, "quit") == 0)
      quit(); 

    //Shell script block
    else if (strcmp(command, "myshell") == 0)
      shell(arg);

    else {
      printf("\n");
      continue;
    }
    printf("\n");
  }
  return 0;
}
