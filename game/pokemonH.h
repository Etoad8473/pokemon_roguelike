#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define HEIGHT 21
#define WIDTH 80
#define WORLDSIZE 401

typedef struct tMapTile
{
    int worldX,worldY;
    int n,w,s,e;
    char map[HEIGHT][WIDTH];
} tMapTile;


void runner();
void fillMap(tMapTile *m, int n, int s, int e, int w);
void drawRect(tMapTile *m, char terrain, int y1, int x1, int y2, int x2);
void drawBorder(tMapTile *m, char terrain);
void sprinkleDraw(tMapTile *m, char terrain, int number);
void drawRoads(tMapTile *m, int n, int s, int e, int w, char pokM, char pokC, char roadC, char borderC, char gateC);
void martsOnRoad(tMapTile *m, int y1, int y2, int x1, char pokM, char pokC);
int getRandMnMx(int lowest, int size);
void printMap(tMapTile *m, int pY, int pX);
void linkMapToWorld(tMapTile *m, int y, int x);
int coordOrRand(int coordinate, int lowest, int size);
tMapTile* createMapTile(int y, int x, int n, int s, int e, int w);
int getRandLwSz(int lowest, int size);
void flyToTile(tMapTile **currMap, int y, int x);
int probability(double percent);
void printMapRaw(tMapTile *m);
