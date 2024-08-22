#include "0_pokemonH.h"

Map* worldMap[401][401] = {NULL};
int adjVer[9]={0,-1,-1,-1,0,1,1,1,0};
int adjHor[9]={1,1,0,-1,-1,-1,0,1,0};

int playerW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 10, 10, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, 10};
int hikerW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 15, 10, 15, 15, SHRT_MAX, SHRT_MAX};
int rivalW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, SHRT_MAX};
int otherW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, SHRT_MAX};

char terrTypes[NUMTERRAINTYPES]={'+','$','#','M','C',':','.','%','^','~','='};
int terrHeights[NUMTERRAINTYPES]={1 , 2 , 0 , 3 , 2 , 1 , 0 , 4 , 3 , 0 , 1 };

int quitGame = 0;
Player* PLAYER = NULL;
Screen* GAMESCREEN = nullptr;
stringstream DEBUGprint = stringstream();
stringstream MESSAGEprint = stringstream();

Node* hikerPMap[HEIGHT][WIDTH] = {NULL};
Node* rivalPMap[HEIGHT][WIDTH] = {NULL};



#define COLOR_BROWN 8
#define COLOR_DARKGREEN 9
#define COLOR_GREY 10
#define COLOR_SKYBLUE 11
#define COLOR_BRIGHTYELLOW 12

void initColors()
{
    start_color();

    init_color(COLOR_BROWN, 700, 400, 0);
    init_color(COLOR_DARKGREEN, 0, 200, 0);
    init_color(COLOR_GREY, 700, 700, 700);
    init_color(COLOR_SKYBLUE, 400, 800, 1000);
    init_color(COLOR_BRIGHTYELLOW, 0, 800, 800);

    int background = COLOR_WHITE;

    init_pair(BOUNDARY, COLOR_CYAN, COLOR_SKYBLUE);
    init_pair(TREE, COLOR_DARKGREEN, COLOR_GREEN);
    init_pair(ROAD, COLOR_BRIGHTYELLOW, COLOR_BROWN);
    init_pair(MART, COLOR_CYAN, background);
    init_pair(CENTER, COLOR_MAGENTA, background);
    init_pair(TALLGRASS, COLOR_BRIGHTYELLOW, COLOR_GREEN);
    init_pair(GRASS, COLOR_BRIGHTYELLOW, COLOR_GREEN);
    init_pair(MOUNTAIN, COLOR_BLACK, COLOR_BROWN);
    init_pair(FOREST, COLOR_GREEN, COLOR_DARKGREEN);
    init_pair(WATER, COLOR_CYAN, COLOR_BLUE);
    init_pair(GATE, COLOR_BRIGHTYELLOW, COLOR_BROWN);
    init_pair(SKY, COLOR_CYAN, COLOR_SKYBLUE);
    init_pair(ENEMY, COLOR_BRIGHTYELLOW, COLOR_RED);
    init_pair(TEXT_COLOR, COLOR_WHITE, COLOR_BLACK);

}

int main(int argc, char* argv[])
{
    initscr();
    noecho();
    keypad(stdscr, TRUE); 
    start_color();
    initColors();

    //srand(time(NULL));                                  //init Rand


    gameRunner(5); //defined in 5


    echo();//dont forget to turn it back on
    endwin();
    return 0;
}



