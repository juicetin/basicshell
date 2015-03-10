#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

void change_dir () {

}

void clear () {
  printf("\033[2J\033[1;1H");

}

void dir (char * str) {
  DIR *dp;
  struct dirent *ep;
  dp = opendir (str);

  if (dp != NULL) {
    while (ep = readdir (dp))
      puts (ep->d_name);

    (void) closedir (dp);
  }
  else
    perror ("Couldn't open the directory");
}

void environ () {
  

}

void echo (char str[]) {
  printf ("%s\n", str);

}

void help () {

}

//Naming to avoid conflict with stdlib.h library
void shell_pause () {
  for(;;) {
    char c = getchar();
    c = getchar();
    if (c == '\n')
      break;
  }

}

void quit () {
  exit(0);
}

int main (int argc, char * argv[]) {
  
  for (;;) {
    
    //Get current directory
    char cwd[1024];
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
    char com[1024];
    arg = strtok(str_copy, " ");
    strcpy(com, arg);
    arg = strtok(NULL, " "); 
    // printf("%s\n", arg);
    
    //Show cwd if only enter pressed
    //if (strcmp(str, "") == 0)
    //  continue;

    //cd block

    //Clear block
    if (strcmp(str, "clr") == 0)
      clear();

    //Dir block
    //else if (strcmp(str, "dir") == 0)
    //  dir();

    //Environ block
    else if (strcmp(str, "environ") == 0);

    //Echo block
    else if (strcmp(com, "echo") == 0)
      echo(arg);

    //Help block
    else if (strcmp(str, "help") == 0);

    //Pause block
    else if (strcmp(str, "pause") == 0)
      shell_pause();

    //Quit block
    else if (strcmp(str, "quit") == 0)
     quit(); 
    
    else {
      printf("\n");
      continue;
    }
    printf("\n");
  }
  return 0;
}
