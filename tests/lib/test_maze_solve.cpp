#include "gtest/gtest.h"

extern "C" {
  #include "lib_maze_solve.h"
}

#define test_all_combos(FirstX, FirstY, SecondX, SecondY) \
TEST(maze_solve_test, maze_##FirstX##_##FirstY##_##SecondX##_##SecondY) {\
  maze_solve_coors first_marker = \
    { .x = FirstX, .y=FirstY };\
  maze_solve_coors second_marker = \
    { .x = SecondX, .y=SecondY }; \
  for(int x1 = 0; x1 < 6; x1++) {\
    for(int y1 = 0; y1 < 6; y1++) {\
      for(int x2 = 0; x2 < 6; x2++) {\
        for(int y2 = 0; y2 < 6; y2++) {\
          maze_solve_coors start = \
            { .x = x1, .y=y1 };\
          maze_solve_coors end = \
            { .x = x2, .y=y2 };\
          maze_solve_result * result = \
              maze_solve(\
                 &first_marker,\
                 &second_marker,\
                 &start,\
                 &end\
              );\
          enum maze_solve_error error = \
              maze_error_code(result);\
          free_maze_solve_result(result);\
          ASSERT_EQ(E_None, error); \
        }\
      }\
    }\
  }\
}\

test_all_combos(0, 1, 5, 2);
test_all_combos(4, 1, 1, 3);
test_all_combos(3, 3, 5, 3);
test_all_combos(0, 0, 0, 3);
test_all_combos(4, 2, 3, 5);
test_all_combos(4, 0, 2, 4);
test_all_combos(1, 0, 1, 5);
test_all_combos(3, 0, 2, 3);
test_all_combos(2, 1, 0, 4);
