#ifndef WIRE_SEQUENCE_SOLVE_HEADER
#define WIRE_SEQUENCE_SOLVE_HEADER

#include <stdint.h>

#define WIRE_SEQUENCE_COLOUR_COUNT 3
#define WIRE_SEQUENCE_PORT_COUNT 3

enum wire_sequence_colour {
  C_Red = 0,
  C_Blue = 2,
  C_Black = 3
};

static const enum wire_sequence_colour wire_sequence_colour_values[WIRE_SEQUENCE_COLOUR_COUNT] = {
  C_Red,
  C_Blue,
  C_Black
};

enum wire_sequence_port {
  P_A = 0,
  P_B = 1,
  P_C = 2
};

static const enum wire_sequence_port wire_sequence_port_values[WIRE_SEQUENCE_PORT_COUNT] = {
  P_A,
  P_B,
  P_C
};

typedef struct {
   enum wire_sequence_colour colour;
   enum wire_sequence_port port;
} wire_sequence_connection;


enum wire_sequence_error_code {
  E_None = 0,
  E_InvalidNumberOfCurrentConnections = 1,
  E_TooManyOccurancesTotal = 2,
  E_PortAttachedTwice = 3,
  E_Unknown_Previous_Wire_Colour = 4,
  E_Unknown_Current_Wire_Colour = 5,
  E_Unknown_Port = 6
};

typedef struct wire_sequence_result wire_sequence_result;

const enum wire_sequence_error_code wire_sequence_error(
  const wire_sequence_result * const result
);

const wire_sequence_connection * wire_sequences_to_cut(
  const wire_sequence_result * const result
);

uint8_t wire_sequence_connections_count(
  const wire_sequence_result * const result
);

void free_wire_sequence_result(wire_sequence_result * result);

wire_sequence_result * wire_sequence_solve(
  const enum wire_sequence_colour * previous_wires,
  uint8_t previous_wire_count,
  const wire_sequence_connection * connections,
  uint8_t connections_count
);

#endif
