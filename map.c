#include <stdio.h>
#include <stdlib.h>
#include "map.h"

void mapInitialise(GameMap *map)
{
    if (map != NULL)
    {
        map->rows = 0;
        map->columns = 0;
        map->cells = NULL;
    }
}

/*
 * Read the dimensions first, allocate each map row, and then translate the
 * player marker into a Position. Keeping the player separate prevents a
 * ladder from being erased when the player moves away from it.
 */
MapLoadResult mapLoad(const char *fileName, GameMap *map,
                      Position *playerPosition)
{
    FILE *mapFile;
    MapLoadResult result;
    int row;
    int column;
    int value;
    int playerFound;

    mapInitialise(map);
    playerPosition->row = 0;
    playerPosition->column = 0;
    result = MAP_LOAD_OK;
    playerFound = 0;
    mapFile = fopen(fileName, "r");

    if (mapFile == NULL)
    {
        result = MAP_LOAD_OPEN_ERROR;
    }
    else
    {
        if (fscanf(mapFile, "%d %d", &map->rows, &map->columns) != 2 ||
            map->rows <= 0 || map->columns <= 0)
        {
            result = MAP_LOAD_CONTENT_ERROR;
        }

        if (result == MAP_LOAD_OK)
        {
            map->cells = (char **)malloc((size_t)map->rows *
                                         sizeof(char *));

            if (map->cells == NULL)
            {
                result = MAP_LOAD_MEMORY_ERROR;
            }
            else
            {
                row = 0;
                while (row < map->rows)
                {
                    map->cells[row] = NULL;
                    row++;
                }
            }
        }

        row = 0;
        while (row < map->rows && result == MAP_LOAD_OK)
        {
            map->cells[row] = (char *)malloc((size_t)map->columns *
                                             sizeof(char));

            if (map->cells[row] == NULL)
            {
                result = MAP_LOAD_MEMORY_ERROR;
            }

            row++;
        }

        row = 0;
        while (row < map->rows && result == MAP_LOAD_OK)
        {
            column = 0;

            while (column < map->columns && result == MAP_LOAD_OK)
            {
                if (fscanf(mapFile, "%d", &value) != 1)
                {
                    result = MAP_LOAD_CONTENT_ERROR;
                }
                else
                {
                    if (value == CELL_PLAYER)
                    {
                        playerPosition->row = row;
                        playerPosition->column = column;
                        map->cells[row][column] = CELL_EMPTY;
                        playerFound = 1;
                    }
                    else
                    {
                        map->cells[row][column] = (char)value;
                    }
                }

                column++;
            }

            row++;
        }

        if (result == MAP_LOAD_OK && playerFound == 0)
        {
            result = MAP_LOAD_CONTENT_ERROR;
        }

        fclose(mapFile);
    }

    if (result != MAP_LOAD_OK)
    {
        mapDestroy(map);
    }

    return result;
}

void mapDestroy(GameMap *map)
{
    int row;

    if (map != NULL)
    {
        if (map->cells != NULL)
        {
            row = 0;

            while (row < map->rows)
            {
                free(map->cells[row]);
                row++;
            }

            free(map->cells);
        }

        mapInitialise(map);
    }
}

int mapIsInside(const GameMap *map, int row, int column)
{
    int inside;

    inside = 0;

    if (map != NULL &&
        row >= 0 && row < map->rows &&
        column >= 0 && column < map->columns)
    {
        inside = 1;
    }

    return inside;
}

int mapGetCell(const GameMap *map, int row, int column)
{
    int cell;

    cell = CELL_BLOCK;

    if (mapIsInside(map, row, column) != 0)
    {
        cell = map->cells[row][column];
    }

    return cell;
}

int mapCellIsSolid(const GameMap *map, int row, int column)
{
    int cell;
    int solid;

    cell = mapGetCell(map, row, column);
    solid = 0;

    if (cell == CELL_BLOCK || cell == CELL_LADDER)
    {
        solid = 1;
    }

    return solid;
}
