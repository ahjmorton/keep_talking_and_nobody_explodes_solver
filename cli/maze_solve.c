#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errno.h>

#include "lib_maze_solve.h"

static inline bool safe_parse_num(char * num, uint8_t * result) {
   char * endptr;

   errno = 0;
   (*result) = strtoumax(num, &endptr, 10);
   strerror(errno);

   if(*endptr != '\0') {
     return false;
   }
   if(errno != 0) {
     return false;
   }
   return true;
}

static inline bool parse_coors(char* arg, maze_solve_coors * coors) {
   char * x = strtok(arg, ",");
   if(x == NULL) {
     return false;
   }
   char * y = strtok(NULL, ",");
   if(y == NULL) {
     return false;
   }
   if(!safe_parse_num(x, &(coors->x))) {
     return false;
   }
   if(!safe_parse_num(y, &(coors->y))) {
     return false;
   }
   return true;
}

int main(int argc, char** argv) {
   if(argc != 5) {
      return 1;
   }
   maze_solve_coors first, second, start, end;
   if(!parse_coors(argv[1], &first)) {
     return 2;
   }
   if(!parse_coors(argv[2], &second)) {
     return 3;
   }
   if(!parse_coors(argv[3], &start)) {
     return 4;
   }
   if(!parse_coors(argv[4], &end)) {
     return 5;
   } 

   maze_solve_result * result = maze_solve(
     &first,
     &second,
     &start,
     &end
   );

   int rc = 0;
   enum maze_solve_error error_code = maze_error_code(result);
   if(error_code != E_None) {
     rc = 10 + error_code;
   } else {
     uint8_t dir_length = maze_directions_count(result);
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
   }

   free_maze_solve_result(result);
   return rc;
}
