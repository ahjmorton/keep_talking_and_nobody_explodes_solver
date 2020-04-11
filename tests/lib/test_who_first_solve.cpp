#include <stdlib.h>
#include <time.h>

#include "gtest/gtest.h"

extern "C" {
  #include "lib_who_first_solve.h"
}

#define ALL_BUTTON_WORDS_COUNT 28
#define ALL_DISPLAY_WORDS_COUNT 28
#define ITERATIONS 10000

static const char * ALL_DISPLAY_WORDS[ALL_DISPLAY_WORDS_COUNT] = {
"yes",
"first",
"display",
"okay",
"says",
"nothing",
"",
"blank",
"no",
"led",
"lead",
"read",
"red",
"reed",
"leed",
"hold on",
"you",
"you are",
"your",
"you're",
"ur",
"there",
"they're",
"their",
"they are",
"see",
"c",
"cee"
};

static const char * ALL_BUTTON_WORDS[ALL_BUTTON_WORDS_COUNT] = {
"blank",
"done",
"first",
"hold",
"left",
"like",
"middle",
"next",
"no",
"nothing",
"okay",
"press",
"ready",
"right",
"sure",
"u",
"uh huh",
"uh uh",
"uhhh",
"ur",
"wait",
"what",
"what?",
"yes",
"you",
"you are",
"you're",
"your"
};



TEST(who_first_test, test_random_words) {
  srand(time(0));
  for(uint16_t i = 0; i < ITERATIONS; i++) {
    uint8_t display_word_i = 
      rand() * ALL_DISPLAY_WORDS_COUNT;
    const char * display_word = 
      ALL_DISPLAY_WORDS[display_word_i];
    const char * button_words[6];
    for(uint8_t i = 0; i < 6; i++) {
      uint8_t button_word_i = 
        rand() * ALL_BUTTON_WORDS_COUNT;
      button_words[i] = 
        ALL_BUTTON_WORDS[button_word_i];
    }
    who_first_result * result = 
      solve_who_first(
        display_word,
        button_words,
        6
      );
    free_who_first_result(result);
  }
}
