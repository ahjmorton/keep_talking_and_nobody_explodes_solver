#include <stdio.h>
#include <string.h>

#include "lib_password_solve.h"

int main(int argc, char **argv) 
{
  if(argc != 2) {
     return 1;
  }
  
  char * letters = argv[1];
  const char * password = password_solve(letters, strlen(letters));
  if(password == NULL) {
     return 2;
  } else {
     printf("%s\n", password);
     return 0;
  }
}
