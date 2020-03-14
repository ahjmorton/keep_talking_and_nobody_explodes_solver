#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib_password_solve.h"

static const char * WORDS[] = {
"about",
"after",
"again",
"below",
"could",
"every",
"first",
"found",
"great",
"house",
"large",
"learn",
"never",
"other",
"place",
"plant",
"point",
"right",
"small",
"sound",
"spell",
"still",
"study",
"their",
"there",
"these",
"thing",
"think",
"three",
"water",
"where",
"which",
"world",
"would",
"write",
};

static const size_t WORDS_LENGTH = sizeof(WORDS) / sizeof(WORDS[0]);
#define WORD_LENGTH 5

#define CHAR_MAP_LENGTH 26
typedef uint8_t * char_map;
static inline char_map init_char_map(void) {
  return (char_map)calloc(CHAR_MAP_LENGTH, sizeof(uint8_t));
}
static inline void clear_char_map(char_map to_clear) {
   memset(to_clear, 0, CHAR_MAP_LENGTH);
}
static inline void pop_char_map(char_map result, const char * word, uint32_t length) { 
  for(uint32_t i = 0; i < length; i++) {
    uint32_t char_index = word[i] - 'a';
    result[char_index]++;
  }
}
static inline uint8_t count_for(const char_map map, const char index) {
  return map[index - 'a'];
}

static inline void free_char_map(char_map item) {
  free(item);
}

const char * password_solve(const char * letters, uint32_t letters_length) {
   char_map letter_char_map = init_char_map();
   pop_char_map(letter_char_map, letters, letters_length);

   char_map word_char_map = init_char_map();
   const char * result = NULL;
   for(uint32_t i = 0; i < WORDS_LENGTH && result == NULL; i++) {
     clear_char_map(word_char_map);
     const char * word = WORDS[i];
     pop_char_map(word_char_map, word, WORD_LENGTH);
     uint8_t c;
     for(c = 0; c < WORD_LENGTH; c++) {
        const char ch = word[c];
        if(count_for(word_char_map, ch) > count_for(letter_char_map, ch)) {
          break;
        }
     }
     if(c >= WORD_LENGTH) {
       result = word;
     }
   }
   free_char_map(letter_char_map);
   free_char_map(word_char_map);

   return result;
}
