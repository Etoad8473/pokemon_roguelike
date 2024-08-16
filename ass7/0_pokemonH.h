#ifndef POKEMONH_H
#define POKEMONH_H

// #include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <time.h>
#include <string.h>
#include <limits.h>
// #include <windows.h>
#include <unistd.h>
#include <ncurses.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define HEIGHT 21
#define WIDTH 80
#define WORLDSIZE 401
#define NUMTERRAINTYPES 11
#define DEFAULT_NUM_TRAINERS 5
#define NPC_SPAWN_ATTEMPTS 10



class Map;

class Character{
    public:
    int* weights;
    // int* Map;
    int y;
    int x;
    char character;
    int nextTurn;
    int turnOrder;
    int currDir;
    int defeated;
    
    Map* (*moveFunct)(struct Map* m, struct Character* c);
    //pointer to generic movement function
};

class Player : public Character{

};

class Npc : public Character{

};

class Node
{
    public:
    struct Node *parent;
    int heapPos;
    int y;
    int x;
    int currWeight;
    int status;
    int order;

}; //dijkstra node

class Heap
{
    public:
    Node** heapArr;
    int currSize;
    int maxSize;

    //Heap* createHeap(int size);//CONSTRUCTOR
    Heap(int size);
    ~Heap(); //DESTRUCTOR

    Node* extractMin();
    Node* peekMin();
    int heapIsEmpty();
    void heapInsert(Node* d);
    void heapifyDown(int index);
    void heapifyUp(int index);

    int compareNodeWeight(Node* a, Node* b) const;


    private:
    void doubleHeapSize();
    void swap(int aPos, int bPos);
    int rightChild(int pos);
    int leftChild(int pos);
    int parent(int pos);
};

class Map
{
    public:
    int worldX,worldY;
    int n,w,s,e;
    char map[HEIGHT][WIDTH];
    Character* cMap[HEIGHT][WIDTH];//TODO
    Character** npcList;
    int numNPCs;
    int characterOrder;
    Heap* turnHeap;
    int turn;

};






extern Map* worldMap[401][401];
extern int adjVer[9];
extern int adjHor[9];

extern int playerW[NUMTERRAINTYPES];
extern int hikerW[NUMTERRAINTYPES];
extern int rivalW[NUMTERRAINTYPES];
extern int otherW[NUMTERRAINTYPES];
extern char terrTypes[NUMTERRAINTYPES];

extern Node* hikerPMap[HEIGHT][WIDTH];
extern Node* rivalPMap[HEIGHT][WIDTH];

extern Player* PLAYER;
extern int quitGame;
extern char errorMsg[];




//void runner(int numNPCs); //deprecated
void fillMap(Map *m, int n, int s, int e, int w);
void drawRect(Map *m, char terrain, int y1, int x1, int y2, int x2);
void drawBorder(Map *m, char terrain);
void sprinkleDraw(Map *m, char terrain, int number);
void drawRoads(Map *m, int n, int s, int e, int w, char pokM, char pokC, char roadC, char borderC, char gateC);
void martsOnRoad(Map *m, int y1, int y2, int x1, char pokM, char pokC);
int getRandMnMx(int lowest, int size);
void printMap(Map *m);
void linkMapToWorld(Map *m, int y, int x);
int coordOrRand(int coordinate, int lowest, int size);
Map* createMapTile(int y, int x, int n, int s, int e, int w, int numNPCs);
int getRandLwSz(int lowest, int size);
Map* flyToTile(Map* prevM, int y, int x, char newGate);
int probability(double percent);
void printMapRaw(Map *m);


//3:Dijkstra stuff

// void temporaryInsert(Node *d, Heap* h); //deprecated
void printDijNode(Node *d);
void printHeap(Heap* h);
void initPathMap(Node* pMap[HEIGHT][WIDTH]);
void resetPathMap(Node* pMap[HEIGHT][WIDTH]);
void printPathMap(Node* pMap[HEIGHT][WIDTH]);
Node* createDNode(int y, int x, int weight, int status);//CONSTRUCTOR
void printHeapShort(Heap* h, int num);
int getTerrainWeight(int weight[], Map* m, int y, int x);
int dijkstra(Map* m, int weight[], Node* pMap[HEIGHT][WIDTH]);


//4: NPC Stuff

Character* spawnCharacterHelper(char ch, Map* m, int y, int x, int order);
int playerExists();
int nextCharacterOrder(Map *m);
void printCharacter(Character* c);
void printCMap(Map* m);
void updateCPosition(Map *m, Character *c, int nY, int nX);
int rotateDir(int dir, int rot);
Map* sentryMove(Map* m, Character* c);
Map* pacerMove(Map* m, Character* c);
Map* explorerMove(Map* m, Character* c);
Map* wandererMove(Map* m, Character* c);
Map* hikerMove(Map* m, Character* c);
Map* rivalMove(Map* m, Character* c);
Map* playerMove(Map* m, Character* c);
// void npcRunner(); //deprecated
Character* spawnNPC(char ch, Map* m, int y, int x);
int cIsPlayer(Character* c);
char getRandNPCType();
void spawnAllNPCs(int num, Map* m);


//5: Keyboard input stuff (1.5)

void pokemonBattle_cutscene(Map* m, Character* npc);
Map* keyboardInput(Map *m, Character *pc);
//void setMessage(char* string); //TODO
void gameRunner(int numNPC);
void pokemart_cutScene(Map *m, Character *pc);
void trainersList_cutscene(Map* m, Character* pc);
void npcPrintStatement(Map* m, Character* pc, Character* npc);


//6: better mapTile traversal

Map* initializeGame(int numNPCs);
void sendPlayerToNewMap(Map* oldM, Map* newM);


//7: csv parsing

// int csvRunner(int argc, char* argv[]);
int mainParseCSVs();





#endif
