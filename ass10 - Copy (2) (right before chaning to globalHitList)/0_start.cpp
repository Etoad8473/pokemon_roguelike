#include "0_pokemonH.h"

Map* worldMap[401][401] = {NULL};
int adjVer[9]={0,-1,-1,-1,0,1,1,1,0};
int adjHor[9]={1,1,0,-1,-1,-1,0,1,0};

int playerW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 10, 10, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, 10};
int hikerW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 15, 10, 15, 15, SHRT_MAX, SHRT_MAX};
int rivalW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, SHRT_MAX};
int otherW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, SHRT_MAX};

char terrTypes[NUMTERRAINTYPES]={'+','$','#','M','C',':','.','%','^','~','='};
int terrHeights[NUMTERRAINTYPES]={3 , 3 , 0 , 4 , 4 , 1 , 0 , 2 , 3 , 0 , 2 };

int quitGame = 0;
Player* PLAYER = NULL;
stringstream DEBUGprint = stringstream();
stringstream MESSAGEprint = stringstream();

Node* hikerPMap[HEIGHT][WIDTH] = {NULL};
Node* rivalPMap[HEIGHT][WIDTH] = {NULL};



#define COLOR_BROWN 8
#define COLOR_DARKGREEN 9
#define COLOR_GREY 10

void initColors()
{
    start_color();

    init_color(COLOR_BROWN, 600, 300, 0);
    init_color(COLOR_DARKGREEN, 50, 300, 50);
    init_color(COLOR_GREY, 500, 500, 500);

    int background = COLOR_WHITE;

    init_pair(BOUNDARY, COLOR_BROWN, background);
    init_pair(TREE, COLOR_DARKGREEN, background);
    init_pair(ROAD, COLOR_YELLOW, background);
    init_pair(MART, COLOR_MAGENTA, background);
    init_pair(CENTER, COLOR_MAGENTA, background);
    init_pair(TALLGRASS, COLOR_GREEN, background);
    init_pair(GRASS, COLOR_GREEN, background);
    init_pair(MOUNTAIN, COLOR_BROWN, background);
    init_pair(FOREST, COLOR_DARKGREEN, background);
    init_pair(WATER, COLOR_BLUE, background);
    init_pair(GATE, COLOR_CYAN, background);
    init_pair(SKY, COLOR_CYAN, background);
    init_pair(ENEMY, COLOR_RED, background);
    init_pair(TEXT_COLOR, COLOR_WHITE, COLOR_BLACK);

}

// int main(int argc, char* argv[])
// {
    // initscr();
    // noecho();
    // keypad(stdscr, TRUE); 
    // start_color();

    // //srand(time(NULL));                                  //init Rand


//     gameRunner(5); //defined in 5


    // echo();//dont forget to turn it back on
    // endwin();
//     return 0;
// }



