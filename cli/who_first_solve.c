#include <stdio.h>

#include "lib_who_first_solve.h"

int main(int argc, char** argv) {
 if(argc != 8) {
   return 1;
 }
 who_first_result * result = solve_who_first(
    argv[1],
    argv + 2,
    argc - 2
  );
  int rc = 0;
  if(who_first_error(result) != E_None) {
     rc = 10 + who_first_error(result);
  } else {
     printf("%s\n", who_first_to_push(result));
  }

  free_who_first_result(result);

  return rc;
  
}
