#ifndef CLI_PRESENTER_H
#define CLI_PRESENTER_H

#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#define TIMEOUT 10
#define SCORE_LABEL "SCORE"
#define NEXT_LABLE "NEXT"
#define TOPSCORE_LABEL "TOP SCORE"
#define LEVEL_LABEL "LEVEL"
#define SPEED_LABEL "SPEED"
#define HINTS_LABEL "HINTS"
#define GAME_STATUS_LABEL "GAME STATUS"

#ifdef __cplusplus
extern "C" {
#endif

void initPresentation();
void freePresentation();
void printBoardLayout();
void printFrame(int top, int left, const int height, const int width,
                const char *label);
void printGameField(int **gameMatrix, const int state);
void printGameBoard();

#ifdef __cplusplus
}
#endif

#endif