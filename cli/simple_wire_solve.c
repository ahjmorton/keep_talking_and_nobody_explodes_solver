#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "lib_simple_wire_solve.h"

int main(int argc, char **argv) 
{ 
   if(argc > 3 || argc < 2) {
     return 1;
   }

   char * wire_str = argv[1];

   uint8_t wire_length = strnlen(wire_str, 6);

   if(wire_length < 3 || wire_length > 6) {
     return 2;
   }
   
   enum simple_wire_colours wires[wire_length];
   for(uint8_t i  = 0; i < wire_length; i++) {
     switch(tolower(wire_str[i])) {
        case 'r':
          wires[i] = C_Red;
          break;
        case 'w':
          wires[i] = C_White;
          break;
        case 'y':
          wires[i] = C_Yellow;
          break;
        case 'b':
          wires[i] = C_Blue;
          break;
        case 'l':
          wires[i] = C_Black;
          break;
        default :
          return 3;
     }
   }
   
   enum simple_wire_error_code error;
   uint8_t result;
   if(argc == 2) {
     error = simple_wire_solve_no_serial(
       wires,
       wire_length,
       &result
     );
   } else {
      char * serial_number_str = argv[2];
      if(strnlen(serial_number_str, 1) != 1) {
        return 4;
      }
      char serial_number_digit = serial_number_str[0];
      if(!isdigit(serial_number_digit)) {
        return 5;
      }
      error = simple_wire_solve(
        wires,
        wire_length,
        serial_number_digit - '0',
        &result
      );
   }
   if(error != E_None) {
     return 10 + error;
   } else {
     printf("%i\n", result);
   }

   return 0;
}
