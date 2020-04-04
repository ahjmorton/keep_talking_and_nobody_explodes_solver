#ifndef HEADER_MAZE_SOLVE
#define HEADER_MAZE_SOLVE

#include <stdint.h>

enum maze_solve_error {
   E_None = 0,
   E_UnknownMaze = 1,
   E_InvalidFirstMarkerCoordinates = 2,
   E_InvalidSecondMarkerCoordinates = 3,
   E_InvalidStartCoordinates = 4,
   E_InvalidEndCoordinates = 5,
};

enum maze_solve_direction {
   D_Up, D_Right, D_Down, D_Left
};

typedef struct maze_solve_result maze_solve_result;
typedef struct {
  uint8_t x;
  uint8_t y;
} maze_solve_coors;

const enum maze_solve_direction * maze_directions(const maze_solve_result * result);

uint8_t maze_directions_count(const maze_solve_result * result);

enum maze_solve_error maze_error_code(const maze_solve_result * result);

void free_maze_solve_result(maze_solve_result * result);

maze_solve_result * maze_solve(
  const maze_solve_coors * const first_marker,
  const maze_solve_coors * const second_marker,
  const maze_solve_coors * const start_position,
  const maze_solve_coors * const end_position
);
#endif
