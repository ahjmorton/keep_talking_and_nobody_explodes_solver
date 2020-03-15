#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib_morse_code_solve.h"

int main(int argc, char **argv) 
{
  if(argc != 2) {
     return 1;
  }
  
  char * letters = argv[1];
  morse_code_solve_result * results = NULL;
  int rc = 0;
  uint8_t result_count = morse_code_solve(
    letters, strlen(letters), &results
  );
  if(result_count == 0) {
    rc = 2;
  } else {
    for(uint8_t i = 0; i < result_count; i++) {
      morse_code_solve_result result = results[i];
      printf("%s => %s\n", 
        result.word, result.frequency);
    }
  }

  if(results != NULL) {
    free(results);
  }
  return rc;
}
