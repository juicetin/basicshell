#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void change_dir () {

}

void clear () {

}

void dir () {

}

void environ () {


}

void echo () {

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
    printf("%s ~ ", cwd); 

    //Read user input
    char str[100];
    scanf("%s", str);
    
    //Show cwd if only enter pressed
    if (str[0] == "\n")
      continue;

    //Pause block
    if (strcmp(str, "pause") == 0)
      shell_pause();

    //Quit block
    if (strcmp(str, "quit") == 0)
     quit(); 
  }
  return 0;
}
