#include <stdio.h>

#include "lib_maze_solve.h"

int main(void) {
   maze_solve_coors first = {
      .x = 0, .y = 1
   };
   maze_solve_coors second = {
      .x = 5, .y = 2
   };
   maze_solve_coors start = {
      .x = 0, .y = 0
   };
   maze_solve_coors end = {
      .x = 5, .y = 0
   };
   maze_solve_result * result = maze_solve(
     &first,
     &second,
     &start,
     &end
   );
 
   enum maze_solve_error error_code = maze_error_code(result);
   printf("error: %i\n", maze_error_code(result));
   uint8_t dir_length = maze_directions_count(result);
   printf("direction_count: %i\n", dir_length);
   const enum maze_solve_direction * directions = 
     maze_directions(result);
   for(uint8_t i = 0; i < dir_length; i++) {
     const enum maze_solve_direction direction = 
       directions[i];
     if(direction == D_Up) {
        printf("Up,");
     } else if(direction == D_Right) {
        printf("Right,");
     } else if(direction == D_Down) {
        printf("Down,");
     } else if(direction == D_Left) {
        printf("Left,");
     }
   }
   printf("\n");

   free_maze_solve_result(result);
}
