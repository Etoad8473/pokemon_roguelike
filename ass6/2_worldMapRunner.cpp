#include "0_pokemonH.h"

// int main() 
// {
//     srand(time(NULL));

//     runner();

//     // tMapTile *m = createMapTile(200,200,-1,-1,-1,-1);
//     // printMap(m);

//     return 0;
// }

// void runner(int numNPCs)
// {
//     char inp;
//     //char *errorMsg = malloc(60);
//     //starter mapTile
//     Map *currMap = createMapTile(200,200,-1,-1,-1,-1, numNPCs);
//     //printMap(currMap);

//     //start getting input
//     while(inp != 'q')
//     {
//         printMap(currMap);
//         //printw("%s", errorMsg);
//         printw("World Coordinates- X:%d Y:%d (index y%d,x%d) Enter a letter: ", currMap->worldX - 200, 200 - currMap->worldY, currMap->worldY, currMap->worldX);
//         scanf(" %c", &inp);

//         int currY = currMap->worldY;
//         int currX = currMap->worldX;
//         int prevY = currY - 1;
//         int prevX = currX - 1;
//         int nextY = currY + 1;
//         int nextX = currX + 1;

//         switch(inp) {
//             case 'n': 
//                 // Move north
//                 if(prevY < 0) {
//                     printw("You're at the northern border, cannot go further north\n");
//                 } else {
//                     currMap = flyToTile(currMap, prevY, currX);
//                 }
//                 break;
//             case 's': 
//                 // Move south
//                 if (nextY >= WORLDSIZE) {
//                     printw("You're at the southern border, cannot go further south\n");
//                 } else {
//                     currMap = flyToTile(currMap, nextY, currX);
//                 }
//                 break;
//             case 'e': 
//                 // Move east
//                 if (nextX >= WORLDSIZE) {
//                     printw("You're at the eastern border, cannot go further east\n");
//                 } else {
//                     currMap = flyToTile(currMap, currY, nextX);
//                 }
//                 break;
//             case 'w': 
//                 // Move west
//                 if (prevX < 0) {
//                     printw("You're at the western border, cannot go further west\n");
//                 } else {
//                     currMap = flyToTile(currMap, currY, prevX);
//                 }
//                 break;
//             case 'f':
//                 scanf(" %d %d", &nextX, &nextY);
//                 nextX += 200; nextY = 200 - nextY;
//                 if(nextX < 0 || nextX >= WORLDSIZE || nextY < 0 || nextY >= WORLDSIZE){
//                 printw("INVALID INPUT B***H (im sorry for cursing at you)\n");
//                 } else {
//                     currMap = flyToTile(currMap, nextY,nextX);
//                 }
//                 break;
//             case 'q': 
//                 printw("QUITTING");
//                 break;
//             default: 
//                 printw("Invalid input.\n");
//         }

//     }    

// }


//updates reference to 
Map* flyToTile(Map* prevM, int y, int x, char newGate)//coordinates
{
    if(x < 0 || x >= WORLDSIZE || y < 0 || y >= WORLDSIZE)
    {
        printw("EzWarning: out of bounds input parameters passed to flyToTile function\n");
        return NULL;
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
        createMapTile(y,x,n,s,e,w, prevM->numNPCs);
    }

    
    // sendPlayerToNewMap(currMap, worldMap[y][x]);    
    /*
    (player has not been moved yet)
    (map is created by flyToTile if it doesnt exist, that adds player to newMap turnHeap)
    NULL old map's reference to player
    add player to new map's character reference at pos
    update player's internal pos reference
    updatePlayer's gameTurn
    update dijk
    add player to turnheap at map's currGameTurn
    */ 

    Player* pc = PLAYER;
    Map* destM = worldMap[y][x];

    prevM->cMap[pc->y][pc->x] = NULL;
    int pY, pX;
    switch(newGate){
        case 'e':
            pY = destM->e;
            pX = WIDTH-2;
            break;
        case 'n':
            pY = 1;
            pX = destM->n;
            break;
        case 'w':
            pY = destM->w;
            pX = 1;
            break;
        case 's':
            pY = HEIGHT-2;
            pX = destM->s;
            break;
        default:
            printw("err: invalid gate arg in flyToTile");
    }
    destM->cMap[pY][pX] = pc;
    pc->y = pY;
    pc->x = pX;
    pc->nextTurn = destM->turn;
    dijkstra(destM,hikerW,hikerPMap);
    dijkstra(destM,rivalW,rivalPMap);
    destM->turnHeap->heapInsert(createDNode(pc->y,pc->x,pc->nextTurn,0));

    return destM;

}



void sendPlayerToNewMap(Map* oldM, Map* newM)
{
    Player* pc = PLAYER;

    oldM->cMap[pc->y][pc->x] = NULL;


}