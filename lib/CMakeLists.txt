ADD_LIBRARY(lib_char_map STATIC char_map.c)
ADD_LIBRARY(lib_password_solve STATIC password_solve.c)
ADD_LIBRARY(lib_morse_code_solve STATIC morse_code_solve.c)
ADD_LIBRARY(lib_maze_solve STATIC maze_solve.c)
ADD_LIBRARY(lib_who_first_solve STATIC who_first_solve.c)
ADD_LIBRARY(lib_simple_wire_solve STATIC simple_wire_solve.c)

TARGET_LINK_LIBRARIES(lib_password_solve PRIVATE lib_char_map)
TARGET_LINK_LIBRARIES(lib_morse_code_solve PRIVATE lib_char_map)
