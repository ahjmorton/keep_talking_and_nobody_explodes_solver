#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

uint8_t password_solve(const char ** letter_sets, uint8_t letters_set_length, password_solve_result *const result_holder) {

   bool to_include[WORDS_LENGTH];
   memset(to_include, true, WORDS_LENGTH * sizeof(to_include[0]));
   uint8_t include_count = WORDS_LENGTH;
   for(int i = 0 ; i < WORDS_LENGTH; i++) {
     bool all_sets_match = true;
     for(int ic = 0; ic < WORD_LENGTH && ic < letters_set_length && all_sets_match; ic++) {
       char c = WORDS[i][ic];
       const char * letter_set = letter_sets[ic];
       bool letter_set_match_found = false;
       for(int lsic = 0; lsic < LETTER_SET_SIZE && !letter_set_match_found; lsic++) {
         char lsc = letter_set[lsic];
         if(c == lsc) {
           letter_set_match_found = true;
         }
       }
       if(!letter_set_match_found) {
         all_sets_match = false;
         include_count--;
         to_include[i] = false;
       }
     }
   }
   result_holder->possible_words = (const char **)malloc(include_count * sizeof(char *));
   int include_i = 0;
   for(int i = 0; i < WORDS_LENGTH; i++) {
      if(to_include[i]) {
        result_holder->possible_words[include_i++] = WORDS[i];
      }
   }

   return include_count;
}
