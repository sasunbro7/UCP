#include <stdio.h>
#include <string.h>
#include "color.h"
#include "game.h"
#include "newSleep.h"
#include "terminal.h"

static void gameSetMessage(Game *game, const char *message)
{
    strncpy(game->message, message, GAME_MESSAGE_CAPACITY - 1U);
    game->message[GAME_MESSAGE_CAPACITY - 1U] = '\0';
}

/* A ladder under or behind the player counts as support, as does the floor. */
static int gameIsSupported(const Game *game)
{
    int supported;

    supported = 0;

    if (game->player.row == game->map.rows - 1)
    {
        supported = 1;
    }
    else if (mapGetCell(&game->map, game->player.row,
                        game->player.column) == CELL_LADDER)
    {
        supported = 1;
    }
    else if (mapCellIsSolid(&game->map, game->player.row + 1,
                            game->player.column) != 0)
    {
        supported = 1;
    }

    return supported;
}

/* Apply the movement rules without changing state or recording undo yet. */
static int gameFindDestination(const Game *game, int command,
                               Position *destination)
{
    int allowed;
    int currentCell;
    int cellBelow;

    allowed = 0;
    currentCell = mapGetCell(&game->map, game->player.row,
                             game->player.column);
    cellBelow = mapGetCell(&game->map, game->player.row + 1,
                           game->player.column);
    *destination = game->player;

    if (command == 'a' || command == 'd')
    {
        if (gameIsSupported(game) != 0)
        {
            if (command == 'a')
            {
                destination->column--;
            }
            else
            {
                destination->column++;
            }

            allowed = 1;
        }
    }
    else if (command == 'w')
    {
        if (currentCell == CELL_LADDER)
        {
            destination->row--;
            allowed = 1;
        }
    }
    else if (command == 's')
    {
        if (currentCell == CELL_LADDER || cellBelow == CELL_LADDER)
        {
            destination->row++;
            allowed = 1;
        }
    }

    if (allowed != 0)
    {
        if (mapIsInside(&game->map, destination->row,
                        destination->column) == 0 ||
            mapGetCell(&game->map, destination->row,
                       destination->column) == CELL_BLOCK)
        {
            allowed = 0;
        }
    }

    return allowed;
}

static void gameCheckCurrentCell(Game *game)
{
    int cell;

    cell = mapGetCell(&game->map, game->player.row,
                      game->player.column);

    if (cell == CELL_GOAL)
    {
        game->status = GAME_WON;
        gameSetMessage(game, "The goal has been reached.");
    }
    else if (cell == CELL_DANGER)
    {
        game->status = GAME_LOST;
        gameSetMessage(game, "The player touched a danger zone.");
    }
}

/*
 * Falling is one uninterrupted action. Only the position before the user's
 * move is kept in history; individual animation frames are deliberately not.
 */
static void gameApplyGravity(Game *game)
{
    int fallDistance;
    int falling;

    fallDistance = 0;
    falling = 0;

    if (game->status == GAME_PLAYING && gameIsSupported(game) == 0)
    {
        falling = 1;
    }

    while (falling != 0)
    {
        game->player.row++;
        fallDistance++;
        gameCheckCurrentCell(game);

        if (game->status == GAME_PLAYING)
        {
            gameSetMessage(game, "Falling...");
        }

        gameRender(game);

        if (game->status == GAME_PLAYING)
        {
            newSleep(0.15F);
        }

        if (game->status != GAME_PLAYING || gameIsSupported(game) != 0)
        {
            falling = 0;
        }
    }

    if (game->status == GAME_PLAYING && fallDistance >= 5)
    {
        game->status = GAME_LOST;
        gameSetMessage(game, "The player fell at least five grids.");
    }
    else if (game->status == GAME_PLAYING && fallDistance > 0)
    {
        gameSetMessage(game, "The player landed safely.");
    }
}

/* Save a stable position before every valid move so undo restores one action. */
static void gameApplyCommand(Game *game, int command)
{
    Position destination;
    Position previousPosition;

    if (command == 'u')
    {
        if (linkedListPopFront(&game->history, &previousPosition,
                               sizeof(Position)) != 0)
        {
            game->player = previousPosition;
            gameSetMessage(game, "The previous move was undone.");
        }
        else
        {
            gameSetMessage(game, "There are no moves to undo.");
        }
    }
    else if (command == 'w' || command == 's' ||
             command == 'a' || command == 'd')
    {
        if (gameFindDestination(game, command, &destination) != 0)
        {
            previousPosition = game->player;

            if (linkedListPushFront(&game->history, &previousPosition,
                                    sizeof(Position)) != 0)
            {
                game->player = destination;
                gameSetMessage(game, "Player moved.");
                gameCheckCurrentCell(game);

                if (game->status == GAME_PLAYING)
                {
                    gameApplyGravity(game);
                }
            }
            else
            {
                game->status = GAME_ERROR;
                gameSetMessage(game,
                               "Memory allocation failed while saving undo.");
            }
        }
        else
        {
            gameSetMessage(game, "That move is blocked.");
        }
    }
    else
    {
        gameSetMessage(game, "Invalid key.");
    }
}

static void gamePrintBorder(int columns)
{
    int column;

    setForeground("red");
    column = 0;

    while (column < columns + 2)
    {
        putchar('*');
        column++;
    }

    setForeground("reset");
    putchar('\n');
}

/* Overlay the player at render time while preserving the underlying map cell. */
static void gamePrintCell(const Game *game, int row, int column)
{
    int cell;
    char symbol;
    const char *background;

    cell = mapGetCell(&game->map, row, column);
    symbol = ' ';
    background = "reset";

    if (row == game->player.row && column == game->player.column)
    {
        symbol = 'P';
        background = "blue";
    }
    else if (cell == CELL_BLOCK)
    {
        background = "white";
    }
    else if (cell == CELL_LADDER)
    {
        symbol = 'H';
    }
    else if (cell == CELL_DANGER)
    {
        background = "red";
    }
    else if (cell == CELL_GOAL)
    {
        symbol = 'G';
        background = "green";
    }

    setBackground(background);
    putchar(symbol);
    setBackground("reset");
}

static int gameReadCommand(void)
{
    int command;

    disableBuffer();
    command = getchar();
    enableBuffer();

    return command;
}

static void gamePrintResult(const Game *game)
{
    if (game->status == GAME_WON)
    {
        setForeground("green");
        printf("You win!\n");
        setForeground("reset");
    }
    else if (game->status == GAME_LOST)
    {
        setForeground("red");
        printf("You lose!\n");
        setForeground("reset");
    }
    else if (game->status == GAME_STOPPED)
    {
        printf("Input ended. Game stopped.\n");
    }
    else if (game->status == GAME_ERROR)
    {
        fprintf(stderr, "The game stopped because of an internal error.\n");
    }
}

int gameInitialise(Game *game, const char *mapFileName)
{
    MapLoadResult loadResult;
    int success;

    mapInitialise(&game->map);
    linkedListInitialise(&game->history);
    game->player.row = 0;
    game->player.column = 0;
    game->status = GAME_ERROR;
    game->message[0] = '\0';
    success = 0;

    loadResult = mapLoad(mapFileName, &game->map, &game->player);

    if (loadResult == MAP_LOAD_OK)
    {
        game->status = GAME_PLAYING;
        gameSetMessage(game, "Reach G while avoiding danger and long falls.");
        success = 1;
    }
    else if (loadResult == MAP_LOAD_OPEN_ERROR)
    {
        fprintf(stderr, "Error: could not open map file \"%s\".\n",
                mapFileName);
    }
    else if (loadResult == MAP_LOAD_MEMORY_ERROR)
    {
        fprintf(stderr, "Error: memory allocation failed while loading map.\n");
    }
    else
    {
        fprintf(stderr, "Error: map file contents could not be read.\n");
    }

    return success;
}

GameStatus gameRun(Game *game)
{
    int command;

    gameRender(game);

    while (game->status == GAME_PLAYING)
    {
        printf("Command: ");
        fflush(stdout);
        command = gameReadCommand();

        if (command == EOF)
        {
            game->status = GAME_STOPPED;
            gameSetMessage(game, "No more input was available.");
        }
        else
        {
            gameApplyCommand(game, command);
        }

        gameRender(game);
    }

    gamePrintResult(game);

    return game->status;
}

void gameRender(const Game *game)
{
    int row;
    int column;

    printf("\033[2J\033[H");
    gamePrintBorder(game->map.columns);
    row = 0;

    while (row < game->map.rows)
    {
        setForeground("red");
        putchar('*');
        setForeground("reset");
        column = 0;

        while (column < game->map.columns)
        {
            gamePrintCell(game, row, column);
            column++;
        }

        setForeground("red");
        putchar('*');
        setForeground("reset");
        putchar('\n');
        row++;
    }

    gamePrintBorder(game->map.columns);
    printf("w: up | s: down | a: left | d: right | u: undo\n");

    if (game->message[0] != '\0')
    {
        printf("%s\n", game->message);
    }

    fflush(stdout);
}

void gameDestroy(Game *game)
{
    if (game != NULL)
    {
        linkedListClear(&game->history);
        mapDestroy(&game->map);
        game->message[0] = '\0';
        game->status = GAME_STOPPED;
    }
}
