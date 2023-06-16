#pragma once

#include <ncurses.h>
#include <iostream>

class ScoreBoard {
    const char* maxLength;
    WINDOW *board_win;
    void initialize() {
        wattron(board_win, COLOR_PAIR(0));
        for (int r = 0; r < 10; r++)
        for (int c = 0; c < 40; c++)
            mvwaddch(board_win, r, c, ' ');
        wattroff(board_win, COLOR_PAIR(0));
        wattron(board_win, COLOR_PAIR(4));
        for (int c = 0; c < 40; c++) {
            mvwaddch(board_win, 0, c, ' ');
            mvwaddch(board_win, 9, c, ' ');
        }
        for (int r = 0; r < 10; r++) {
            mvwaddch(board_win, r, 0, ' ');
            mvwaddch(board_win, r, 1, ' ');
            mvwaddch(board_win, r, 38, ' ');
            mvwaddch(board_win, r, 39, ' ');
        }
        wattroff(board_win, COLOR_PAIR(4));

        wattron(board_win, COLOR_PAIR(0));
        mvwaddstr(board_win, 2, 4, "Score Board");
        mvwaddstr(board_win, 4, 4, "B : ");
        mvwaddstr(board_win, 5, 4, "+ : ");
        mvwaddstr(board_win, 6, 4, "- : ");
        mvwaddstr(board_win, 7, 4, "G : ");
        wattroff(board_win, COLOR_PAIR(0));
    }

public:
    ScoreBoard() {
        this->maxLength = "20";
        board_win = newwin(10, 40, 2, 72);
        initialize();
        loadScore();
        refrash();
    }

    void loadScore(const char* currentLength = "4", const char* growth = "0", const char* poison = "0", const char* gate = "0") {
        mvwaddstr(board_win, 4, 8, currentLength);
        mvwaddstr(board_win, 4, 9,  " / ");
        mvwaddstr(board_win, 4, 12, maxLength);
        mvwaddstr(board_win, 5, 8, growth);
        mvwaddstr(board_win, 6, 8, poison);
        mvwaddstr(board_win, 7, 8, gate);
    }

    void refrash() {
        wrefresh(board_win);
    }


};