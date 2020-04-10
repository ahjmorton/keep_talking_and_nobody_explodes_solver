#ifndef HEADER_WHO_FIRST_SOLVE
#define HEADER_WHO_FIRST_SOLVE

#include <stdint.h>

enum who_first_error_code {
   E_None = 0,
   E_UnknownDisplayWord = 1,
   E_UnknownButtonWord = 2,
   E_RequireSixWords = 3,
   E_ButtonPressFindError = 4
};

typedef struct who_first_result who_first_result;

void free_who_first_result(who_first_result * result);

enum who_first_error_code who_first_error(const who_first_result * result);

const char * who_first_to_push(const who_first_result * result);

who_first_result * solve_who_first(
  const char * display_word,
  const char ** button_words,
  uint8_t button_words_count
);

#endif
