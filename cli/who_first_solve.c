#include <stdio.h>

#include "lib_who_first_solve.h"

int main(int argc, char** argv) {
 char * display_word = "okay";
 char * button_words[6] = {
   "no", "uhhh", "blank", "blank", "wait", "middle"
 };
 who_first_result * result = solve_who_first(
    display_word,
    button_words,
    6
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
