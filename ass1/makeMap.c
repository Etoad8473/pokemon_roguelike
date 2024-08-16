#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 21
#define WIDTH 80

typedef struct MAP{
    int height;
    int width;
    char map[HEIGHT][WIDTH];
    int seed;
} tMapTile;

void fillRect(tMapTile *m, char terrain, int y1, int x1, int y2, int x2);
void drawBorder(tMapTile *m, char terrain);
void sprinkle(tMapTile *m, char terrain, int number);
void drawRoads(tMapTile *m);
void martsOnRoad(tMapTile *m, int y1, int y2, int x1);
int getRandMnMx(int lowest, int size);
void printMap(tMapTile *m);

int main() {
    tMapTile m;
    m.height = HEIGHT;
    m.width = WIDTH;
    m.seed = time(NULL);
    srand(m.seed);

    fillRect(&m, '.', 0, 0, m.height - 1, m.width - 1);
    fillRect(&m, ':', getRandMnMx(0, m.height - 1), getRandMnMx(0, m.width - 1), getRandMnMx(0, m.height - 1), getRandMnMx(0, m.width - 1));
    sprinkle(&m, ':', getRandMnMx(0, 20));
    fillRect(&m, '~', getRandMnMx(0, m.height - 1), getRandMnMx(0, m.width - 1), getRandMnMx(0, m.height - 1), getRandMnMx(0, m.width - 1));
    fillRect(&m, '%', getRandMnMx(0, m.height - 1), getRandMnMx(0, m.width - 1), getRandMnMx(0, m.height - 1), getRandMnMx(0, m.width - 1));
    sprinkle(&m, '%', getRandMnMx(0, 5));
    fillRect(&m, '^', 6, 3, 5, 1);
    sprinkle(&m, '^', getRandMnMx(0, 10));
    drawBorder(&m, '%');
    drawRoads(&m);

    printMap(&m);

    return 0;
}

void fillRect(tMapTile *m, char terrain, int y1, int x1, int y2, int x2) {
    int yMin, yMax, xMin, xMax;
    yMax = m->height - 1;
    xMax = m->width - 1;
    yMin = 0;
    xMin = 0;

    int y, x;

    // FORMAT & BOUND INPUT
    if (x1 > x2) {
        x = x2;
        x2 = x1;
        x1 = x;
    }
    if (y1 > y2) {
        y = y2;
        y2 = y1;
        y1 = y;
    }

    if (x1 < xMin) {
        x1 = xMin;
    }
    if (y1 < yMin) {
        y1 = yMin;
    }
    if (x2 > xMax) {
        x2 = xMax;
    }
    if (y2 > yMax) {
        y2 = yMax;
    }

    for (int i = y1; i <= y2; i++) {
        for (int j = x1; j <= x2; j++) {
            m->map[i][j] = terrain;
        }
    }
}

void drawBorder(tMapTile *m, char terrain) {
    int hM = m->height - 1;
    int wM = m->width - 1;
    fillRect(m, terrain, 0, 0, 0, wM);
    fillRect(m, terrain, 0, 0, hM, 0);
    fillRect(m, terrain, hM, 0, hM, wM);
    fillRect(m, terrain, 0, wM, hM, wM);
}

void sprinkle(tMapTile *m, char terrain, int number) {
    for (int i = 0; i < number; i++) {
        m->map[getRandMnMx(0, m->height - 1)][getRandMnMx(0, m->width - 1)] = terrain;
    }
}

void drawRoads(tMapTile *m) {
    int y1 = getRandMnMx(3, m->height - 4);
    int y2 = getRandMnMx(3, m->height - 4);
    int x1 = getRandMnMx(3, m->width - 4);

    martsOnRoad(m, y1, y2, x1);

    fillRect(m, '#', y1, 0, y1, x1);
    fillRect(m, '#', y2, x1, y2, m->width - 1);
    fillRect(m, '#', y1, x1, y2, x1);

    y1 = getRandMnMx(2, m->height - 3);
    int x2 = getRandMnMx(2, m->width - 3);

    fillRect(m, '#', 0, x1, y1, x1);
    fillRect(m, '#', y1, x2, m->height - 1, x2);
    fillRect(m, '#', y1, x1, y1, x2);
}

void martsOnRoad(tMapTile *m, int y1, int y2, int x1) {
    int pX = getRandMnMx(2, x1 - 2);
    y1++;

    fillRect(m, 'p', y1, pX, y1 + 1, pX - 1);

    int cX = getRandMnMx(x1, m->width - 3 - x1);
    y2--;
    fillRect(m, 'c', y2, cX, y2 - 1, cX + 1);
}

int getRandMnMx(int lowest, int size) {
    return lowest + rand() % size;
}

void printMap(tMapTile *m) {
    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            printf("%c", m->map[i][j]);
        }
        printf("\n");
    }
}