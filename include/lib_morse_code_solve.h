#ifndef HEADER_MORSE_CODE_SOLVE
#define HEADER_MORSE_CODE_SOLVE

#include <stdint.h>

typedef struct {
  const char * word;
  const char * frequency;
} morse_code_solve_result;

uint8_t morse_code_solve(const char * letters, uint32_t letters_length, morse_code_solve_result ** results);

#endif

