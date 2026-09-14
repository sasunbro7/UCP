#include <stdio.h>
#include "game.h"

int main(int argc, char *argv[])
{
    Game game;
    GameStatus gameStatus;
    int programStatus;

    programStatus = 1;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
    }
    else
    {
        if (gameInitialise(&game, argv[1]) != 0)
        {
            gameStatus = gameRun(&game);

            if (gameStatus != GAME_ERROR)
            {
                programStatus = 0;
            }

            gameDestroy(&game);
        }
        else
        {
            gameDestroy(&game);
        }
    }

    return programStatus;
}
