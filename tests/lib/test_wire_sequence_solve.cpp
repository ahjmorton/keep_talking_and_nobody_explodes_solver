#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "gtest/gtest.h"

extern "C" {
  #include "lib_wire_sequence_solve.h"
}

TEST(wire_sequence_solve_test, all_combinations) {
   enum wire_sequence_colour previous_colours[9 * 3];
   wire_sequence_connection connections[3];
   char status_string[200];
   for(uint8_t red_count = 0 ; red_count <= 9; red_count++) {
     for(uint8_t blue_count = 0 ; blue_count <= 9; blue_count++) {
       for(uint8_t black_count = 0 ; black_count <= 9; black_count++) {
          uint8_t red_current = red_count % 3;
          uint8_t black_current = black_count % 3;
          uint8_t blue_current = blue_count % 3;
          uint8_t current_wire_count = 
            red_current + black_current + blue_current;
          if(current_wire_count > 3) {
            continue;
          }
          uint8_t i = 0;
          for(uint8_t ri = 0; ri < red_current; ri++) {
            connections[i++].colour = C_Red;
          }
          for(uint8_t li = 0; li < black_current; li++) {
            connections[i++].colour = C_Black;
          }
          for(uint8_t bi = 0; bi < blue_current; bi++) {
            connections[i++].colour = C_Blue;
          }

          uint8_t red_priors = red_count - red_current;
          uint8_t black_priors = black_count - black_current;
          uint8_t blue_priors = blue_count - blue_current;
          uint8_t previous_wire_count = red_priors + black_priors + blue_priors;
          sprintf(status_string, "Should not produce an error for: PWC:%i,CWC:%i,R:%i,B:%i,L:%i,RP:%i,BP:%i,LP:%i,RC:%i,BC:%i,LC:%i\n",
              previous_wire_count,
              current_wire_count,
              red_count,
              blue_count,
              black_count,
              red_priors,
              blue_priors,
              black_priors,
              red_current,
              blue_current,
              black_current
             );
          i = 0;
          for(uint8_t ri = 0; ri < red_priors; ri++) {
             previous_colours[i++] = C_Red;
          }
          for(uint8_t bi = 0; bi < black_priors; bi++) {
             previous_colours[i++] = C_Black;
          }
          for(uint8_t li = 0; li < blue_priors; li++) {
             previous_colours[i++] = C_Blue;
          }
          
          for(uint8_t port_offset = 0; 
              port_offset < current_wire_count; 
              port_offset++) {
              for(i = 0; i < current_wire_count; i++) {
                connections[i].port = 
                  wire_sequence_port_values[
                   ( port_offset + i) % WIRE_SEQUENCE_PORT_COUNT
                  ];
              }
              wire_sequence_result * result = wire_sequence_solve(
                  previous_colours,
                  previous_wire_count,
                  connections,
                  current_wire_count
              );
              const enum wire_sequence_error_code error = 
                  wire_sequence_error(result);
              free_wire_sequence_result(result);
              ASSERT_EQ(error, E_None) << status_string;
          }
       }
     }
   }
}
