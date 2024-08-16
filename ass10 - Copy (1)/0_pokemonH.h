#ifndef POKEMONH_H
#define POKEMONH_H

// #include <stdio.h>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <ncurses.h>
#include <sstream>
#include <string>
#include <string.h>
#include <time.h>
// #include <windows.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>


using namespace std;

#define HEIGHT 21
#define WIDTH 80
#define WORLDSIZE 401
#define NUMTERRAINTYPES 11
#define DEFAULT_NUM_TRAINERS 5
#define NPC_SPAWN_ATTEMPTS 10
#define ESCAPE_KEY 27

enum STAT{
    hpSTAT,
    attackSTAT,
    defenseSTAT,
    speedSTAT,
    spAttackSTAT,
    spDefSTAT
};


class Map;
class Pokemon;


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
    vector<Pokemon> *pokeRoster;

    Map* (*moveFunct)(struct Map* m, struct Character* c);
    //pointer to generic movement function
};

class Player : public Character{

    public:
    int pokeballs;
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
    int manhattan;
};


class PokemonInfo{
    public:
    int id;
    string name;
    int spc_id;
    int height;
    int weight;
    int base_exp;
    int order;
    int is_default;

    void print();
    PokemonInfo(int id, string identifier, int species_id, int h, int w, int base_experience, int order, int is_default);
    PokemonInfo(string line);
};


class Move{
    public:
    int id;
    string name;
    int gen;
    int type;
    int power;
    int pp;
    int accuracy;
    int priority;
    int target;
    int damage_class;
    int effect;
    int effect_chance;
    int contest_type;
    int contest_effect;
    int super_contest_effect;

    void print();
    Move(int id, string name, int gen, int type, int power, int pp, int accuracy, 
    int priority, int target, int damage_class, int effect, int effect_chance, 
    int contest_type, int contest_effect, int super_contest_effect);
    Move(string line);
};

class PokemonMove {
public:
    int pokemon_id;
    int version_group;
    int move_id;
    int pokemon_move_method_id;
    int level;
    int order;

    PokemonMove(int pokemon_id, int version_group, int move_id, int pokemon_move_method_id, int level, int order);
    PokemonMove(string line);
    void print();
};

class Species {
public:
    int id;
    string name;
    int evolves_from_species_id;
    int evolution_chain_id;
    int color_id;
    int shape_id;
    int habitat_id;
    int gender_rate;
    int capture_rate;
    int base_happiness;
    int is_baby;
    int hatch_counter;
    int has_gender_differences;
    int growth_rate_id;
    int forms_switchable;
    int is_legendary;
    int is_mythical;
    int order;
    int conquest_order;

    Species(int id, string name, int evolves_from_species_id, int evolution_chain_id, int color_id, int shape_id, int habitat_id, int gender_rate, int capture_rate, int base_happiness, int is_baby, int hatch_counter, int has_gender_differences, int growth_rate_id, int forms_switchable, int is_legendary, int is_mythical, int order, int conquest_order);
    Species(string line);
    void print();
};

class Experience {
public:
    int growth_rate_id;
    int level;
    int experience;

    Experience(int growth_rate_id, int level, int experience);
    Experience(string line);
    void print();
};

class TypeName {
public:
    int type_id;
    int local_language;
    string name;

    TypeName(int type_id, int local_language, string name);
    TypeName(string line);
    void print();
};

class PokemonStats {
public:
    int pokemon_id;
    int stat_id;
    int base_stat;
    int effort;

    PokemonStats(int pokemon_id, int stat_id, int base_stat, int effort);
    PokemonStats(string line);
    void print();
};

class Stats {
public:
    int id;
    int damage_class_id;
    string name;
    int is_battle_only;
    int game_index;

    Stats(int id, int damage_class_id, string name, int is_battle_only, int game_index);
    Stats(string line);
    void print();
};

class PokemonType {
public:
    int pokemon_id;
    int type_id;
    int slot;

    PokemonType(int pokemon_id, int type_id, int slot);
    PokemonType(string line);
    void print();
};

class Pokedex{
    public:
    vector<PokemonInfo> pokemonInfoList;
    vector<Move> moveList;
    vector<PokemonMove> pokemonMoveList;
    vector<Species> speciesList;
    vector<Experience> experienceList;
    vector<TypeName> typeNameList;
    vector<PokemonStats> pokemonStatsList;
    vector<Stats> statsList;
    vector<PokemonType> pokemonTypeList;

    unordered_map<int, Move*> moveTable;
    unordered_map<int, PokemonInfo*> pokemonInfoTable;

    Pokedex();
    // PokemonInfo* getRandPokemonInfo();

};

class Pokemon{

    public:
    //permanent
    int csvIndex;//the index the pokemon is read in from
    int gender;
    int shiny;
    PokemonInfo* info;
    int baseStats[6];

    //dynamic
    int level;
    vector<Move*> movesKnown;
    int currHP;
    int stats[6]; //the actual stats used in game/battle

    string toString();
    void levelUp();
    Pokemon(int csvInd, int manHattanDist);

    private:
    int IVStats[6];
    
    void setLevel(int lvl);
    void setInitialLevelNStats(int manhattanDist);
    void setBaseStats();
    void initializeMoves(int lvl);

};


extern Map* worldMap[401][401];
extern int adjVer[9];
extern int adjHor[9];

extern int playerW[NUMTERRAINTYPES];
extern int hikerW[NUMTERRAINTYPES];
extern int rivalW[NUMTERRAINTYPES];
extern int otherW[NUMTERRAINTYPES];
extern char terrTypes[NUMTERRAINTYPES];
extern int terrHeights[NUMTERRAINTYPES];

extern Node* hikerPMap[HEIGHT][WIDTH];
extern Node* rivalPMap[HEIGHT][WIDTH];

extern Player* PLAYER;
extern int quitGame;
extern stringstream DEBUGprint;
extern stringstream MESSAGEprint;
extern Pokedex* POKEDEX;

enum Color{
    BOUNDARY,
    TREE,
    ROAD,
    MART,
    CENTER,
    TALLGRASS,
    GRASS,
    MOUNTAIN,
    FOREST,
    WATER,
    GATE,
    SKY,
    ENEMY,
    TEXT_COLOR
};

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
int mainParseCSVs(string option);
vector<string> lineParser(const string &s, char splitter);
void printStringVector(vector<string> words);
vector<PokemonInfo> importPokemonList(istream &file);
void printPokemonList(vector<PokemonInfo>& pokedex);
vector<Move> importMoveList(istream& file);
void printMoveList(vector<Move>& moveList);
vector<PokemonMove> importPokemonMoveList(istream& file);
void printPokemonMovesList(vector<PokemonMove>& pokemonMovesList);
vector<Species> importSpeciesList(istream& file);
void printPokemonSpeciesList(vector<Species>& speciesList);
vector<Experience> importExperienceList(istream& file);
void printExperienceList(vector<Experience>& experienceList);
vector<TypeName> importTypeNameList(istream& file);
void printTypeNameList(vector<TypeName>& typeNameList);
vector<PokemonStats> importPokemonStatsList(istream& file);
void printPokemonStatsList(vector<PokemonStats>& pokemonStatsList);
vector<Stats> importStatsList(istream& file);
void printStatsList(vector<Stats>& statsList);
vector<PokemonType> importPokemonTypeList(istream& file);
void printPokemonTypeList(vector<PokemonType>& pokemonTypeList);


//8: pokemon mechanics
void wildPokemon_cutscene(Map* m);
void startPokemon_cutscene();
void printMessage(const char* s);

//10: raycasting
void initColors();

#endif
