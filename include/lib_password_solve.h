#ifndef HEADER_PASSWORD_SOLVE
#define HEADER_PASSWORD_SOLVE

#include <stdint.h>

#define LETTER_SET_SIZE 6

typedef struct {
  const char ** possible_words;
} password_solve_result;

uint8_t password_solve(const char ** letter_sets, uint8_t letters_set_length, password_solve_result * const result_holder);

#endif
