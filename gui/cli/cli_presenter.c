#include "cli_presenter.h"

void initPresentation() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    clear();
    timeout(TIMEOUT);
    printBoardLayout();
}

void freePresentation() {
    clear();
    endwin();
}

void printBoardLayout() {
    // Left part
    printFrame(1, 1, 3, 10, TOPSCORE_LABEL);
    printFrame(4, 1, 10, 10, "HINTS");
    // Main board
    printFrame(1, 21, 22, 12, NULL);
    // Right part
    printFrame(1, 45, 3, 10, SCORE_LABEL);
    printFrame(4, 45, 3, 10, LEVEL_LABEL);
    printFrame(7, 45, 6, 10, NEXT_LABLE);
    printFrame(13, 45, 3, 10, SPEED_LABEL);
    printFrame(16, 45, 7, 10, "Game status");
}

void printMenu() {
    mvprintw(5, 5, "PRESS \"ENTER\"");
    mvprintw(6, 5, "TO  CONTINUE");
    mvprintw(11, 6, "PRESS \"ESC\"");
    mvprintw(12, 7, "TO  EXIT");
}

void printPauseMsg() {
    mvprintw(3, 5, "\"P A U S E\"");
}

void printFrame(int top, int left, const int height, const int width, const char* label) {
    int lableLength = 0;

    if (height < 2 && width < 2) return;

    if (top < 0) top = 0;
    if (left < 0) left = 0;
    
    if (label != NULL) {
        lableLength = (int)strlen(label);
    }
    
    if (lableLength > (width * 2) - 2) {
        lableLength = width * 2 - 2;
    }

    int labelpos = left + 1 + (width * 2 - 2 - lableLength) / 2;

    for (int i = left; i < left + width * 2; i++) {
        if (lableLength > 0 && i >= labelpos && i < labelpos + lableLength) {
            mvaddch(top, i, label[i - labelpos]);
        } else {
            mvaddch(top, i, ACS_HLINE);
        }
        mvaddch(top + height - 1, i, ACS_HLINE);
    }

    for (int j = top; j < top + height; j++) {
        mvaddch(j, left, ACS_VLINE);
        mvaddch(j, left + (width * 2) - 1, ACS_VLINE);
    }

    mvaddch(top, left, ACS_ULCORNER);
    mvaddch(top, left + (width * 2) - 1, ACS_URCORNER);
    mvaddch(top + height - 1, left, ACS_LLCORNER);
    mvaddch(top + height - 1, left + (width * 2) - 1, ACS_LRCORNER);
    refresh();
}

void printGameBoard() {
    printGameField(NULL, 0);
}


void printGameField(int** gameMatrix, const int pause) {
    if (!gameMatrix) {
        printMenu();        
    } else if (pause) {
        printPauseMsg();
        printMenu();
    } else {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 20; j++) {
                if (gameMatrix[i][j] != 0) {
                    mvprintw(1 + i, 1 + j * 2, "[]");
                } else {
                    mvprintw(1 + i, 1 + j * 2, "  ");
                }
            }
        }
    }
}
