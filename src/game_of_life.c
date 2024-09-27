// Copyright 2022 <nobukoto>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ncurses.h>
#define ROW 23
#define COLUMN 78

void render_frame(char a[ROW][COLUMN]);
void init(char a[ROW][COLUMN]);
char check_life(char a[ROW][COLUMN], int k, int l);
int insert_people(char a[ROW][COLUMN]);

int main() {
    char a[ROW][COLUMN], b[ROW][COLUMN], scan;
    int speed = 500000, blank, i, j;
    init(a);
    init(b);
    initscr();
    clear();
    cbreak();
    WINDOW *menu_win;
    menu_win = newwin(25, 81, 25, 0);
    nodelay(menu_win, TRUE);
    keypad(menu_win, TRUE);
    refresh();
    printw("Choose your starting blank:\n");
    printw("1) blank1.txt\n");
    printw("2) blank2.txt\n");
    printw("3) blank3.txt\n");
    printw("4) blank4.txt\n");
    printw("5) blank5.txt\n");
    FILE *file;
    if (scanw("%d", &blank) != 1 || blank < 1 || blank > 5) {
        printw("Incorrect input empty blank\n");
        refresh();
        usleep(3000000);
    } else {
     if (blank == 1) {
        file = fopen("blank1.txt", "r");
    } else if (blank == 2) {
        file = fopen("blank2.txt", "r");
    } else if (blank == 3) {
        file = fopen("blank3.txt", "r");
    } else if (blank == 4) {
        file = fopen("blank4.txt", "r");
    } else if (blank == 5) {
        file = fopen("blank5.txt", "r");
    }
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j ++) {
            fscanf(file, "%c", &scan);
            if (scan == '0') a[i][j] = ' ';
            if (scan == '1') a[i][j] = '*';
            scan = ' ';
        }
        fscanf(file, "%c", &scan);
    }
    fclose(file);
    }
    while (1) {
        int key = wgetch(menu_win);
        if (key == 27) break;
        if (key == 49) speed = 100000;
        if (key == 50) speed = 250000;
        if (key == 51) speed = 500000;
        if (key == 52) speed = 2000000;
        if (key == 105) {
        if (insert_people(a) == 1) {
                printw("n/a");
                break;
            }
        }
        for (i = 0; i < ROW; i++) {
            for (j = 0; j < COLUMN; j++) {
                b[i][j] = check_life(a, i, j);
            }
        }
        for (i = 0; i < ROW; i++) {
            for (j = 0; j < COLUMN; j++) {
                a[i][j] = b[i][j];
            }
        }
        render_frame(a);
        refresh();
        usleep(speed);
    }
    clrtoeol();
    endwin();
    return 0;
}

void render_frame(char a[ROW][COLUMN]) {
    int i, j, w = 80, h = 25;
    clear();
    printw("----------------------------------------");
    printw("----------------------------------------\n");
    for (i = 0; i < h - 2; i++) {
        printw("|");
        for (j = 0; j < w - 2; j++) {
            printw("%c", a[i][j]);
        }
        printw("|\n");
    }
    printw("----------------------------------------");
    printw("----------------------------------------\n");
}

void init(char a[ROW][COLUMN]) {
    int i, j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            a[i][j] = ' ';
        }
    }
}

char check_life(char a[ROW][COLUMN], int k, int l) {
    char exit = ' ';
    int check_k, check_l;
    int sum = 0, i, j;
    for (i = -1; i < 2; i++) {
        for (j = -1; j < 2; j++) {
            if (k + i < 0) {
                check_k = k + i + ROW;
            } else if (k + i > 22) {
                check_k = k + i - ROW;
            } else {
                check_k = k + i;
            }
            if (l + j < 0) {
                check_l = l + j + COLUMN;
            } else if (l + j > 77) {
                check_l = l + j - COLUMN;
            } else {
                check_l = l + j;
            }
            if (check_k == k && check_l == l) {
            } else {
                if (a[check_k][check_l] == '*') {
                    sum += 1;
                }
            }
        }
    }
    if (a[k][l] == '*' && sum < 2) exit = ' ';
    if (a[k][l] == '*' && (sum == 2 || sum == 3)) exit = '*';
    if (a[k][l] == '*' && sum > 3) exit = ' ';
    if (a[k][l] == ' ' && sum == 3) exit = '*';
    return exit;
}

int insert_people(char a[ROW][COLUMN]) {
    int number = 0, i = 0, exit = 0, k = 0, l = 0;
    printw("How many people do you want to kill or give birth?\n");
    scanw("%d", &number);
    for (i = 0; i < number; i++) {
        if (scanw("%d%d", &k, &l) != 2 || k < 0 || k >= COLUMN ||
            l < 0 || l >= ROW) {
            exit = 1;
        } else {
            if (a[l][k] == '*') {
                a[l][k] = ' ';
            } else {
                a[l][k] = '*';
            }
        }
    }
    return exit;
}
