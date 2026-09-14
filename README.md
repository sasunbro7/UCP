# Dungeon

Dungeon is a C89 terminal game for the COMP1000 Unix and C Programming assignment.

## Build and run

The program is intended for a Linux terminal with GCC and Make.

    make
    ./dungeon map.txt

The executable accepts exactly one map-file argument. The first two integers in the file are the playable row and column counts; the remaining integers describe the cells.

## Controls

- w: climb up while holding a ladder
- s: climb down a ladder
- a: move left
- d: move right
- u: undo the most recent valid move

The player wins by reaching the green goal. Touching a red danger cell or landing after a fall of at least five grids loses the game.

## Tests

Run the scripted command-line and gameplay checks with:

    make test

The tests cover argument and file errors, a full winning route, undo, ignored input, danger-zone loss, and fatal fall distance.

## Source layout

- main.c: command-line entry point
- game.c and game.h: game rules, input, rendering, animation, and status
- map.c and map.h: map-file loading and dynamically allocated 2D map storage
- linkedList.c and linkedList.h: generic linked-list storage used by undo
- terminal.c and terminal.h: immediate single-character terminal input
- color.c and color.h: ANSI foreground and background colours
- newSleep.c and newSleep.h: animation timing
