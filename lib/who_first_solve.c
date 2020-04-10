#include <stdlib.h>
#include <string.h>

#include "lib_who_first_solve.h"

struct display_word_mapping {
  const char * display_word;
  uint8_t button_start_word_i;
};

#define WORD_MAX_LENGTH 8
#define DISPLAY_WORD_COUNT 28
#define START_WORD_COUNT 28
// We include the start word as the first item in the array
#define PUSH_WORD_COUNT 15
#define BUTTON_WORD_COUNT 6
#define ALL_BUTTON_WORDS_COUNT 29

static const struct display_word_mapping DISPLAY_WORD_MAP[DISPLAY_WORD_COUNT] = {
  { "yes", 2 },
  { "first", 1 },
  { "display", 5 },
  { "okay", 1 },
  { "says", 5 },
  { "nothing", 2 },
  { "", 4 },
  { "blank", 3 },
  { "no", 5 },
  { "led", 2 },
  { "lead", 5 },
  { "read", 3 },
  { "red", 3 },
  { "reed", 4 },
  { "leed", 4 },
  { "hold on", 5 },
  { "you", 3 },
  { "you are", 5 },
  { "your", 3 },
  { "you're", 3 },
  { "ur", 0 },
  { "there", 5 },
  { "they're", 4 },
  { "their", 3 },
  { "they are", 2 },
  { "see", 5 },
  { "c", 1 },
  { "cee", 5 }
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

static const char * WORD_MAP[START_WORD_COUNT][PUSH_WORD_COUNT] = {
  {"ready","yes","okay","what","middle","left","press","right","blank","ready","no","first","uhhh","nothing","wait"},
{"first","left","okay","yes","middle","no","right","nothing","uhhh","wait","ready","blank","what","press","first"},
{"no","blank","uhhh","wait","first","what","ready","right","yes","nothing","left","press","okay","no","middle"},
{"blank","wait","right","okay","middle","blank","press","ready","nothing","no","what","left","uhhh","yes","first"},
{"nothing","uhhh","right","okay","middle","yes","blank","no","press","left","what","wait","first","nothing","ready"},
{"yes","okay","right","uhhh","middle","first","what","press","ready","nothing","yes","left","blank","no","wait"},
{"what","uhhh","what","left","nothing","ready","blank","middle","no","okay","first","wait","yes","press","right"},
{"uhhh","ready","nothing","left","what","okay","yes","right","no","press","blank","uhhh","middle","wait","first"},
{"left","right","left","first","no","middle","yes","blank","what","uhhh","wait","press","ready","okay","nothing"},
{"right","yes","nothing","ready","press","no","wait","what","right","middle","left","uhhh","blank","okay","first"},
{"middle","blank","ready","okay","what","nothing","press","no","wait","left","middle","right","first","uhhh","yes"},
{"okay","middle","no","first","yes","uhhh","nothing","wait","okay","left","ready","blank","press","what","right"},
{"wait","uhhh","no","blank","okay","yes","left","first","press","what","wait","nothing","ready","right","middle"},
{"press","right","middle","yes","ready","press","okay","nothing","uhhh","blank","left","first","what","no","wait"},
{"you","sure","you are","your","you're","next","uh huh","ur","hold","what?","you","uh uh","like","done","u"},
{"you are","your","next","like","uh huh","what?","done","uh uh","hold","you","u","you're","sure","ur","you are"},
{"your","uh uh","you are","uh huh","your","next","ur","sure","u","you're","you","what?","hold","like","done"},
{"you're","you","you're","ur","next","uh uh","you are","u","your","what?","uh huh","sure","done","like","hold"},
{"ur","done","u","ur","uh huh","what?","sure","your","hold","you're","like","next","uh uh","you are","you"},
{"u","uh huh","sure","next","what?","you're","ur","uh uh","done","u","you","like","hold","you are","your"},
{"uh huh","uh huh","your","you are","you","done","hold","uh uh","next","sure","like","you're","ur","u","what?"},
{"uh uh","ur","u","you are","you're","next","uh uh","done","you","uh huh","like","your","sure","hold","what?"},
{"what?","you","hold","you're","your","u","done","uh uh","like","you are","uh huh","ur","next","what?","sure"},
{"done","sure","uh huh","next","what?","your","ur","you're","hold","like","you","u","you are","uh uh","done"},
{"next","what?","uh huh","uh uh","your","hold","sure","next","like","done","you are","ur","you're","u","you"},
{"hold","you are","u","done","uh uh","you","ur","sure","what?","you're","next","hold","uh huh","your","like"},
{"sure","you are","done","like","you're","you","hold","uh huh","ur","sure","u","what?","next","your","uh uh"},
{"like","you're","next","u","ur","hold","done","uh uh","what?","uh huh","you","like","sure","you are","your"}

};

struct who_first_result {
  enum who_first_error_code error;
  uint8_t start_word_i;
  uint8_t word_i;
};

void free_who_first_result(who_first_result * result) {
  free(result);
}

enum who_first_error_code who_first_error(const who_first_result * result) {
   return result->error;
}

const char * who_first_to_push(const who_first_result * result) {
   return WORD_MAP[result->start_word_i][result->word_i];
}

static uint8_t button_start_word_i_for(
  const char * display_word,
  enum who_first_error_code * error
) {
  for(uint8_t i = 0; i < DISPLAY_WORD_COUNT; i++) {
    const struct display_word_mapping * mapping = 
       DISPLAY_WORD_MAP + i;
    if(strncasecmp(
         display_word, 
         mapping->display_word,
         WORD_MAX_LENGTH
       ) == 0
    ) {
       return mapping->button_start_word_i;
    }
  }
  (*error) = E_UnknownDisplayWord;
  return 254;
}

static enum who_first_error_code validate_button_words(const char ** button_words, uint8_t button_words_count) {
   for(uint8_t i = 0; i < button_words_count; i++) {
     const char * button_word = button_words[i];
     uint8_t j;
     for(j = 0; j < ALL_BUTTON_WORDS_COUNT; j++) {
       if(strncasecmp(
         button_word, 
         ALL_BUTTON_WORDS[j],
         WORD_MAX_LENGTH
       ) == 0
       ) {
         break;
       }
     }
     if(j == ALL_BUTTON_WORDS_COUNT) {
        return E_UnknownButtonWord;
     }
   }
   return E_None;
}

static enum who_first_error_code do_find_push_word(
  const char * start_word,
  const char ** button_words,
  uint8_t button_word_count,
  uint8_t * start_word_i,
  uint8_t * push_word_i
) {
  uint8_t s_word_i;
  for(s_word_i = 0; s_word_i < START_WORD_COUNT; s_word_i++) {
    const char * start_word_candidate = WORD_MAP[s_word_i][0];
    if(strncasecmp(
         start_word, 
         start_word_candidate,
         WORD_MAX_LENGTH
       ) == 0
    ) {
       break;
    }
  }
  if(s_word_i == START_WORD_COUNT) {
    return E_ButtonPressFindError;
  }
  (*start_word_i) = s_word_i;

  const char ** push_word_line = WORD_MAP[s_word_i];
  uint8_t p_word_i;
  for(p_word_i = 1; p_word_i < PUSH_WORD_COUNT; p_word_i++) {
    const char * candidate_word = push_word_line[p_word_i];
    uint8_t b_word_i;
    for(b_word_i = 0; b_word_i < button_word_count; b_word_i++) {
      if(strncasecmp(
         candidate_word, 
         button_words[b_word_i],
         WORD_MAX_LENGTH
       ) == 0
      ) {
       break;
      } 
    }
    if(b_word_i < button_word_count) {
      break;
    }
  }
  if(p_word_i == PUSH_WORD_COUNT) {
    return E_ButtonPressFindError;
  }
  (*push_word_i) = p_word_i;

  return E_None;
}

who_first_result * solve_who_first(
  const char * display_word,
  const char ** button_words,
  uint8_t button_words_count
) {
   who_first_result * result = (who_first_result *)malloc(sizeof(who_first_result));
   result->error = E_None;
   if(button_words_count != BUTTON_WORD_COUNT) {
     result->error = E_RequireSixWords;
     return result;
   }
   result->error = validate_button_words(button_words, button_words_count);
   if(result->error != E_None) {
     return result;
   }
   uint8_t button_start_word_i = button_start_word_i_for(
      display_word,
      &(result->error)
   );
   if(result->error != E_None) {
     return result;
   }
   result->error = do_find_push_word(
     button_words[button_start_word_i],
     button_words,
     button_words_count,
     &(result->start_word_i),
     &(result->word_i)
   );

   return result;
}

