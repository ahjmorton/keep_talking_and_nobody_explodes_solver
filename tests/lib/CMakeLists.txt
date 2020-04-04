ADD_EXECUTABLE(maze_solve_test "test_maze_solve.cpp")

TARGET_LINK_LIBRARIES(maze_solve_test lib_maze_solve gtest gtest_main)

ADD_TEST(maze_solve_test maze_solve_test)
