#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lib_maze_solve.h"

#define MAZE_COUNT 9
#define MAZE_WIDTH 6

typedef uint8_t maze_t[MAZE_WIDTH][MAZE_WIDTH];

#define right 0b1
#define up 0b10
#define left 0b100
#define down 0b1000
#define down_right down | right
#define down_up down | up
#define down_left down | left
#define left_right left | right
#define left_up left | up
#define right_up right | up
#define left_t left | up | down
#define right_t right | up | down
#define down_t right | up | left
#define up_t right | down | left

#define MARKER 0b10000
#define marker(Directions) (Directions | MARKER)
#define is_marker(Cell) ((Cell & MARKER) == MARKER)
#define can_go_left(Cell) ((Cell & left) == left)
#define can_go_up(Cell) ((Cell & up) == up)
#define can_go_right(Cell) ((Cell & right) == right)
#define can_go_down(Cell) ((Cell & down) == down)

static const maze_t MAZES[MAZE_COUNT] = {
  {
    {down_right, left_right, down_left, down_right, left_right, left},
    {marker(down_up), down_right, left_up, right_up, left_right, down_left},
    {down_up, right_up, down_left, down_right, left_right, marker(left_t)},
    {down_up, right, down_t, left_up, right, left_t},
    {right_t, left_right, down_left, down_right, left, down_up},
    {right_up, left, right_up, left_up, right, left_up}
  },
  {
    {right, up_t, left, down_right, up_t, left},
    {down_right, left_up, down_right, left_up, marker(right_up), down_left},
    {down_up, down_right, left_up, down_right, left_right, left_t},
    {right_t, marker(left_up), down_right, left_up, down, down_up},
    {down_up, down, down_up, down_right, left_up, down_up},
    {up, right_up, left_up, right_up, left_right, left_up}
  },
  {
    {down_right, left_right, down_left, down, down_right, down_left},
    {up, down, down_up, right_up, left_up, down_up},
    {down_right, left_t, down_up, down_right, down_left, down_up},
    {down_up, down_up, down_up, marker(down_up), down_up, marker(down_up)},
    {down_up, right_up, left_up, down_up, down_up, down_up},
    {right_up, left_right, left_right, left_up, right_up, left_up}
  },
  {
    {marker(down_right), down_left, right, left_right, left_right, down_left},
    {down_up, down_up, down_right, left_right, left_right, left_t},
    {down_up, right_up, left_up, down_right, left, down_up},
    {marker(down_up), right, left_right, down_t, left_right, left_t},
    {right_t, left_right, left_right, left_right, down_left, down_up},
    {right_up, left_right, left, right, left_up, up}
  },
  {
    {right, left_right, left_right, left_right, up_t, down_left},
    {down_right, left_right, left_right, up_t, left_up, up},
    {right_t, down_left, right, left_up, marker(down_right), down_left},
    {down_up, right_up, left_right, down_left, up, down_up},
    {down_up, down_right, left_right, down_t, left, down_up},
    {up, right_up, left_right, marker(left_right), left_right, left_up}
  },
  {
    {down, down_right, down_left, right, marker(up_t), down_left},
    {down_up, down_up, down_up, down_right, left_up, down_up},
    {right_t, left_up, up, down_up, down_right, left_up},
    {right_up, down_left, down_right, left_t, down_up, down},
    {down_right, left_up, marker(up), down_up, right_up, left_t},
    {right_up, left_right, left_right, left_up, right, left_up}
  },
  {
    {down_right, marker(left_right), left_right, down_left, down_right, down_left},
    {down_up, down_right, left, right_up, left_up, down_up},
    {right_up, left_up, down_right, left, down_right, left_up},
    {down_right, down_left, right_t, left_right, left_up, down},
    {down_up, up, right_up, left_right, down_left, down_up},
    {right_up, marker(left_right), left_right, left_right, down_t, left_up}
  },
  {
    {down, down_right, left_right, marker(down_left), down_right, down_left},
    {right_t, down_t, left, right_up, left_up, down_up},
    {down_up, down_right, left_right, left_right, down_left, down_up},
    {down_up, right_up, marker(down_left), right, down_t, left_up},
    {down_up, down, right_up, left_right, left_right, left},
    {right_up, down_t, left_right, left_right, left_right, left}
  },
  {
    {down, down_right, left_right, left_right, up_t, down_left},
    {down_up, down_up, marker(down_right), left, down_up, down_up},
    {right_t, down_t, left_up, down_right, left_up, down_up},
    {down_up, down, down_right, left_up, right, left_t},
    {marker(down_up), down_up, down_up, down_right, down_left, up},
    {right_up, left_up, right_up, left_up, right_up, left}
  }
};

struct maze_solve_result {
  enum maze_solve_error error;
  uint8_t maze_directions_count;
  enum maze_solve_direction * directions;
};

const enum maze_solve_direction * maze_directions(const maze_solve_result * result) {
  return result->directions;
}

uint8_t maze_directions_count(const maze_solve_result * result) 
{
  return result->maze_directions_count;
}

enum maze_solve_error maze_error_code(const maze_solve_result * result)
{
  return result->error;
}

void free_maze_solve_result(maze_solve_result * result) 
{
  if(maze_error_code(result) == E_None) {
    free(result->directions);
  }
  free(result);
}

typedef uint8_t direction_store_handle;

typedef struct direction_store direction_store;

typedef struct direction_store_node direction_store_node;

struct direction_store_node {
  uint8_t length;
  direction_store_handle previous_node;
  enum maze_solve_direction direction;
};

struct direction_store {
  direction_store_node * nodes;
  uint8_t count;
  uint8_t capacity;
};

static direction_store * init_direction_store(void) 
{
   direction_store * result = (direction_store *)malloc(sizeof(direction_store));
   result->count = 0;
   result->capacity = 36;
   result->nodes = (direction_store_node *)malloc(sizeof(direction_store_node) * result->capacity);
   return result;
}

static void free_direction_store(direction_store * store)
{

   free(store->nodes);
   free(store);
}

static direction_store_handle add_head_direction(
  direction_store * store, 
  enum maze_solve_direction direction
) {
  if((store->count + 1) == store->capacity) {
    store->capacity = store->capacity * 2;
    store->nodes = (direction_store_node *)realloc(store->nodes, sizeof(direction_store_node) * store->capacity);
  }
  direction_store_handle new_handle = store->count;
  direction_store_node * node = store->nodes + new_handle;
  node->length = 1;
  node->previous_node = 255;
  node->direction = direction;
  store->count++;
  return new_handle;
}

static direction_store_handle add_direction(
  direction_store * store, 
  enum maze_solve_direction direction, 
  direction_store_handle previous_node
) {
  if((store->count + 1) == store->capacity) {
    store->capacity = store->capacity * 2;
    store->nodes = (direction_store_node *)realloc(store->nodes, sizeof(direction_store_node) * store->capacity);
  }
  direction_store_handle new_handle = store->count;
  direction_store_node * node = store->nodes + new_handle;
  node->length = store->nodes[previous_node].length + 1;
  node->previous_node = previous_node;
  node->direction = direction;
  store->count++;
  return new_handle;
}

static uint8_t read_off_directions(
  const direction_store * const store,
  direction_store_handle handle,
  enum maze_solve_direction ** result
) {
  direction_store_node * node = (store->nodes) + handle;
  enum maze_solve_direction * directions = 
    (enum maze_solve_direction *)malloc(sizeof(enum maze_solve_direction) * node->length);
  uint8_t result_length = node->length;
  int8_t i = node->length - 1;
  do {
    directions[i] = node->direction;
    node = (store->nodes) + node->previous_node;
    i--;
  } while(i >= 0);
  (*result) = directions;
  return result_length;
}

static enum maze_solve_direction last_direction(
  const direction_store * const store,
  direction_store_handle handle
) {
  return ((store->nodes) + handle)->direction;
}

typedef struct p_q p_q;

typedef struct p_q_node p_q_node;

struct p_q_node {
  uint8_t priority;
  uint8_t x;
  uint8_t y;
  direction_store_handle path_handle;
};

struct p_q {
  struct p_q_node * nodes;
  uint8_t count;
  uint8_t capacity;
};

static p_q * init_p_q(void) 
{
  p_q * result = (p_q *)malloc(sizeof(p_q));
  result->count = 0;
  result->capacity = 36;
  result->nodes = (p_q_node *)malloc(result->capacity * sizeof(p_q_node));
  return result;
}

static void free_p_q(p_q * p_q) 
{
  free(p_q->nodes);
  free(p_q);
}

static uint8_t insertion_index(p_q * p_q, uint8_t priority)
{
  uint8_t count = p_q->count;
  p_q_node * nodes = p_q->nodes;
  uint8_t i;
  for(i = 0; i < count; i++) {
    if(nodes[i].priority >= priority) {
      return i;
    }
  }
  return i;
}

static void add_p_q_entry(
  p_q * p_q,
  uint8_t priority, 
  uint8_t x, 
  uint8_t y,
  direction_store_handle path_so_far
) {
  if(p_q->count + 1 == p_q->capacity) {
     p_q->capacity = p_q->capacity * 2;
     p_q->nodes = (p_q_node *)realloc(p_q->nodes, p_q->capacity * sizeof(p_q_node));
  }
  uint8_t i_index = insertion_index(p_q, priority);
  if(i_index < p_q->count) {
    memmove(
      p_q->nodes + i_index + 1,
      p_q->nodes + i_index,
      (p_q->count - i_index) * sizeof(p_q_node)
    );
  }
  p_q_node * node = p_q->nodes + i_index;
  node->priority = priority;
  node->x = x;
  node->y = y;
  node->path_handle = path_so_far;
  p_q->count++;
}

static bool pop_p_q(p_q * p_q, p_q_node * result)
{
  if(p_q->count == 0) {
    return false;
  }
  memcpy(result, p_q->nodes, sizeof(p_q_node));
  memmove(
    p_q->nodes,
    p_q->nodes + 1,
    (p_q->count - 1) * sizeof(p_q_node)
  );
  p_q->count--;
  return true;
}

static inline uint8_t distance(
  uint8_t x, uint8_t y, const maze_solve_coors * const end
) {
  return 
    abs(
     ((int8_t)x) - ((int8_t)end->x)
    ) 
    + 
    abs(
     ((int8_t)y) - ((int8_t)end->y)
    );
}

static uint8_t do_solve_maze(
  const maze_t * maze,
  const maze_solve_coors * const start_position,
  const maze_solve_coors * const end_position,
  enum maze_solve_direction ** result_directions,
  enum maze_solve_error * error_code
)
{
  if(start_position->x == end_position->x && 
     start_position->y == end_position->y) {
     (*result_directions) = (enum maze_solve_direction *)malloc(0);
     return 0;
  }
  direction_store * store = init_direction_store();
  p_q * queue = init_p_q();
  uint8_t cell = (*maze)[start_position->y][start_position->x];
  if(can_go_up(cell)) {
    add_p_q_entry(queue,
      distance(
        start_position->x, 
        start_position->y - 1,
        end_position
      ),
      start_position->x,
      start_position->y - 1,
      add_head_direction(store, D_Up)
    );
  } 
  if(can_go_right(cell)) {
    add_p_q_entry(queue,
      distance(
        start_position->x + 1, 
        start_position->y,
        end_position
      ),
      start_position->x + 1,
      start_position->y,
      add_head_direction(store, D_Right)
    );
  }
  if(can_go_down(cell)) {
    add_p_q_entry(queue,
      distance(
        start_position->x, 
        start_position->y + 1,
        end_position
      ),
      start_position->x,
      start_position->y + 1,
      add_head_direction(store, D_Down)
    );
  }
  if(can_go_left(cell)) {
    add_p_q_entry(queue,
      distance(
        start_position->x - 1, 
        start_position->y,
        end_position
      ),
      start_position->x - 1,
      start_position->y,
      add_head_direction(store, D_Left)
    );
  }
  
  p_q_node current;
  bool no_errors = false;
  no_errors = pop_p_q(queue, &current);

  while(no_errors && !(
    (current.x == end_position->x) && 
    (current.y == end_position->y))
  ) {
    enum maze_solve_direction last = 
      last_direction(store, current.path_handle);
    cell = (*maze)[current.y][current.x]; 
    if(last != D_Down && can_go_up(cell)) {
      add_p_q_entry(queue,
        distance(
          current.x, 
          current.y - 1,
          end_position
        ),
        current.x,
        current.y - 1,
        add_direction(store, D_Up, current.path_handle)
      );
    } 
    if(last != D_Left && can_go_right(cell)) {
      add_p_q_entry(queue,
        distance(
          current.x + 1, 
          current.y,
          end_position
        ),
        current.x + 1,
        current.y,
        add_direction(store, D_Right, current.path_handle)
      );
    }
    if(last != D_Up && can_go_down(cell)) {
      add_p_q_entry(queue,
        distance(
          current.x, 
          current.y + 1,
          end_position
        ),
        current.x,
        current.y + 1,
        add_direction(store, D_Down, current.path_handle)
      );
    }
    if(last != D_Right && can_go_left(cell)) {
      add_p_q_entry(queue,
        distance(
          current.x - 1, 
          current.y,
          end_position
        ),
        current.x - 1,
        current.y,
        add_direction(store, D_Left, current.path_handle)
      );
    }
    no_errors = pop_p_q(queue, &current);
  }
  uint8_t result_length = 0;
  if(!no_errors) {
    (*error_code) = E_SolveError;
  } else {
    result_length = read_off_directions(
      store, 
      current.path_handle, 
      result_directions
    );
  }
  free_direction_store(store);
  free_p_q(queue);
  return result_length;
}

static const maze_t * determine_maze(
  const maze_solve_coors * const first_marker,
  const maze_solve_coors * const second_marker,
  enum maze_solve_error * error_code) {
  uint8_t i;
  const maze_t * maze;
  for(i = 0; i < MAZE_COUNT; i++) {
    maze = MAZES + i;
    uint8_t first_cell = (*maze)[first_marker->y][first_marker->x];
    uint8_t second_cell = (*maze)[second_marker->y][second_marker->x];
    if(is_marker(first_cell) && is_marker(second_cell)) {
      break;
    }
  }
  if(i >= MAZE_COUNT) {
    (*error_code) = E_UnknownMaze;
  }
  return maze;
}

inline static bool valid_coors(const maze_solve_coors * const coors) {
  return coors->x < MAZE_WIDTH && coors->y < MAZE_WIDTH;
}

static enum maze_solve_error validate_args(
  const maze_solve_coors * const first_marker,
  const maze_solve_coors * const second_marker,
  const maze_solve_coors * const start_position,
  const maze_solve_coors * const end_position
) 
{
  if(!valid_coors(first_marker)) {
    return E_InvalidFirstMarkerCoordinates;
  } else if(!valid_coors(second_marker)) {
    return E_InvalidSecondMarkerCoordinates;
  } else if(!valid_coors(start_position)) {
    return E_InvalidStartCoordinates;
  } else if(!valid_coors(end_position)) {
    return E_InvalidEndCoordinates;
  } else {
    return E_None;
  }
}

maze_solve_result * maze_solve(
  const maze_solve_coors * const first_marker,
  const maze_solve_coors * const second_marker,
  const maze_solve_coors * const start_position,
  const maze_solve_coors * const end_position
) 
{
  maze_solve_result * result = (maze_solve_result *)malloc(sizeof(maze_solve_result));
  result->error = validate_args(
    first_marker,
    second_marker,
    start_position,
    end_position
  );
  if(result->error != E_None) {
    return result;
  }
  const maze_t * maze = determine_maze(first_marker, second_marker, &(result->error));
  if(result->error != E_None) {
    return result;
  }
  
  result->maze_directions_count = 
    do_solve_maze(
      maze, 
      start_position, 
      end_position, 
      &(result->directions),
      &(result->error)
   );
  return result;
}

