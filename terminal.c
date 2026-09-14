#include <termios.h>
#include <unistd.h>
#include "terminal.h"

void disableBuffer(void)
{
    struct termios mode;

    /* A redirected test input is not a terminal, so only change valid TTYs. */
    if (tcgetattr(STDIN_FILENO, &mode) == 0)
    {
        mode.c_lflag &= (tcflag_t)~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &mode);
    }
}

void enableBuffer(void)
{
    struct termios mode;

    if (tcgetattr(STDIN_FILENO, &mode) == 0)
    {
        mode.c_lflag |= (ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &mode);
    }
}
