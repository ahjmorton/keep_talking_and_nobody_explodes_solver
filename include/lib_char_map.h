#ifndef HEADER_CHAR_MAP
#define HEADER_CHAR_MAP

#include <stdint.h>

typedef struct char_map char_map;

char_map * init_char_map(void);

void clear_char_map(char_map * to_clear);

void pop_char_map(char_map * result, const char * word, uint32_t length);

uint8_t count_for(const char_map * map, const char index);

void free_char_map(char_map * item);

#endif
