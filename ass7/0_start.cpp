#include "0_pokemonH.h"

Map* worldMap[401][401] = {NULL};
int adjVer[9]={0,-1,-1,-1,0,1,1,1,0};
int adjHor[9]={1,1,0,-1,-1,-1,0,1,0};

int playerW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 10, 10, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, 10};
int hikerW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 15, 10, 15, 15, SHRT_MAX, SHRT_MAX};
int rivalW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, SHRT_MAX};
int otherW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, SHRT_MAX};

char terrTypes[NUMTERRAINTYPES]={'+','$','#','M','C',':','.','%','^','~','='};
int quitGame = 0;
char errorMsg[WIDTH*2];//TODO: make errorMessage functionality
Player* PLAYER = NULL;



// int main(int argc, char* argv[])
// {
    
//     csvRunner(argc, argv); //defined in 7_pokedex.cpp

//     return 0;

// }

