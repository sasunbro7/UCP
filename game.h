#ifndef GAME_H
#define GAME_H

#include "linkedList.h"
#include "map.h"

#define GAME_MESSAGE_CAPACITY 128

typedef enum
{
    GAME_PLAYING,
    GAME_WON,
    GAME_LOST,
    GAME_STOPPED,
    GAME_ERROR
} GameStatus;

typedef struct
{
    GameMap map;
    Position player;
    LinkedList history;
    GameStatus status;
    char message[GAME_MESSAGE_CAPACITY];
} Game;

int gameInitialise(Game *game, const char *mapFileName);
GameStatus gameRun(Game *game);
void gameRender(const Game *game);
void gameDestroy(Game *game);

#endif
