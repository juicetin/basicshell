#include <stdio.h>
#include <stdlib.h>

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

void pause () {

}

void quit () {
  exit(0);
}

int main (int argc, char * argv[]) {
  for (;;) {
    char str[100];
    scanf("%s", str);
    
    //Pause block
    if (strcmp(str, "pause") == 0)


    //Quit block
    if (strcmp(str, "quit") == 0)
     quit(); 
  }
  return 0;
}
