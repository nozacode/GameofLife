#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#define N 25
#define M 80

int input(int **matr);
void output(int **matrix);
void nextstep(int **matr);
int livingcells(int **matr, int str, int slb);
void clone(int **matr, int **buff);
long int edit_time(long int interval, char command, int *speed);

int main() {
    int a[N][M], b[N][M];
    int *matr[N];
    int *buff[N];
    for (int i = 0; i < N; i++) {
        matr[i] = &a[i][0];
        buff[i] = &b[i][0];
    }
    if (input(matr) == 0) {
        printf("n/a");
    } else {
        if (freopen("/dev/tty", "r", stdin)) initscr();
        nodelay(stdscr, true);
        noecho();
        long int sec = 100000;
        int speed = 5;
        while (1) {
            char c = getch();
            sec = edit_time(sec, c, &speed);

            clone(matr, buff);
            nextstep(matr);
            usleep(sec);

            clear();

            output(matr);
            refresh();
        }
    }
    return 0;
}

int input(int **matr) {
    int res = 1;
    for (int i = 0; i < N && res; i++) {
        for (int j = 0; j < M && res; j++) {
            if (scanf("%d", &matr[i][j]) == 0 || (matr[i][j] != 0 && matr[i][j] != 1)) {
                res = 0;
                j = M;
            }
        }
    }
    return res;
}

int livingcells(int **matr, int str, int slb) {
    int k = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int sosedstr = ((str + i) + N) % N;
            int sosedslb = ((slb + j) + M) % M;
            k += matr[sosedstr][sosedslb];
        }
    }
    k -= matr[str][slb];
    return k;
}

void nextstep(int **matr) {
    int buff[N][M];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int sosedi = livingcells(matr, i, j);
            if (matr[i][j] == 1) {
                if (sosedi < 2 || sosedi > 3) {
                    buff[i][j] = 0;
                } else {
                    buff[i][j] = 1;
                }
            } else {
                if (sosedi == 3) {
                    buff[i][j] = 1;
                } else {
                    buff[i][j] = 0;
                }
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            matr[i][j] = buff[i][j];
        }
    }
}

void output(int **matr) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (j == M - 1 && matr[i][j] == 0) {
                printw(" \n");
            } else if (j == M - 1 && matr[i][j] == 1) {
                printw("@\n");
            } else if (matr[i][j] == 1) {
                printw("@");
            } else if (matr[i][j] == 0) {
                printw(" ");
            }
        }
    }
}

void clone(int **matr, int **buff) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) buff[i][j] = matr[i][j];
}
long int edit_time(long int interval, char command, int *speed) {
    if ((command == 'w' || command == 'W') && (*speed) < 10) {
        interval = interval / 1.6;
        (*speed)++;
    }
    if ((command == 's' || command == 'S') && (*speed) > 1) {
        interval = interval * 1.6;
        (*speed)--;
    }
    return interval;
}
