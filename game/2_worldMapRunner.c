#include "pokemonH.h"


// int main() 
// {
//     srand(time(NULL));

//     runner();

//     // tMapTile *m = createMapTile(200,200,-1,-1,-1,-1);
//     // printMap(m);

//     return 0;
// }

void runner()
{
    char inp;
    //char *errorMsg = malloc(60);
    //starter mapTile
    tMapTile *currMap = createMapTile(200,200,-1,-1,-1,-1);
    //printMap(currMap);

    //start getting input
    while(inp != 'q')
    {
        printMap(currMap);
        //printw("%s", errorMsg);
        printw("World Coordinates- X:%d Y:%d (index y%d,x%d) Enter a letter: ", currMap->worldX - 200, 200 - currMap->worldY, currMap->worldY, currMap->worldX);
        scanf(" %c", &inp);

        int currY = currMap->worldY;
        int currX = currMap->worldX;
        int prevY = currY - 1;
        int prevX = currX - 1;
        int nextY = currY + 1;
        int nextX = currX + 1;

        switch(inp) {
            case 'n': 
                // Move north
                if(prevY < 0) {
                    printw("You're at the northern border, cannot go further north\n");
                } else {
                    flyToTile(&currMap, prevY, currX);
                }
                break;
            case 's': 
                // Move south
                if (nextY >= WORLDSIZE) {
                    printw("You're at the southern border, cannot go further south\n");
                } else {
                    flyToTile(&currMap, nextY, currX);
                }
                break;
            case 'e': 
                // Move east
                if (nextX >= WORLDSIZE) {
                    printw("You're at the eastern border, cannot go further east\n");
                } else {
                    flyToTile(&currMap, currY, nextX);
                }
                break;
            case 'w': 
                // Move west
                if (prevX < 0) {
                    printw("You're at the western border, cannot go further west\n");
                } else {
                    flyToTile(&currMap, currY, prevX);
                }
                break;
            case 'f':
                scanf(" %d %d", &nextX, &nextY);
                nextX += 200; nextY = 200 - nextY;
                if(nextX < 0 || nextX >= WORLDSIZE || nextY < 0 || nextY >= WORLDSIZE){
                printw("INVALID INPUT B***H (im sorry for cursing at you)\n");
                } else {
                    flyToTile(&currMap, nextY,nextX);
                }
                break;
            case 'q': 
                printw("QUITTING");
                break;
            default: 
                printw("Invalid input.\n");
        }

    }    

}

