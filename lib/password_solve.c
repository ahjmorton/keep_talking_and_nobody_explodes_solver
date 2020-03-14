#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib_password_solve.h"
#include "lib_char_map.h"

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

const char * password_solve(const char * letters, uint32_t letters_length) {
   char_map * letter_char_map = init_char_map();
   pop_char_map(letter_char_map, letters, letters_length);

   char_map * word_char_map = init_char_map();
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
