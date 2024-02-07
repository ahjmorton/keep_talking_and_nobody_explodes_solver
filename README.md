# Keep Talking and Nobody Explodes solvers

This repo contains a series of solvers for the amazing [Keep Talking And Nobody Explodes](https://keeptalkinggame.com/) game written in [C](https://en.wikipedia.org/wiki/C_(programming_language)).

## Should I use it while playing the game?

No, go grab a friend. [print out the manual](https://www.bombmanual.com/) and play it first. Even if you have trouble I wouldn't recommend using these solvers. 

If you believe you can create a solver for a module not already handled here then consider raising a PR.

## Does this really solve _every_ module?

At present the repo has solvers for the following modules:

- [maze solving](cli/maze_solve.c)
- [Morse code solving](cli/morse_code_solve.c)
- [password solving](cli/password_solve.c)
- [simple wiring solving](cli/simple_wire_solve.c)
- [who first solving](cli/who_first_solve.c)

In terms of solvers for other modules it comes down to when the original author can be bothered to create them (PRs welcome) and how feasible they are to actually solve.

## How do I use solver X?

_TODO WRITE DOCS_

At this point the code is the documentation unfortunately. Given this code was written for fun / exploration the documentation might never come.

Potentially try reading through the implementation of the [command line](./cli) and figure out what arguments mean what.

# Technical stuff

At present the only dependencies required to build the solvers are [CMake](https://cmake.org/) and a [working C99 compiler (other compilers are available)](https://gcc.gnu.org/).

Which means from the root of the directory we can:

```
cd build
cmake ../
make
```

For tests we also require that [GTest](https://github.com/google/googletest) is available.

Assuming that is installed we can run the (incomplete, PRs welcome) test suite with:

```
cd build 
make test
```

## Project layout

The layout of the project is

```
 |
 |- cli     // Executable commands for the various solvers.
 !          // Tend to include an implementation from lib that
 |          // implements an interface from include.
 |          // Primarily there for arg parsing and output.
 |- include // Header files for the various solvers.
 |- lib     // Implementations of the solvers along with any utlity code.
 |- test    // Some but not all the solvers have tests written.
```

## Why C?

The most flippant answer I can give is that it would be boring to do this in [a higher level programming language](https://en.wikipedia.org/wiki/High-level_programming_language).

The other reason is that I was using this library to test out of concepts from [data orientated design](https://www.youtube.com/watch?v=rX0ItVEVjHc), [mechanical sympathy](https://wa.aws.amazon.com/wellarchitected/2020-07-02T19-33-23/wat.concept.mechanical-sympathy.en.html#:~:text=Mechanical%20sympathy%20is%20when%20you,have%20to%20have%20Mechanical%20Sympathy.) and ultimately [how to best leverage the cache](https://www.youtube.com/watch?v=WDIkqP4JbkE). 
With C we have a higher degree of control about how the program is laid out in memory at runtime.
Not every solver truly explores these concepts, with the only real notable example being the [maze solver](lib/maze_solve.c) where the maze data structures are supposedly designed for cache efficiency. How successful I've been in making that the case is left as a later exercise for myself / one for the reader.
