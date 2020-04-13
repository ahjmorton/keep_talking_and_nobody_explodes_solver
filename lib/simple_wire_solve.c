#include "lib_simple_wire_solve.h"

#define NO_SERIAL_NUMBER_DIGIT 10

#define is_odd(Digit) ((Digit & 1) == 1)

#define set_wire_counts(VarName, Wires, WireCount) \
  uint8_t VarName[SIMPLE_WIRE_COLOUR_COUNT] = {0}; \
  {\
    for(uint8_t i = 0; i < WireCount; i++) {\
      VarName[Wires[i]]++;\
    }\
  }\

static uint8_t solve_3_wire(const enum simple_wire_colours * wires) {
  set_wire_counts(wire_counts, wires, 3);
  if(wire_counts[C_Red] == 0) {
    return 1;
  } else if(wires[2] == C_White) {
    return 2;
  } else if(wire_counts[C_Blue] > 1) {
    if(wires[2] == C_Blue) {
      return 2;
    } else if(wires[1] == C_Blue) {
      return 1;
    } else {
      return 0;
    }
  } else {
    return 2;
  }
}

static uint8_t solve_4_wire(
  const enum simple_wire_colours * wires, 
  const uint8_t last_serial_number_digit) {
  set_wire_counts(wire_counts, wires, 4);
  if(wire_counts[C_Red] > 1 && is_odd(last_serial_number_digit)) {
    if(wires[3] == C_Red) {
      return 3;
    } else if(wires[2] == C_Red) {
      return 2;
    } else if(wires[1] == C_Red) {
      return 1;
    } else {
      return 0;
    }
  } else if(wires[3] == C_Yellow && 
     wire_counts[C_Red] == 0) {
    return 0;
  } else if(wire_counts[C_Blue] == 1) {
    return 0;
  } else if(wire_counts[C_Yellow] > 1) {
    return 3;
  } else {
    return 1;
  }
}

static uint8_t solve_5_wire(
  const enum simple_wire_colours * wires, 
  const uint8_t last_serial_number_digit) {
  set_wire_counts(wire_counts, wires, 5);
  if(wires[4] == C_Black && is_odd(last_serial_number_digit)) {
    return 3;
  } else if(wire_counts[C_Red] == 1 && wire_counts[C_Yellow] > 1) {
    return 0;
  } else if(wire_counts[C_Black] == 0) {
    return 1;
  } else {
    return 0;
  }
}

static uint8_t solve_6_wire(
  const enum simple_wire_colours * wires, 
  const uint8_t last_serial_number_digit) {
  set_wire_counts(wire_counts, wires, 6);
  if(wire_counts[C_Yellow] == 0 && is_odd(last_serial_number_digit)) {
    return 2;
  } else if(wire_counts[C_Yellow] == 1 && wire_counts[C_White] > 1) {
    return 3;
  } else if(wire_counts[C_Red] == 0) {
    return 5;
  } else {
    return 3;
  }
}

static enum simple_wire_error_code do_simple_wire_solve(
  const enum simple_wire_colours * wires,
  const uint8_t wire_count,
  const uint8_t last_serial_number_digit,
  uint8_t * wire_to_cut
) {
  switch(wire_count) {
    case 3:
      (*wire_to_cut) = solve_3_wire(wires);
      break;
    case 4:
      (*wire_to_cut) = solve_4_wire(wires, last_serial_number_digit);
      break;
    case 5:
      (*wire_to_cut) = solve_5_wire(wires, last_serial_number_digit);
      break;
    case 6:
      (*wire_to_cut) = solve_6_wire(wires, last_serial_number_digit);
      break;
  }
  return E_None;
}

enum simple_wire_error_code simple_wire_solve_no_serial(
  const enum simple_wire_colours * wires,
  const uint8_t wire_count,
  uint8_t * wire_to_cut
) {
  if(wire_count < 3) {
    return E_TooFewWires;
  } else if(wire_count > 6) {
    return E_TooManyWires;
  }
  return do_simple_wire_solve(
    wires,
    wire_count,
    NO_SERIAL_NUMBER_DIGIT,
    wire_to_cut
  );
}

enum simple_wire_error_code simple_wire_solve(
  const enum simple_wire_colours * wires,
  const uint8_t wire_count,
  const uint8_t last_serial_number_digit,
  uint8_t * wire_to_cut
) {
  if(wire_count < 3) {
    return E_TooFewWires;
  } else if(wire_count > 6) {
    return E_TooManyWires;
  } else if(last_serial_number_digit > 9) {
    return E_InvalidSerialNumberDigit;
  }
  return do_simple_wire_solve(
    wires, 
    wire_count,
    last_serial_number_digit,
    wire_to_cut
  );
}
