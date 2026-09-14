#ifndef MAP_H
#define MAP_H

/* Integer values used by the map-file format and the in-memory char map. */
#define CELL_EMPTY 0
#define CELL_BLOCK 1
#define CELL_LADDER 2
#define CELL_DANGER 3
#define CELL_PLAYER 4
#define CELL_GOAL 5

typedef struct
{
    int row;
    int column;
} Position;

typedef struct
{
    int rows;
    int columns;
    char **cells;
} GameMap;

typedef enum
{
    MAP_LOAD_OK,
    MAP_LOAD_OPEN_ERROR,
    MAP_LOAD_MEMORY_ERROR,
    MAP_LOAD_CONTENT_ERROR
} MapLoadResult;

void mapInitialise(GameMap *map);
MapLoadResult mapLoad(const char *fileName, GameMap *map,
                      Position *playerPosition);
void mapDestroy(GameMap *map);
int mapIsInside(const GameMap *map, int row, int column);
int mapGetCell(const GameMap *map, int row, int column);
int mapCellIsSolid(const GameMap *map, int row, int column);

#endif
