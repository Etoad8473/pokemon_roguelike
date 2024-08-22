#ifndef POKEMONH_H
#define POKEMONH_H

// #include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
// #include <windows.h>
#include <unistd.h>
#include <ncurses.h>

#define HEIGHT 21
#define WIDTH 80
#define WORLDSIZE 401
#define NUMTERRAINTYPES 11
#define DEFAULT_NUM_TRAINERS 5
#define NPC_SPAWN_ATTEMPTS 10



struct tMapTile;

typedef struct characterT{

    int* weights;
    int* tMapTile;
    int y;
    int x;
    char character;
    int nextTurn;
    int turnOrder;
    int currDir;
    int defeated;
    
    void (*moveFunct)(struct tMapTile* m, struct characterT* c);
    //pointer to generic movement function

} characterT;

typedef struct tMapTile
{
    int worldX,worldY;
    int n,w,s,e;
    char map[HEIGHT][WIDTH];
    characterT* cMap[HEIGHT][WIDTH];//TODO
    characterT** npcList;
    int numNPCs;
    int characterOrder;

} tMapTile;

typedef struct dNode_t
{
    struct dNode_t *parent;
    int heapPos;
    int y;
    int x;
    int currWeight;
    int status;
    int order;

}dNode_t; //dijkstra node

typedef struct heap_t
{
    dNode_t** heapArr;
    int currSize;
    int maxSize;
}heap_t;





extern tMapTile* worldMap[401][401];
extern int adjVer[9];
extern int adjHor[9];

extern int playerW[NUMTERRAINTYPES];
extern int hikerW[NUMTERRAINTYPES];
extern int rivalW[NUMTERRAINTYPES];
extern int otherW[NUMTERRAINTYPES];
extern char terrTypes[NUMTERRAINTYPES];

extern dNode_t* hikerPMap[HEIGHT][WIDTH];
extern dNode_t* rivalPMap[HEIGHT][WIDTH];

extern characterT* player;
extern int quitGame;
extern char errorMsg[];



void runner();
void fillMap(tMapTile *m, int n, int s, int e, int w);
void drawRect(tMapTile *m, char terrain, int y1, int x1, int y2, int x2);
void drawBorder(tMapTile *m, char terrain);
void sprinkleDraw(tMapTile *m, char terrain, int number);
void drawRoads(tMapTile *m, int n, int s, int e, int w, char pokM, char pokC, char roadC, char borderC, char gateC);
void martsOnRoad(tMapTile *m, int y1, int y2, int x1, char pokM, char pokC);
int getRandMnMx(int lowest, int size);
void printMap(tMapTile *m);
void linkMapToWorld(tMapTile *m, int y, int x);
int coordOrRand(int coordinate, int lowest, int size);
tMapTile* createMapTile(int y, int x, int n, int s, int e, int w);
int getRandLwSz(int lowest, int size);
void flyToTile(tMapTile **currMap, int y, int x);
int probability(double percent);
void printMapRaw(tMapTile *m);

//Dijkstra stuff
heap_t* increaseHeapSize(heap_t* h);
dNode_t* extractMin(heap_t* h);
dNode_t* peekMin(heap_t* h);
heap_t* createHeap(int size);
void temporaryInsert(dNode_t *d, heap_t* h);
void printDijNode(dNode_t *d);
void printHeap(heap_t* h);
void swap(int aPos, int bPos, heap_t* h);//maybe?
int rightChild(int pos);
int leftChild(int pos);
int parent(int pos);
int compareNodeWeight(dNode_t* a, dNode_t* b);
void doubleHeapSize(heap_t* h);
void heapifyDown(heap_t* h, int index);
void initPathMap(dNode_t* pMap[HEIGHT][WIDTH]);
void resetPathMap(dNode_t* pMap[HEIGHT][WIDTH]);
void printPathMap(dNode_t* pMap[HEIGHT][WIDTH]);
dNode_t* createDNode(int y, int x, int weight, int status);
int heapIsEmpty(heap_t* h);
void heapInsert(dNode_t* d, heap_t* h);
void heapifyUp(int index, heap_t* h);
void printHeapShort(heap_t* h, int num);
void freeHeap(heap_t* h);
int getTerrainWeight(int weight[], tMapTile* m, int y, int x);
int dijkstra(tMapTile* m, int weight[], dNode_t* pMap[HEIGHT][WIDTH]);


//character generation
characterT* spawnCharacterHelper(char ch, tMapTile* m, heap_t* h, int y, int x, int order);
int playerExists();

//NPC Stuff
int nextCharacterOrder(tMapTile *m);
void printCharacter(characterT* c);
void printCMap(tMapTile* m);
void updateCPosition(tMapTile *m, characterT *c, int nY, int nX);
int rotateDir(int dir, int rot);
void sentryMove(tMapTile* m, characterT* c);
void pacerMove(tMapTile* m, characterT* c);
void explorerMove(tMapTile* m, characterT* c);
void wandererMove(tMapTile* m, characterT* c);
void hikerMove(tMapTile* m, characterT* c);
void rivalMove(tMapTile* m, characterT* c);
void playerMove(tMapTile* m, characterT* c);
void swimmerMove(tMapTile* m, characterT* c);
void npcRunner();
characterT* spawnPlayer(tMapTile* m, heap_t* h);
characterT* spawnNPC(char ch, tMapTile* m, heap_t* h, int y, int x);
int cIsPlayer(characterT* c);
char getRandNPCType();
void spawnAllNPCs(int num, tMapTile* m, heap_t* h);

//Keyboard input stuff (1.5)
void pokemonBattle(tMapTile* m, characterT* npc);
void keyboardInput(tMapTile *m, characterT *pc);
void setMessage(char* string);


#endif
