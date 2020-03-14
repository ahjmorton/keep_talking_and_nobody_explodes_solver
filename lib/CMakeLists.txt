ADD_LIBRARY(lib_char_map char_map.c)
ADD_LIBRARY(lib_password_solve password_solve.c)

TARGET_LINK_LIBRARIES(lib_password_solve PRIVATE lib_char_map)
