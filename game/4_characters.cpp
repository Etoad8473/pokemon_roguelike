#include "0_pokemonH.h"

/*
    TERMINAL START WITH --numtrainers SWITCH
*/
// int main(int argc, char* argv[])
// {

//     if (argc == 3 && strcmp(argv[1], "--numtrainers") == 0) 
//     {
//         int numTrainers = atoi(argv[2]);
//         if(numTrainers > 50 || numTrainers < -1)
//         {
//             printw("Invalid number of trainers (%d). Number must be between [0:50], or -1 for default.\nStopping.\n", numTrainers);
//             return 1;
//         }
//         npcRunner(numTrainers);
//     }
//     else if(argc == 1)
//     {
//         npcRunner(-1);
//     }
//     else
//     {
//         printw("Usage: %s --numPlayer <number>\n", argv[0]);
//         return 1;
//     }

// }


//OBSCELETE
// void npcRunner(int numNPC)
// {
//     if(numNPC == -1)
//         numNPC = DEFAULT_NUM_TRAINERS;
    
//     printw("Number of trainers: %d\n", numNPC);

//     //--SETUP--//
//     Heap* turnHeap = createHeap(8);                //create heap
//     srand(time(NULL));                                  //init Rand
//     initPathMap(hikerPMap);                             //init hiker&rival path Maps
//     initPathMap(rivalPMap);                             //^    
//     Map *m = createMapTile(200,200,-1,-1,-1,-1);   //init map
//     Character* pc = spawnPlayer(m,turnHeap);                    //spawn PC
//     dijkstra(m, hikerW, hikerPMap);                     //run dijkstra for hiker&rival
//     dijkstra(m, rivalW, rivalPMap);                     //^

//     //spawn in NPC's +give order
//         //add to queue with terrainWeight & order
    
//     spawnAllNPCs(numNPC,m,turnHeap);

//     // Sleep(1000);



//     //while heap!empty
//         //pop heap
//         //get character from XY
//         //move them
//         //update their turnCount
//         //push to heap
//         //print heap
//         //print map
//         //wait a second

//     while(!heapIsEmpty(turnHeap))
//     {
//         Node* node = extractMin(turnHeap);
//         Character* c = m->cMap[node->y][node->x];

//         c->moveFunct(m,c);
//         c->nextTurn = c->nextTurn + getTerrainWeight(c->weights,m,c->y,c->x);

//         node->currWeight = c->nextTurn;
//         node->y = c->y;
//         node->x = c->x;

//         heapInsert(node, turnHeap);
//     }

// }



void spawnAllNPCs(int num, Map* m)
{
    int centerY = HEIGHT/2;
    int centerX = WIDTH/2;

    m->npcList = (Character**)calloc(num, sizeof(Character*));
    m->numNPCs = num;

    if(num>=1)
    {
        spawnNPC('r',m,centerY,centerX);
        if(num>=2)
        {
            spawnNPC('h',m,centerY,centerX);
            if(num>2)
            {
                num -= 2;
                
                
                for(num; num>0; num--)
                {
                    spawnNPC(getRandNPCType(), m,centerY,centerX);
                }
            }
        }
    }
}

char getRandNPCType()
{
    const char* types = "hrpwse";
    return types[getRandLwSz(0,6)];
}

//TODO: bound input
Character* spawnCharacterHelper(char ch, Map* m, int y, int x, int order)
{
    //TODO: bound input

    //malloc
    Character *c = (Character*)malloc(sizeof(Character));

    //check if character is valid & set weight array
    switch(ch)
    {
        case 'a':
            c->weights = playerW;
            c->moveFunct = &playerMove;
            c->currDir = 0;
            break;
        case 'h':
            c->weights = hikerW;
            c->moveFunct = &hikerMove;
            c->currDir = 0;
            break;
        case 'r':
            c->weights = rivalW;
            c->moveFunct = &rivalMove;
            c->currDir = 0;
            break;
        case 'p':
            c->weights = otherW;
            c->moveFunct = &pacerMove;
            c->currDir = getRandMnMx(0,7);
            break;
        case 'w':
            c->weights = otherW;
            c->moveFunct = &wandererMove;
            c->currDir = getRandMnMx(0,7);
            break;
        case 's':
            c->weights = otherW;
            c->moveFunct = &sentryMove;
            c->currDir = getRandMnMx(0,7);
            break;
        case 'e':
            c->weights = otherW;
            c->moveFunct = &explorerMove;
            c->currDir = getRandMnMx(0,7);
            break;
        default:
            printw("spawnCharacterHelper() cancelled\n-invalid character char input\n");
            free(c);
            return NULL;
            
    }
    c->character = ch;

    int spawnAttempts = 0;
    //check if input is valid location, if not search for valid location to spawn, give up after a while
    while(getTerrainWeight(c->weights,m,y,x)>=SHRT_MAX || m->cMap[y][x])//while terrain is impassable, or someone is in that spot, keep searching
    {
        if(spawnAttempts >= NPC_SPAWN_ATTEMPTS)
        {
            printw("spawnCharacterHelper() cancelled\n-Could not find a suitable spawn location with %d attempts.\n", NPC_SPAWN_ATTEMPTS);
            free(c);
            return NULL;
        }
        y = getRandMnMx(1, HEIGHT-2);
        x = getRandMnMx(1, WIDTH-2);

        spawnAttempts++;
    }

    //set xy
    c->y = y; c->x = x;
    //add to map at xy
    m->cMap[y][x] = c;

    //initialize gameTurn
    c->nextTurn = 0;

    //set turnOrder
    c->turnOrder = order;

    //set to not defeated
    c->defeated = 0;

    
    c->pokeRoster = new vector<Pokemon>;

    m->turnHeap->heapInsert(createDNode(c->y,c->x,c->nextTurn,0));

    return c;

}



Character* spawnNPC(char ch, Map* m, int y, int x)
{
    //check if !char == 'a' & PLAYER exists
        //spawnCharacterHelper (order: nextCharacterOrder())

    if(ch != 'a')
    {
        Character* npc = spawnCharacterHelper(ch,m,y,x, nextCharacterOrder(m));
        m->npcList[(npc->turnOrder-1)] = npc;//add them to the maps list of NPCs

        while(npc->pokeRoster->size()<6)
        {
            npc->pokeRoster->push_back(Pokemon(-1, m->manhattan));
            if(probability(0.4)){break;}
        }

        return npc;
    }
    else
    {
        printw("spawnNPC() cancelled\ntried to create player\n");
        return NULL;
    }
}

Node* createPlayerNode(int y, int x, int weight, int playerOrder)
{
    Node* d = (Node*)malloc(sizeof(Node));
    d->heapPos = -1;
    d->currWeight = weight;
    d->y = y;
    d->x = x;
    d->order = playerOrder;

    return d;
}

//Direction: 0 is looking east, 2 is north, etc
//a rot of "1" adds 45 degrees counterclockwise
int rotateDir(int dir, int rot)
{
    return ((dir+rot)%8);
}

void updateCPosition(Map *m, Character *c, int nY, int nX)
{
    if(m->cMap[nY][nX])
    {
        printw("updateCPosition() - cancelled\n-Tried to enter a taken position.\n");
        return;
    }

    //null previous map reference
    m->cMap[c->y][c->x] = NULL;

    //new map reference position
    m->cMap[nY][nX] = c;

    //new internal position
    c->y = nY;
    c->x = nX;

    //update dijkstra if player
    if(cIsPlayer(c))
    {
        dijkstra(m,hikerW,hikerPMap);
        dijkstra(m,rivalW,rivalPMap);
    }

}

int cIsPlayer(Character* c)
{
    if(c == PLAYER)
        {return 1;}
    else if(c->character == 'a')
    {
        printw("cIsPlayer() Warning\n- character is the player but doesn't reference the global player address\n");
        return 1;
    }
    else
        {return 0;}
}

int playerExists()
{
    if(PLAYER)
        return 1;
    return 0;
}

//TODO: fix segmentation fault bug
Map* sentryMove(Map* m, Character* c)
{
    if(c->defeated){
        return m;
    }

    /*
    loop through all directions
        if it is the player
            turn that dir
            init pokemon battle
    */

    //BUG: Creates segmentation fault
    int i;
    for(i = 0; i < 8; i++)
    {
        int y = c->y + adjVer[i];
        int x = c->x + adjHor[i];

        if(m->cMap[y][x])
        {
            if(cIsPlayer(m->cMap[y][x]))
            {
                c->currDir = i;
                pokemonBattle_cutscene(m,c);
                break;
            }
        }
    }

        return m;


}

Map* pacerMove(Map* m, Character* c)
{
    if(c->defeated){
        return m;
    }
    //get nextPos from direction
    int nextY = c->y + adjVer[c->currDir];
    int nextX = c->x + adjHor[c->currDir];


    //check weights, if impassable
        //180 direction, re-get nextPos
    int nextWeight = getTerrainWeight(c->weights, m, nextY, nextX);
    if(nextWeight>=SHRT_MAX)
    {
        c->currDir = rotateDir(c->currDir, 4);
        nextY = c->y + adjVer[c->currDir];
        nextX = c->x + adjHor[c->currDir];
    }

    //if someone's there, do nothing
    if(m->cMap[nextY][nextX])
    {
        //dont move
        //if it is the player, initiate pokemon battle
        if(cIsPlayer(m->cMap[nextY][nextX]))
        {
            pokemonBattle_cutscene(m,c);
        }

    }
    //else
    else
    {
        updateCPosition(m,c,nextY,nextX);
    }

        return m;

}

Map* explorerMove(Map* m, Character* c)
{
    if(c->defeated){
        return m;
    }

    //get nextPos from direction
    int nextY = c->y + adjVer[c->currDir];
    int nextX = c->x + adjHor[c->currDir];

    /*
    check weights, if impassable
        random direction, re-get nextPos, check if walkable
    */
    int nextWeight = getTerrainWeight(c->weights, m, nextY, nextX);
    if(nextWeight>=SHRT_MAX)
    {
        c->currDir = rotateDir(c->currDir, getRandMnMx(0,7));
        nextY = c->y + adjVer[c->currDir];
        nextX = c->x + adjHor[c->currDir];

        while(getTerrainWeight(c->weights, m, nextY, nextX) >= SHRT_MAX)//if the next position cant be reached, keep trying new positions till you get a valid one
        {
            c->currDir = rotateDir(c->currDir, getRandMnMx(0,7));
            nextY = c->y + adjVer[c->currDir];
            nextX = c->x + adjHor[c->currDir];
        }
    }

    //if someone's there, do nothing
    if(m->cMap[nextY][nextX])
    {
        //dont move
        //if it is the player, initiate pokemon battle
        if(cIsPlayer(m->cMap[nextY][nextX]))
        {
            pokemonBattle_cutscene(m,c);
        }

    }
    else //if the space is not occupied
    {
        updateCPosition(m,c,nextY,nextX);
    }


        return m;

}

Map* wandererMove(Map* m, Character* c)
{
    if(c->defeated){
        return m;
    }
    
    //get nextPos from direction
    int nextY = c->y + adjVer[c->currDir];
    int nextX = c->x + adjHor[c->currDir];
    char prevTerr = m->map[c->y][c->x];

    //if impassable or diffTerrain
        //random direction, re-get nextPos, check if walkable&&same terr
    int nextWeight = getTerrainWeight(c->weights, m, nextY, nextX);
    if(nextWeight>=SHRT_MAX || m->map[nextY][nextX] != prevTerr)
    {
        c->currDir = rotateDir(c->currDir, getRandMnMx(0,7));
        nextY = c->y + adjVer[c->currDir];
        nextX = c->x + adjHor[c->currDir];

        while(getTerrainWeight(c->weights, m, nextY, nextX) >= SHRT_MAX || m->map[nextY][nextX] != prevTerr)//if the next position cant be reached, keep trying new positions till you get a valid one
        {
            c->currDir = rotateDir(c->currDir, getRandMnMx(0,7));
            nextY = c->y + adjVer[c->currDir];
            nextX = c->x + adjHor[c->currDir];
        }
    }

    //if someone's there, do nothing
    if(m->cMap[nextY][nextX])
    {
        //dont move
        //if it is the player, initiate pokemon battle
        if(cIsPlayer(m->cMap[nextY][nextX]))
        {
            pokemonBattle_cutscene(m,c);
        }

    }
    //else
        //updatePos
    else
    {
        updateCPosition(m,c,nextY,nextX);
    }    

        return m;

}

Map* hikerMove(Map* m, Character* c)
{
    if(c->defeated){
        return m;
    }
    
    //check hikerMap
        //get parentNode's position
    Node* currN = hikerPMap[c->y][c->x];

    if(!currN)
        {return m;}
    
    Node* parentN = currN->parent;
    //check if parentNull
    if(parentN)//if not null
    {
        //if spot is empty
        //update Pos?
        int nextY = parentN->y;
        int nextX = parentN->x;
        if(!m->cMap[nextY][nextX])//if not filled
        {
            updateCPosition(m,c,nextY,nextX);
        }
        else
        {
            //do nothing if someone is in the spot
            
            //if it is the player, initiate pokemon battle
            if(cIsPlayer(m->cMap[nextY][nextX]))
            {
                pokemonBattle_cutscene(m,c);
            }

        }
    }

        return m;

}

Map* rivalMove(Map* m, Character* c)
{
    if(c->defeated){
        return m;
    }
    
    //check hikerMap
        //get parentNode's position
    Node* currN = rivalPMap[c->y][c->x];

    if(!currN)
        {return m;}
    
    Node* parentN = currN->parent;
    //check if parentNull
    if(parentN)//if not null
    {
        //if spot is empty
        //update Pos?
        int nextY = parentN->y;
        int nextX = parentN->x;
        if(!m->cMap[nextY][nextX])//if not filled
        {
            updateCPosition(m,c,nextY,nextX);
        }
        else
        {
            //do nothing if someone is in the spot

            //if it is the player, initiate pokemon battle
            if(cIsPlayer(m->cMap[nextY][nextX]))
            {
                pokemonBattle_cutscene(m,c);
            }
        }
    }

        return m;

}

//TODO: for future assignments allow gates to be moved through
Map* playerMove(Map* m, Character* c)
{
    printMap(m);//only print when the player moves

    m->turn = c->nextTurn; //remember what turn it is between maps

    Map* prevM = m;
    m = keyboardInput(m,c);
    if(m != prevM){return m;}//if we changed maps, return
        
    
    //get next XY from the characterDir
    int nextY = c->y + adjVer[c->currDir];
    int nextX = c->x + adjHor[c->currDir];    
    //check if it's passable and not an NPC
    int nextWeight = getTerrainWeight(c->weights, m, nextY, nextX);



    if(m->map[nextY][nextX] != '=')
    {
        //check for wildPokemon
        if(m->map[c->y][c->x]==':')
        {
            if(probability(0.2)){
                wildPokemon_cutscene(m);
            }
        }
        if(nextWeight<SHRT_MAX)//if passable
        {
            //if someone's there (and not yourself), battle
            if(m->cMap[nextY][nextX])
            {
                Character* npc = m->cMap[nextY][nextX];
                if(!cIsPlayer(npc) && !npc->defeated)
                {
                    pokemonBattle_cutscene(m, npc);
                }
            }
            else  //updatePos
            {
                updateCPosition(m,c,nextY,nextX);
            }
        }
    }
    else //moving through gate
    {
        /*
        figure out direction by which gate it is
        fly to next map
        set player position
        */

        int nextGlobalY = m->worldY;
        int nextGlobalX = m->worldX;

        char newGate;

        if(nextX == WIDTH-1)//east gate
        {
            nextGlobalX++;
            newGate = 'w';
        }
        else if(nextY==0)//north gate
        {
            nextGlobalY--;
            newGate = 's';
        }
        else if(nextX == 0)//west gate
        {
            nextGlobalX--;
            newGate = 'e';
        }
        else if(nextY == HEIGHT-1)//south gate
        {
            nextGlobalY++;
            newGate = 'n';
        }

        m = flyToTile(m, nextGlobalY, nextGlobalX, newGate); //if next map doesn't exist: createMap(), moveThroughGate()

    }

    return m;
    
}



int nextCharacterOrder(Map *m)
{
    return m->characterOrder++;
}


