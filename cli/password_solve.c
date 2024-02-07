#include <stdio.h>
#include <string.h>

#include "lib_password_solve.h"

int main(int argc, char **argv) 
{
  if(argc == 1) {
     return 1;
  }

  if(argc > 6) {
     return 2;
  }
 
  for(int i = 1; i < argc; i++) {
    if(strlen(argv[i]) != LETTER_SET_SIZE) {
      return 4 + i;
    }
  }

  password_solve_result holder; 

  uint8_t solution_count =  password_solve(argv + 1, argc - 1, &holder);
  if(solution_count == 0) {
     return 3;
  } else {
     for(int i = 0; i < solution_count; i++) {
       printf("%s\n", holder.possible_words[i]);
     }
     return 0;
  }
}
