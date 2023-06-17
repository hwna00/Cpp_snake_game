#pragma once

#include <ncurses.h>
#include <iostream>

class MissionBoard {
    const char *b, *gi, *pi, *gate;
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
        mvwaddstr(board_win, 2, 4, "Mission Board");
        mvwaddstr(board_win, 4, 4, "B : ");
        mvwaddstr(board_win, 4, 8, b);
        mvwaddstr(board_win, 4, 9, " ( ");
        mvwaddstr(board_win, 4, 12, ")");
        mvwaddstr(board_win, 5, 4, "+ : ");
        mvwaddstr(board_win, 5, 8, gi);
        mvwaddstr(board_win, 5, 9, " ( ");
        mvwaddstr(board_win, 5, 12, ")");
        mvwaddstr(board_win, 6, 4, "- : ");
        mvwaddstr(board_win, 6, 8, pi);
        mvwaddstr(board_win, 6, 9, " ( ");
        mvwaddstr(board_win, 6, 12, ")");
        mvwaddstr(board_win, 7, 4, "G : ");
        mvwaddstr(board_win, 7, 8, gate);
        mvwaddstr(board_win, 7, 9, " ( ");
        mvwaddstr(board_win, 7, 12, ")");
        wattroff(board_win, COLOR_PAIR(0));
    }

public:
    MissionBoard() {}
    
    MissionBoard(const char *b, const char *gi, const char *pi, const char *gate) : b(b), gi(gi), pi(pi), gate(gate) {
        board_win = newwin(10, 40, 13, 72);
        initialize();
        loadMission();
        refrash();
    }

    void loadMission(bool isMb = false, bool isMgi = false, bool isMpi = false, bool isMgate = false) {
        mvwaddstr(board_win, 4, 11, isMb ? "V" : " ");
        mvwaddstr(board_win, 5, 11, isMgi ? "V" :" ");
        mvwaddstr(board_win, 6, 11, isMpi ? "V" : " ");
        mvwaddstr(board_win, 7, 11, isMgate ? "V" : " ");
    }

    void refrash() {
        wrefresh(board_win);
    }
};