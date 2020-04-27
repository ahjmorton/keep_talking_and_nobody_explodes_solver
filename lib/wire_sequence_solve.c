#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lib_wire_sequence_solve.h"

#define a_port 0b1
#define b_port 0b10
#define c_port 0b100
#define a_or_b (a_port | b_port)
#define a_or_c (a_port | c_port)
#define b_or_c (b_port | c_port)
#define a_b_or_c (a_port | b_port | c_port)
#define is_a(Port) ((Port & a_port) == a_port)
#define is_b(Port) ((Port & b_port) == b_port)
#define is_c(Port) ((Port & c_port) == c_port)

#define RED_I 0
#define BLUE_I 1
#define BLACK_I 2
#define A_I 0
#define B_I 1
#define C_I 2

#define MAX_OCCURANCES 9
#define MAX_CURRENT_WIRES 3

static const uint8_t OCCURANCE_MAP[WIRE_SEQUENCE_COLOUR_COUNT][MAX_OCCURANCES] = {
  {c_port, b_port, a_port, a_or_c, b_port, a_or_c, a_b_or_c, a_or_b, b_port},
  {b_port, a_or_c, b_port, a_port, b_port, b_or_c, c_port, a_or_c, a_port},
  {a_b_or_c, a_or_c, b_port, a_or_c, b_port, b_or_c, a_or_b, c_port, c_port}
};

struct wire_sequence_result {
   enum wire_sequence_error_code error;
   wire_sequence_connection * wires_to_cut;
   uint8_t wires_to_cut_count;
};

const enum wire_sequence_error_code wire_sequence_error(const wire_sequence_result * const result) {
  return result->error;
}

const wire_sequence_connection * wire_sequences_to_cut(const wire_sequence_result * const result) {
  return result->wires_to_cut;
}

uint8_t wire_sequence_connection_count(const wire_sequence_result * const result) {
  return result->wires_to_cut_count;
}

void free_wire_sequence_result(wire_sequence_result * result) {
  if(wire_sequence_error(result) == E_None) {
    free(result->wires_to_cut);
  }
  free(result);
}

static inline enum wire_sequence_error_code validate_connections(
  const uint8_t * prior_colour_counts,
  const wire_sequence_connection * connections,
  uint8_t connection_count
) {
  uint8_t current_colour_counts[WIRE_SEQUENCE_COLOUR_COUNT] = {0};
  uint8_t attached_port_bit_field = 0;
  for(uint8_t i = 0; i < connection_count; i++) {
    wire_sequence_connection connection = connections[i];
    uint8_t colour_i;   
    switch(connection.colour) {
      case C_Red:
        colour_i = RED_I;
        break;
      case C_Black:
        colour_i = BLACK_I;
        break;
      case C_Blue:
        colour_i = BLUE_I;
        break;
      default :
        return E_Unknown_Current_Wire_Colour;
    }
    current_colour_counts[colour_i]++;
    if(current_colour_counts[colour_i] +
       prior_colour_counts[colour_i] > MAX_OCCURANCES) {
       return E_TooManyOccurancesTotal;
    }

    uint8_t port_bit_map;
    switch(connection.port) {
      case P_A:
        port_bit_map = a_port;
        break;
      case P_B:
        port_bit_map = b_port;
        break;
      case P_C:
        port_bit_map = c_port;
        break;
      default:
        return E_Unknown_Port;
    }
    if((port_bit_map & attached_port_bit_field) == port_bit_map) {
       return E_PortAttachedTwice;
    }
    attached_port_bit_field = attached_port_bit_field | port_bit_map;
  }
  return E_None;
}

wire_sequence_result * wire_sequence_solve(
  const enum wire_sequence_colour * previous_wires,
  uint8_t previous_wire_count,
  const wire_sequence_connection * connections,
  uint8_t connection_count
) {
  wire_sequence_result * result = 
    (wire_sequence_result *)malloc(sizeof(wire_sequence_result));
  result->error = E_None;
  if(connection_count == 0 || connection_count > MAX_CURRENT_WIRES) {
    result->error = E_InvalidNumberOfCurrentConnections;
    return result;
  }

  uint8_t prior_colour_counts[WIRE_SEQUENCE_COLOUR_COUNT] = {0};
  for(uint8_t i = 0; i < previous_wire_count; i++) {
    switch(previous_wires[i]) {
      case C_Red:
        prior_colour_counts[RED_I]++;
        break;
      case C_Black:
        prior_colour_counts[BLACK_I]++;
        break;
      case C_Blue:
        prior_colour_counts[BLUE_I]++;
        break;
      default :
        result->error = E_Unknown_Previous_Wire_Colour;
        return result;
    }
  }

  result->error = validate_connections(
     prior_colour_counts,
     connections,
     connection_count
  );
  if(result->error != E_None) {
    return result;
  }

  uint8_t wires_to_cut_count = 0;
  wire_sequence_connection * wires_to_cut = (wire_sequence_connection *)malloc(sizeof(wire_sequence_connection) * MAX_CURRENT_WIRES);
  for(uint8_t i = 0; i < connection_count; i++) {
    wire_sequence_connection connection = connections[i];
    uint8_t colour_i;   
    switch(connection.colour) {
      case C_Red:
        colour_i = RED_I;
        break;
      case C_Black:
        colour_i = BLACK_I;
        break;
      case C_Blue:
        colour_i = BLUE_I;
        break;
    }
    uint8_t occurance_count = prior_colour_counts[colour_i]++;
    uint8_t port_mapping = OCCURANCE_MAP[colour_i][occurance_count];

    bool is_connected = 
      (connection.port == P_A && is_a(port_mapping)) ||
      (connection.port == P_B && is_b(port_mapping)) ||
      (connection.port == P_C && is_c(port_mapping));
    
    if(is_connected) {
      memcpy(wires_to_cut + wires_to_cut_count,
             &connection,
             sizeof(wire_sequence_connection));
      wires_to_cut_count++;
    }
  }
  if(wires_to_cut_count < MAX_CURRENT_WIRES) {
     result->wires_to_cut = (wire_sequence_connection *)realloc(wires_to_cut, sizeof(wire_sequence_connection) * wires_to_cut_count);
  } else {
     result->wires_to_cut = wires_to_cut;
  }
  result->wires_to_cut_count = wires_to_cut_count;
  return result;
}
