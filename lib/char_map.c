#include <string.h>
#include <stdlib.h>

#include "lib_char_map.h"

#define CHAR_MAP_LENGTH 26

struct char_map {
  uint8_t * data;
};

char_map * init_char_map(void) {
  char_map* result = (char_map*)malloc(sizeof(char_map));
  result->data = (uint8_t *)calloc(CHAR_MAP_LENGTH, sizeof(uint8_t));
  return result;
}

void clear_char_map(char_map * to_clear) {
   memset(to_clear->data, 0, CHAR_MAP_LENGTH);
}

void pop_char_map(char_map * result, const char * word, uint32_t length) { 
  uint8_t * data = result->data;
  for(uint32_t i = 0; i < length; i++) {
    uint32_t char_index = word[i] - 'a';
    data[char_index]++;
  }
}

uint8_t count_for(const char_map * map, const char index) {
  return map->data[index - 'a'];
}

void free_char_map(char_map * item) {
  free(item->data);
  free(item);
}

