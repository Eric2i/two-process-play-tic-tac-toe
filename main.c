// Author: 2020020360@mail.buct.edu.cn
// Date: 16/Mar/2022
// OS.Assignment

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>

void Initial(char*);
void Display(char*);
char Frame(int, int, char*);
bool Judge(const char*);
void Move(const int*);
void Game_Setup();

char *GameBoard;
int result;
int *counter1, *counter2;

int main() {
    Game_Setup();

    int pid = fork();
    while(Judge(GameBoard)) {
        //printf("%p %d %p %d %d\n", counter1, *counter1, counter2, *counter2, pid);
        Move(&pid);
    }

    if(pid == 0) exit(0);
    else {
        wait(NULL);
        if(result) {printf("Winner:"); puts(result == 1? "player1": "player2");}
        else printf("Tie");
    }
    return 0;
}

void Game_Setup() {
    srand(time(0));
    counter1 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    counter2 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    GameBoard = (char*)mmap(NULL, 9*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    Initial(GameBoard);
    system("clear");
    Display(GameBoard);
    sleep(1);
}

void Move(const int *player) {
    int k;
    if(*player == 0 && *counter2 < *counter1) {
        // player2 move
        do{k = rand() % 9;} while(GameBoard[k] != ' ');
//        putchar('\n');
        GameBoard[k] = 'x';

        system("clear");
        Display(GameBoard);
        printf("Player2(x) Move, Player1(o) Wait\n");
        sleep(1);
        ++*counter2;
    }
    else if(*player > 0 && *counter1 == *counter2){
        // player1 move
        do{k = rand() % 9;} while(GameBoard[k] != ' ');
//        putchar('\n');
        GameBoard[k] = 'o';

        system("clear");
        Display(GameBoard);
        printf("Player1(o) Move, Player2(x) Wait\n");
        sleep(1);
        ++*counter1;
    }
}

void Initial(char x[]) {for(int i = 0; i < 9; i++) x[i] = ' ';}

void Display(char x[]) {
    int i, j;
    char outputs[15 * 7 + 1];
    for(i = 0; i <= 6; i++) {
        for(j = 0; j <= 6; j++) {
            outputs[i * 15 + j * 2] = Frame(i, j, x);
            outputs[i * 15 + j * 2 + 1] = ' ';
        }
        outputs[i*15 + 14] = '\n';
    }
    outputs[15 * 7] = '\0';
    puts(outputs);
}
char Frame(int x, int y, char Board[]) {
    char Edge[24][2] = { 0,1, 0,3, 0,5,
                    1,0, 1,2, 1,4, 1,6,
                    2,1, 2,3, 2,5,
                    3,0, 3,2, 3,4, 3,6,
                    4,1, 4,3, 4,5,
                    5,0, 5,2, 5,4, 5,6,
                    6,1, 6,3, 6,5};
    char Chess[9][2] = { 1,1, 1,3, 1,5,
                    3,1, 3,3, 3,5,
                    5,1, 5,3, 5,5};
    int item;
    for(item = 0; item < 24; item++) {
        if(Edge[item][0] == x && Edge[item][1] == y) {
            if(item%7 < 3) return '-';
            else return '|';
        }
    }
    for(item = 0; item < 9; item++) {
        if(Chess[item][0] == x && Chess[item][1] == y) {
            return Board[item];
        }
    }
    return ' ';
}

bool Judge(const char x[]) {
    int Solution[8][3] = {0,1,2,3,4,5,6,7,8,0,3,6,1,4,7,2,5,8,0,4,8,2,4,6};
    int cases;
    for(cases = 0; cases < 8; cases++) {
        if(x[Solution[cases][0]] == x[Solution[cases][1]] && x[Solution[cases][1]] == x[Solution[cases][2]]) {
            if(x[Solution[cases][2]] == 'x') result = 2; else if(x[Solution[cases][2]] == 'o') result = 1; else return true;
            return false;
        }
    }
    for(int i = 0; i < 9; i++) if(x[i] == ' ') return true;
    return false;
}

