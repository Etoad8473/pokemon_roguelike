#include "pokemonH.h"

tMapTile* worldMap[401][401] = {NULL};

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
        printMap(currMap,(int)NULL,(int)NULL);
        //printf("%s", errorMsg);
        printf("World Coordinates- X:%d Y:%d (index y%d,x%d) Enter a letter: ", currMap->worldX - 200, 200 - currMap->worldY, currMap->worldY, currMap->worldX);
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
                    printf("You're at the northern border, cannot go further north\n");
                } else {
                    flyToTile(&currMap, prevY, currX);
                }
                break;
            case 's': 
                // Move south
                if (nextY >= WORLDSIZE) {
                    printf("You're at the southern border, cannot go further south\n");
                } else {
                    flyToTile(&currMap, nextY, currX);
                }
                break;
            case 'e': 
                // Move east
                if (nextX >= WORLDSIZE) {
                    printf("You're at the eastern border, cannot go further east\n");
                } else {
                    flyToTile(&currMap, currY, nextX);
                }
                break;
            case 'w': 
                // Move west
                if (prevX < 0) {
                    printf("You're at the western border, cannot go further west\n");
                } else {
                    flyToTile(&currMap, currY, prevX);
                }
                break;
            case 'f':
                scanf(" %d %d", &nextX, &nextY);
                nextX += 200; nextY = 200 - nextY;
                if(nextX < 0 || nextX >= WORLDSIZE || nextY < 0 || nextY >= WORLDSIZE){
                printf("INVALID INPUT B***H (im sorry for cursing at you)\n");
                } else {
                    flyToTile(&currMap, nextY,nextX);
                }
                break;
            case 'q': 
                printf("QUITTING");
                break;
            default: 
                printf("Invalid input.\n");
        }

    }    

}

//TODO: BOUND INPUT
//this only updates currMap
void flyToTile(tMapTile **currMap, int y, int x)//coordinates
{
    if(x < 0 || x >= WORLDSIZE || y < 0 || y >= WORLDSIZE)
    {
        printf("EzWarning: out of bounds input parameters passed to flyToTile function\n");
        return;
    }

    //check if already exists
    if(worldMap[y][x] == NULL)//if it doesnt exist, create it
    {
        //check all adjacent tiles
        //if they exist capture their corresponding gate

        //TODO: put all of these in if statements for their respective borders (eg if x = 401, dont check for w gate)
        int n = -1;
        int s = -1;
        int e = -1;
        int w = -1;
        //get top adjacent
        if(y <= 0)
        {}
        else//DONT CHECK IF IT WILL BE OUT OF BOUNDS
        {
            if(worldMap[y-1][x] != NULL)
                n = worldMap[y-1][x]->s;
        }
        //bottom
        if(y >= WORLDSIZE-1)
        {}
        else
        {
            if(worldMap[y+1][x] != NULL)
                s = worldMap[y+1][x]->n;
        }
        //right
        if(x >= WORLDSIZE-1)
        {}
        else
        {
            if(worldMap[y][x+1] != NULL)
                e = worldMap[y][x+1]->w;
        }
        //left
        if(x <= 0)
        {}
        else
        {
            if(worldMap[y][x-1] != NULL)
                w = worldMap[y][x-1]->e;
        }

        //generate new map from their gates
        tMapTile m;
        createMapTile(y,x,n,s,e,w);
    }     

    *currMap = worldMap[y][x];

}

tMapTile* createMapTile(int y, int x, int n, int s, int e, int w)
{
    //TODO: bound input
    tMapTile *m = (tMapTile*)malloc(sizeof(tMapTile));
    linkMapToWorld(m, y,x);
    fillMap(m, n,s,e,w);

    return m;
}


void linkMapToWorld(tMapTile *m, int y, int x)
{
    m->worldY = y;
    m->worldX = x;

    if(worldMap[y][x] == NULL)
        {
            worldMap[y][x] = m; //update worldmap
        }
    else
        {
            printf("TRYING TO WRITE OVER PREVIOUSLY EXISTING TILE in variable 'WORLDMAP' with function 'CREATEMAP'");
        }
}


// void printWorld() {
//     for (int i = 0; i < 401; i++) {
//         for (int j = 0; j < 401; j++) {
//             char print = '.';

//             if(worldMap[i][j] != NULL)
//             { 
//                 print = '%';
//             }

//             printf("%c", print);
//         }
//         printf("\n");
//     }
// }