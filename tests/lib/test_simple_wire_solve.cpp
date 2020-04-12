#include "gtest/gtest.h"

extern "C" {
  #include "lib_simple_wire_solve.h"
}

#define test_all_wire_colours(WireCount) \
TEST(simple_wire_solve_test, simple_wire_solve_##WireCount) {\
  enum simple_wire_colours wires[WireCount] = { C_Red };\
  uint8_t uptick_i; \
  do { \
     uint8_t wire_i = 255;\
     enum simple_wire_error_code error_code = \
       simple_wire_solve(\
         wires,\
         WireCount,\
         1,\
         &wire_i);\
     ASSERT_EQ(E_None, error_code) << "Expected success";\
     ASSERT_LT(wire_i, WireCount) << "Wire i out of bounds";\
\
     wire_i = 255;\
     error_code = \
       simple_wire_solve(\
         wires,\
         WireCount,\
         2,\
         &wire_i);\
     ASSERT_EQ(E_None, error_code) << "Expected success";\
     ASSERT_LT(wire_i, WireCount) << "Wire i out of bounds";\
     uptick_i = 0;\
     do {\
      wires[uptick_i] = simple_wire_colour_values[(wires[uptick_i] + 1) % SIMPLE_WIRE_COLOUR_COUNT];\
      uptick_i = (uptick_i + 1) % WireCount;\
     } while(wires[uptick_i - 1] == simple_wire_colour_values[0] &&\
             uptick_i < WireCount && \
             uptick_i != 0);\
  } \
  while(uptick_i != 0 || (uptick_i == 0 && wires[WireCount - 1] != simple_wire_colour_values[0]));\
}\

test_all_wire_colours(3);
test_all_wire_colours(4);
test_all_wire_colours(5);
test_all_wire_colours(6);
