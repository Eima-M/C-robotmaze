#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphics.h"

typedef struct board {
    int rCoords[2];
    int rDirection;
    int rHome[2];
    int rNextTile[2];
    int **rMemory;
    int rMarkerCount;
    int markerCoords[3][2];
    int obstacleCoords[5][2];
    int arenaX;
    int arenaY;
    int sleepTime;
} board;

void drawStage(board *ptr) {
    #define p1 (*ptr) // taken from Copilot, defines an alias for the pointers
    background();
    // draws the arena
    setColour(red);
    fillRect(50, 50, (p1.arenaX + 2) * 50, (p1.arenaY + 2) * 50);
    setColour(white);
    fillRect(100, 100, p1.arenaX * 50, p1.arenaY * 50);
    // creates 3 markers on the grid, while making sure they do not overlap with eachother or the home tile
    setColour(pink);
    for (int i = 0; i < 3; i++) {
        int markerOverlap;
        do {
            markerOverlap = 0;
            p1.markerCoords[i][0] = rand() % p1.arenaX + 1;
            p1.markerCoords[i][1] = rand() % p1.arenaY + 1;
            if (p1.markerCoords[i][0] == p1.rHome[0] && p1.markerCoords[i][1] == p1.rHome[1]) {
                markerOverlap = 1;
            }
            for (int j = 0; j < i; j++) {
                if (p1.markerCoords[i][0] == p1.markerCoords[j][0] && p1.markerCoords[i][1] == p1.markerCoords[j][1]) {
                    markerOverlap = 1;
                    break;
                }
            }
        } while (markerOverlap);
        fillRect(p1.markerCoords[i][0] * 50 + 50, p1.markerCoords[i][1] * 50 + 50, 50, 50);
    }
    // creates 5 obstacles on the grid, while making sure they do not overlap with the markers, eachother or the home tile
    setColour(black);
    for (int i = 0; i < 5; i++) {
        int obstacleOverlap;
        do {
            obstacleOverlap = 0;
            p1.obstacleCoords[i][0] = rand() % p1.arenaX + 1;
            p1.obstacleCoords[i][1] = rand() % p1.arenaY + 1;
            if (p1.obstacleCoords[i][0] == p1.rHome[0] && p1.obstacleCoords[i][1] == p1.rHome[1]) {
                obstacleOverlap = 1;
            }
            for (int j = 0; j < 3; j++) {
                if (p1.obstacleCoords[i][0] == p1.markerCoords[j][0] && p1.obstacleCoords[i][1] == p1.markerCoords[j][1]) {
                    obstacleOverlap = 1;
                    break;
                }
            }
            for (int k = 0; k < i; k++) {
                if (p1.obstacleCoords[i][0] == p1.obstacleCoords[k][0] && p1.obstacleCoords[i][1] == p1.obstacleCoords[k][1]) {
                    obstacleOverlap = 1;
                    break;
                }
            }

        } while (obstacleOverlap);
        fillRect(p1.obstacleCoords[i][0] * 50 + 50, p1.obstacleCoords[i][1] * 50 + 50, 50, 50);
    }
    setColour(black);
    for (int column = 0; column < p1.arenaX + 1; column++) {
        int columnX = 100 + 50 * column;
        drawLine(columnX, 100, columnX, 100 + p1.arenaY * 50);
    }
    for (int row = 0; row < p1.arenaY + 1; row++) {
        int rowY = 100 + 50 * row;
        drawLine(100, rowY, 100 + p1.arenaX * 50, rowY);
    }
    #undef p1
}

void drawHome(int *x, int *y) {
    background();
    setColour(cyan);
    fillRect(*x * 50 + 50, *y * 50 + 50, 50, 50);
    setColour(black);
    drawRect(*x * 50 + 50, *y * 50 + 50, 50, 50);
}

// dynamically allocates memory for the 2d array 'rMemory' which logs the tiles the robot has been on
void initialiseRMemory(board *ptr) {
    #define p1 (*ptr)
    p1.rMemory = malloc(p1.arenaX * sizeof(int *));
    for (int i = 0; i < p1.arenaX; i++) {
        p1.rMemory[i] = malloc(p1.arenaY * sizeof(int));
    }
    // if the robot has not been on the tile, its value is 0. otherwise, it is 1, as seen in the function 'rRecMemory'
    for (int i = 0; i < p1.arenaX; i++) {
        for (int j = 0; j < p1.arenaY; j++) {
            p1.rMemory[i][j] = 0;
        }
    }
    for (int i = 0; i < 5; i++) {
        int x = p1.obstacleCoords[i][0] - 1;
        int y = p1.obstacleCoords[i][1] - 1;
        p1.rMemory[x][y] = 1;
    }
    #undef p1
}

void rRecMemory(board *ptr) {
    #define p1 (*ptr)
    p1.rMemory[p1.rCoords[0] - 1][p1.rCoords[1] - 1] = 1;
    #undef p1
}

// updates the visual position of the robot, with different sets of coordinates for its vertices depending on the direction it is facing
void rUpdate(int *x, int *y, int *direction) {
    foreground();
    setColour(green);
    clear();
    int xCoords[3];
    int yCoords[3];
    if (*direction == 0) {
        xCoords[0] = 95 + 50 * *x;
        yCoords[0] = 95 + 50 * *y;
        xCoords[1] = 55 + 50 * *x;
        yCoords[1] = 95 + 50 * *y;
        xCoords[2] = 75 + 50 * *x;
        yCoords[2] = 55 + 50 * *y;
    }
    else if (*direction == 1) {
        xCoords[0] = 55 + 50 * *x;
        yCoords[0] = 95 + 50 * *y;
        xCoords[1] = 55 + 50 * *x;
        yCoords[1] = 55 + 50 * *y;
        xCoords[2] = 95 + 50 * *x;
        yCoords[2] = 75 + 50 * *y;
    }
    else if (*direction == 2) {
        xCoords[0] = 55 + 50 * *x;
        yCoords[0] = 55 + 50 * *y;
        xCoords[1] = 95 + 50 * *x;
        yCoords[1] = 55 + 50 * *y;
        xCoords[2] = 75 + 50 * *x;
        yCoords[2] = 95 + 50 * *y;
    }
    else if (*direction == 3) {
        xCoords[0] = 95 + 50 * *x;
        yCoords[0] = 55 + 50 * *y;
        xCoords[1] = 95 + 50 * *x;
        yCoords[1] = 95 + 50 * *y;
        xCoords[2] = 55 + 50 * *x;
        yCoords[2] = 75 + 50 * *y;
    }
    fillPolygon(3, xCoords, yCoords);
}

int atMarker(int *x, int *y, int *xMarker, int *yMarker) {
    if (*x == *xMarker && *y == *yMarker) {
        return 1;
    }
    else {
        return 0;
    }
}

void pickUpMarker(int *xMarker, int *yMarker, int *count) {
    background();
    setColour(white);
    fillRect(*xMarker * 50 + 50, *yMarker * 50 + 50, 50, 50);
    setColour(black);
    drawRect(*xMarker * 50 + 50, *yMarker * 50 + 50, 50, 50);
    *xMarker = 0;
    *yMarker = 0;
    (*count)++;
    printf("Counters collected: %d", *count);
}

void dropMarker(int *count) {
    *count = 0;
    printf("All markers have been dropped at the home tile.\n");
}

int canMoveForward(board *ptr) {
    #define p1 (*ptr)
    // find the robot's next tile depending on its current direction
    int rNextX = p1.rCoords[0];
    int rNextY = p1.rCoords[1];
    if (p1.rDirection == 0) {
        rNextY--;
    }
    else if (p1.rDirection == 1) {
        rNextX++;
    }
    else if (p1.rDirection == 2) {
        rNextY++;
    }
    else if (p1.rDirection == 3) {
        rNextX--;
    }
    if (rNextX < 1 || rNextX > p1.arenaX || rNextY < 1 || rNextY > p1.arenaY) {
        return 0;
    }
    for (int i = 0; i < 5; i++) {
        if (rNextX == p1.obstacleCoords[i][0] && rNextY == p1.obstacleCoords[i][1]) {
            return 0;
        }
    }
    return 1;
    #undef p1
}

void forward(board *ptr) {
    #define p1 (*ptr)
    int canMoveForwardFlag = canMoveForward(&p1);
    // if the robot cannot move forwards, it picks a new random direction and recursively calls the 'forward' function
    if (canMoveForwardFlag == 0) {
        p1.rDirection = rand() % 4;
        forward(&p1);
    }
    else {
        sleep(p1.sleepTime);
        sleep(100); // I am not sure why this is necessary, but the robot blinks between non-adjacent tiles without it
        if (p1.rDirection == 0) {
            (p1.rCoords[1])--;
        }
        else if (p1.rDirection == 1) {
            (p1.rCoords[0])++;
        }
        else if (p1.rDirection == 2) {
            (p1.rCoords[1])++;
        }
        else if (p1.rDirection == 3) {
            (p1.rCoords[0])--;
        }
    }
    rUpdate(&p1.rCoords[0], &p1.rCoords[1], &p1.rDirection);
    rRecMemory(&p1);
    for (int i = 0; i < 3; i++) {
        int atMarkerFlag = atMarker(&p1.rCoords[0], &p1.rCoords[1], &p1.markerCoords[i][0], &p1.markerCoords[i][1]);
        if (atMarkerFlag) {
            pickUpMarker(&p1.markerCoords[i][0], &p1.markerCoords[i][1], &p1.rMarkerCount);
            break;
        }
    }
    #undef p1
}

// finds the first tile from 'rMemory' which the robot has not been to, and assigns it to the array 'rNextTile'
void findNextTile(board *ptr) {
    #define p1 (*ptr)
    for (int i = 0; i < p1.arenaX; i++) {
        for (int j = 0; j < p1.arenaY; j++) {
            if (p1.rMemory[i][j] == 0) {
                p1.rNextTile[0] = i + 1;
                p1.rNextTile[1] = j + 1;
                return;
            }
        }
    }
    #undef p1
}

void rGoToTile(board *ptr) {
    #define p1 (*ptr)
    int rCorrectX = 0;
    while (!rCorrectX) {
        if (p1.rCoords[0] != p1.rNextTile[0]) {
            if (p1.rCoords[0] < p1.rNextTile[0]) {
                p1.rDirection = 1;
            }
            else if (p1.rCoords[0] > p1.rNextTile[0]) {
                p1.rDirection = 3;
            }
            forward(&p1);
        }
        else {
            rCorrectX = 1;
        }
    }
    int rCorrectY = 0;
    while (!rCorrectY) {
        if (p1.rCoords[1] != p1.rNextTile[1]) {
            if (p1.rCoords[1] > p1.rNextTile[1]) {
                p1.rDirection = 0;
            }
            else if (p1.rCoords[1] < p1.rNextTile[1]) {
                p1.rDirection = 2;
            }
            forward(&p1);
        }
        else {
            rCorrectY = 1;
        }
    }
    #undef p1
}

void rMove(board *ptr) {
    #define p1 (*ptr)
    // the robot starts in a random direction on a random tile that is not adjacent to a wall
    p1.rDirection = rand() % 4;
    p1.rCoords[0] = p1.rHome[0];
    p1.rCoords[1] = p1.rHome[1];
    drawHome(&p1.rCoords[0], &p1.rCoords[1]);
    initialiseRMemory(&p1);
    rUpdate(&p1.rCoords[0], &p1.rCoords[1], &p1.rDirection);
    rRecMemory(&p1);
    sleep(p1.sleepTime);
    while (p1.rMarkerCount < 3) {
        findNextTile(&p1);
        rGoToTile(&p1);
    }
    // once all markers are collected, return home using the same 'rGoToTile' function
    p1.rNextTile[0] = p1.rHome[0];
    p1.rNextTile[1] = p1.rHome[1];
    rGoToTile(&p1);
    dropMarker(&p1.rMarkerCount);
    // frees the memory allocated by 'initialiseRMemory()'
    for (int i = 0; i < p1.arenaX; i++) {
        free(p1.rMemory);
    }
    free(p1.rMemory);
    #undef p1
}

int main() {
    board p1;
    srand(time(NULL)); // necessary for rand() to work, taken from Copilot
    p1.sleepTime = 200;
    p1.arenaX = rand() % 8 + 8;
    p1.arenaY = rand() % 5 + 6;
    p1.rHome[0] = rand() % (p1.arenaX - 2) + 2;
    p1.rHome[1] = rand() % (p1.arenaY - 2) + 2;
    p1.rMarkerCount = 0;
    setWindowSize(p1.arenaX * 50 + 200, p1.arenaY * 50 + 200);
    drawStage(&p1);
    rMove(&p1);
}