#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lib_morse_code_solve.h"
#include "lib_char_map.h"

#define WORD_COUNT 16

static const char * WORDS[WORD_COUNT][2] = {
  {"shell", "3.505"},
  {"halls", "3.515"},
  {"slick", "3.522"},
  {"trick", "3.532"},
  {"boxes", "3.535"},
  {"leaks", "3.542"},
  {"strobe", "3.545"},
  {"bistro", "3.552"},
  {"flick", "3.555"},
  {"bombs", "3.565"},
  {"break", "3.572"},
  {"brick", "3.575"},
  {"steak", "3.582"},
  {"sting", "3.592"},
  {"vector", "3.595"},
  {"beats", "3.600"}
};

#define word_length(WORD) strnlen(WORD, 7)

uint8_t morse_code_solve(const char * letters, uint32_t letters_length, morse_code_solve_result ** results) {
    uint8_t result = 0;
    char_map * letters_char_map = init_char_map();
    pop_char_map(letters_char_map, letters, letters_length);
    char_map * word_char_map = init_char_map();
    for(int i = 0; i < WORD_COUNT; i++) {
      clear_char_map(word_char_map);
      const char * word = WORDS[i][0];
      const size_t length = word_length(word);
      pop_char_map(word_char_map, word, length);
      uint8_t c;
      for(c = 0; c < letters_length; c++) {
        const char ch = letters[c];
        if(count_for(letters_char_map, ch) > count_for(word_char_map, ch)) {
          break;
        }
      }
      if(c == letters_length) {
         result++;
         (*results) = realloc(*results, result * sizeof(morse_code_solve_result));
         (*results)[result - 1].word = word;
         (*results)[result - 1].frequency = WORDS[i][1];
      }
    }
    free_char_map(letters_char_map);
    free_char_map(word_char_map);
    return result;
}
