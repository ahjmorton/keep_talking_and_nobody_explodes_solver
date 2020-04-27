#ifndef SIMPLE_WIRE_SOLVE_HEADER
#define SIMPLE_WIRE_SOLVE_HEADER

#include <stdint.h>

#define SIMPLE_WIRE_COLOUR_COUNT 5

enum simple_wire_colours {
  C_Red = 0,
  C_Yellow = 1,
  C_Blue = 2,
  C_Black = 3,
  C_White = 4
};

static const enum simple_wire_colours simple_wire_colour_values[SIMPLE_WIRE_COLOUR_COUNT] = {
  C_Red,
  C_Yellow,
  C_Blue,
  C_Black,
  C_White
};

enum simple_wire_error_code {
  E_None = 0,
  E_TooManyWires = 1,
  E_TooFewWires = 2,
  E_InvalidSerialNumberDigit = 3
};


enum simple_wire_error_code simple_wire_solve_no_serial(
  const enum simple_wire_colours * wires,
  const uint8_t wire_count,
  uint8_t * wire_to_cut
);

enum simple_wire_error_code simple_wire_solve(
  const enum simple_wire_colours * wires,
  const uint8_t wire_count,
  const uint8_t last_serial_no_digit,
  uint8_t * wire_to_cut
);

#endif
