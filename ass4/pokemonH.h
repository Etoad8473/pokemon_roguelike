#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <windows.h>

#define HEIGHT 21
#define WIDTH 80
#define WORLDSIZE 401
#define NUMTERRAINTYPES 11
#define DEFAULT_NUM_TRAINERS 5
#define NPC_SPAWN_ATTEMPTS 5



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
    
    void (*moveFunct)(struct tMapTile* m, struct characterT* c);
    //pointer to generic movement function

} characterT;

typedef struct tMapTile
{
    int worldX,worldY;
    int n,w,s,e;
    char map[HEIGHT][WIDTH];
    characterT* cMap[HEIGHT][WIDTH];//TODO
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





tMapTile* worldMap[401][401];
int adjVer[8];
int adjHor[8];

int playerW[NUMTERRAINTYPES];
int hikerW[NUMTERRAINTYPES];
int rivalW[NUMTERRAINTYPES];
int swimmerW[NUMTERRAINTYPES];
int otherW[NUMTERRAINTYPES];
char terrTypes[NUMTERRAINTYPES];

dNode_t* hikerPMap[HEIGHT][WIDTH];
dNode_t* rivalPMap[HEIGHT][WIDTH];

characterT* player;



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
