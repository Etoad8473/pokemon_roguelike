#include "pokemonH.h"

//TODO: clean up by delaring in header and delete duplicate instantiations between C files
//eg (header): int playerW[11]
tMapTile* worldMap[401][401] = {NULL};
int adjVer[9]={0,-1,-1,-1,0,1,1,1,0};
int adjHor[9]={1,1,0,-1,-1,-1,0,1,0};

int playerW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 10, 10, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, 10};
int hikerW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 15, 10, 15, 15, SHRT_MAX, SHRT_MAX};
int rivalW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, SHRT_MAX};
int otherW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, SHRT_MAX};

char terrTypes[NUMTERRAINTYPES]={'+','$','#','M','C',':','.','%','^','~','='};
int quitGame = 0;
char errorMsg[WIDTH];
characterT* player = NULL;





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







//NEW STUFF


//OBSCELETE
void npcRunner(int numNPC)
{
    if(numNPC == -1)
        numNPC = DEFAULT_NUM_TRAINERS;
    
    printw("Number of trainers: %d\n", numNPC);

    //--SETUP--//
    heap_t* turnHeap = createHeap(8);                //create heap
    srand(time(NULL));                                  //init Rand
    initPathMap(hikerPMap);                             //init hiker&rival path Maps
    initPathMap(rivalPMap);                             //^    
    tMapTile *m = createMapTile(200,200,-1,-1,-1,-1);   //init map
    characterT* pc = spawnPlayer(m,turnHeap);                    //spawn PC
    dijkstra(m, hikerW, hikerPMap);                     //run dijkstra for hiker&rival
    dijkstra(m, rivalW, rivalPMap);                     //^

    //spawn in NPC's +give order
        //add to queue with terrainWeight & order
    
    spawnAllNPCs(numNPC,m,turnHeap);

    // Sleep(1000);



    //while heap!empty
        //pop heap
        //get character from XY
        //move them
        //update their turnCount
        //push to heap
        //print heap
        //print map
        //wait a second

    while(!heapIsEmpty(turnHeap))
    {
        dNode_t* node = extractMin(turnHeap);
        characterT* c = m->cMap[node->y][node->x];

        c->moveFunct(m,c);
        c->nextTurn = c->nextTurn + getTerrainWeight(c->weights,m,c->y,c->x);

        node->currWeight = c->nextTurn;
        node->y = c->y;
        node->x = c->x;

        heapInsert(node, turnHeap);
        //printHeap(turnHeap);
        //printMap(m);
        //Sleep(500);
    }

}

void spawnAllNPCs(int num, tMapTile* m, heap_t* h)
{
    int centerY = HEIGHT/2;
    int centerX = WIDTH/2;

    m->npcList = (characterT**)calloc(num, sizeof(characterT*));
    m->numNPCs = num;

    if(num>=1)
    {
        spawnNPC('r',m,h,centerY,centerX);
        if(num>=2)
        {
            spawnNPC('h',m,h,centerY,centerX);
            if(num>2)
            {
                num -= 2;
                
                
                for(num; num>0; num--)
                {
                    spawnNPC(getRandNPCType(), m,h,centerY,centerX);
                }
            }
        }
    }
}

char getRandNPCType()
{
    char* types = "hrpwse";
    return types[getRandLwSz(0,6)];
}

//TODO: bound input
characterT* spawnCharacterHelper(char ch, tMapTile* m, heap_t* h, int y, int x, int order)
{
    //TODO: bound input

    //malloc
    characterT *c = (characterT*)malloc(sizeof(characterT));

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

    heapInsert(createDNode(c->y,c->x,c->nextTurn,0),h);

    return c;

}


characterT* spawnPlayer(tMapTile* m, heap_t* h)
{
    //check if global is null    
    if(!player)//if null make player
    {
    
        //spawnCharacter (order: 0)
        characterT* pc = spawnCharacterHelper('a',m, h,m->w,1,0);
        player = pc;        //set global reference
        return pc;

    }
    //else
        //error msg
    else
    {
        printw("spawnPlayer() cancelled\n-PC already exists\n");
        return NULL;
    }
}

characterT* spawnNPC(char ch, tMapTile* m, heap_t* h, int y, int x)
{
    //check if !char == 'a' & player exists
        //spawnCharacterHelper (order: nextCharacterOrder())

    if(playerExists() && ch != 'a')
    {
        characterT* npc = spawnCharacterHelper(ch,m,h,y,x, nextCharacterOrder(m));
        m->npcList[(npc->turnOrder-1)] = npc;
        return npc;
    }
    else
    {
        printw("spawnNPC() cancelled\n-player DNE or tried to create player\n");
        return NULL;
    }
}

dNode_t* createPlayerNode(int y, int x, int weight, int playerOrder)
{
    dNode_t* d = (dNode_t*)malloc(sizeof(dNode_t));
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

void updateCPosition(tMapTile *m, characterT *c, int nY, int nX)
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

int cIsPlayer(characterT* c)
{
    if(&c == &player)
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
    if(player)
        return 1;
    return 0;
}

//TODO: fix segmentation fault bug
void sentryMove(tMapTile* m, characterT* c)
{
    if(c->defeated){
        return;
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
                pokemonBattle(m,c);
                break;
            }
        }
    }

}

void pacerMove(tMapTile* m, characterT* c)
{
    if(c->defeated){
        return;
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
            pokemonBattle(m,c);
        }

    }
    //else
    else
    {
        updateCPosition(m,c,nextY,nextX);
    }
}

void explorerMove(tMapTile* m, characterT* c)
{
    if(c->defeated){
        return;
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
            pokemonBattle(m,c);
        }

    }
    else //if the space is not occupied
    {
        updateCPosition(m,c,nextY,nextX);
    }
}

void wandererMove(tMapTile* m, characterT* c)
{
    if(c->defeated){
        return;
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
            pokemonBattle(m,c);
        }

    }
    //else
        //updatePos
    else
    {
        updateCPosition(m,c,nextY,nextX);
    }    
}

void hikerMove(tMapTile* m, characterT* c)
{
    if(c->defeated){
        return;
    }
    
    //check hikerMap
        //get parentNode's position
    dNode_t* currN = hikerPMap[c->y][c->x];

    if(!currN)
        {return;}
    
    dNode_t* parentN = currN->parent;
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
                pokemonBattle(m,c);
            }

        }
    }
}

void rivalMove(tMapTile* m, characterT* c)
{
    if(c->defeated){
        return;
    }
    
    //check hikerMap
        //get parentNode's position
    dNode_t* currN = rivalPMap[c->y][c->x];

    if(!currN)
        {return;}
    
    dNode_t* parentN = currN->parent;
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
                pokemonBattle(m,c);
            }
        }
    }
}

//TODO: for future assignments allow gates to be moved through
void playerMove(tMapTile* m, characterT* c)
{
    // setMessage("none");
    strcpy(errorMsg, "no message");

    keyboardInput(m,c);


    //get next XY from the characterDir
    int nextY = c->y + adjVer[c->currDir];
    int nextX = c->x + adjHor[c->currDir];    
    //check if it's passable and not an NPC
    int nextWeight = getTerrainWeight(c->weights, m, nextY, nextX);
    if(m->map[nextY][nextX] != '=')    //CURRENTLY NOT ABLE TO MOVE THROUGH GATES
    {
        if(nextWeight<SHRT_MAX)//if passable
        {
            //if someone's there (and not yourself), battle
            if(m->cMap[nextY][nextX])
            {
                characterT* npc = m->cMap[nextY][nextX];
                if(!cIsPlayer(npc) && !npc->defeated)
                {
                    pokemonBattle(m, npc);
                }
            }
            else  //updatePos
            {
                updateCPosition(m,c,nextY,nextX);
            }
        }
    }
    
    printMap(m);//only print when the player moves
}



void printCharacter(characterT* c)
{
    if(!c)
    {
        printw("printCharacter() cancelled\n-Tried to print null character reference.\n");
        return;
    }

    printw("CHARACTER: %c\n",c->character);
    printw("direction:%d\n",c->currDir);
    printw("last turn: %d\n",c->nextTurn);
    printw("y: %d, x: %d\n",c->y, c->x);
    printw("turn order: %d\n\n",c->turnOrder);
}

void printCMap(tMapTile* m)
{
    printw("\n\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if(m->cMap[i][j])//if not null
                printw("%c", m->cMap[i][j]->character);
            else
                printw("-");
        }
        printw("\n");
    }
    printw("\n\n");
}


int nextCharacterOrder(tMapTile *m)
{
    return m->characterOrder++;
}







//NEW STUFF











void flyToTile(tMapTile **currMap, int y, int x)//coordinates
{
    if(x < 0 || x >= WORLDSIZE || y < 0 || y >= WORLDSIZE)
    {
        printw("EzWarning: out of bounds input parameters passed to flyToTile function\n");
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

    //TODO: set cMap to 0
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            m->cMap[i][j]=(characterT*)NULL;
        }
    }
    m->characterOrder = 1;//start at 1 because every map starts w the player

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
            printw("TRYING TO WRITE OVER PREVIOUSLY EXISTING TILE in variable 'WORLDMAP' with function 'CREATEMAP'");
        }
}

void fillMap(tMapTile *m, int n, int s, int e, int w)
{
    //if on border,, close certain gates

    drawRect(m, '.', 0, 0, HEIGHT - 1, WIDTH - 1);
    drawRect(m, ':', getRandMnMx(0, HEIGHT - 1), getRandMnMx(0, WIDTH - 1), getRandMnMx(0, HEIGHT - 1), getRandMnMx(0, WIDTH - 1));
    sprinkleDraw(m, ':', getRandMnMx(0, 20));
    drawRect(m, '~', getRandMnMx(0, HEIGHT - 1), getRandMnMx(0, WIDTH - 1), getRandMnMx(0, HEIGHT - 1), getRandMnMx(0, WIDTH - 1));
    drawRect(m, '%', getRandMnMx(0, HEIGHT - 1), getRandMnMx(0, WIDTH - 1), getRandMnMx(0, HEIGHT - 1), getRandMnMx(0, WIDTH - 1));
    sprinkleDraw(m, '%', getRandMnMx(0, 5));
    drawRect(m, '^', 6, 3, 5, 1);
    sprinkleDraw(m, '$', getRandMnMx(0, 10));
    drawBorder(m, '+');
    drawRoads(m, n, s, e, w, 'M','C','#','+','=');

}

void drawRect(tMapTile *m, char terrain, int y1, int x1, int y2, int x2) {
    int yMin, yMax, xMin, xMax;
    yMax = HEIGHT - 1;
    xMax = WIDTH - 1;
    yMin = 0;
    xMin = 0;

    int y, x;

    // FORMAT & BOUND INPUT
    if (x1 > x2) {
        x = x2;
        x2 = x1;
        x1 = x;
    }
    if (y1 > y2) {
        y = y2;
        y2 = y1;
        y1 = y;
    }

    if (x1 < xMin) {
        x1 = xMin;
    }
    if (y1 < yMin) {
        y1 = yMin;
    }
    if (x2 > xMax) {
        x2 = xMax;
    }
    if (y2 > yMax) {
        y2 = yMax;
    }

    for (int i = y1; i <= y2; i++) {
        for (int j = x1; j <= x2; j++) {
            m->map[i][j] = terrain;
        }
    }
}

void drawBorder(tMapTile *m, char terrain) {
    int hM = HEIGHT - 1;
    int wM = WIDTH - 1;
    drawRect(m, terrain, 0, 0, 0, wM);
    drawRect(m, terrain, 0, 0, hM, 0);
    drawRect(m, terrain, hM, 0, hM, wM);
    drawRect(m, terrain, 0, wM, hM, wM);
}

void sprinkleDraw(tMapTile *m, char terrain, int number) {
    for (int i = 0; i < number; i++) {
        m->map[getRandMnMx(0, HEIGHT - 1)][getRandMnMx(0, WIDTH - 1)] = terrain;
    }
}

//possible bug note: the world edge maptile borders retain their border coordinates after being blocked off
void drawRoads(tMapTile *m, int n, int s, int e, int w, char pokM, char pokC, char roadC, char borderC, char gateC)
{


    //Horizontal road coordinates
    int westG = coordOrRand(w, 3, HEIGHT - 4);
    int eastG = coordOrRand(e, 3, HEIGHT - 4);
    int breakH = getRandMnMx(3, WIDTH - 4);

    //mart's random chance
    int posX = m->worldX-200;
    int posY = m->worldY-200;
    int manhattanDist = abs(posX) + abs(posY);
    // printw("\ndistance: %d\n", manhattanDist);
    double percent = 1.0;
    //generate marts with coords
    if(manhattanDist > 200){
        percent = .05;
    }       
    else if(manhattanDist > 0){
        percent = ((-45*manhattanDist)/200+50)/(double)100;
    }

    // printw("\npercent: %f\n", percent);

    if(probability(percent)){
        martsOnRoad(m, westG, eastG, breakH, pokM, pokC);
    }

    //Draw horizontal road
    drawRect(m, roadC, westG, 0, westG, breakH);//draw left horizontal
    drawRect(m, roadC, eastG, breakH, eastG, WIDTH - 1);//draw right horizontal
    drawRect(m, roadC, westG, breakH, eastG, breakH);//draw vertical break

    //Vertical road coordinates
    int breakV = getRandMnMx(2, HEIGHT - 3);
    int southG = coordOrRand(s, 2, WIDTH - 3);
    int northG = coordOrRand(n, 2, WIDTH - 3);

    //Draw vertical road
    drawRect(m, roadC, 0, northG, breakV, northG);//draw top vertical
    drawRect(m, roadC, breakV, southG, HEIGHT - 1, southG);//draw bottom vertical
    drawRect(m, roadC, breakV, northG, breakV, southG);//draw horizontal break

    //Draw gates types
    m->map[0][northG] = gateC;
    m->map[eastG][WIDTH-1] = gateC;
    m->map[HEIGHT-1][southG] = gateC;
    m->map[westG][0] = gateC;


    //block borders
    if(m->worldY <= 0){ 
        m->map[0][northG] = borderC;
    }
    if(m->worldY >= WORLDSIZE-1){ 
        m->map[HEIGHT-1][southG] = borderC;
    }
    if(m->worldX >= WORLDSIZE-1){
        m->map[eastG][WIDTH-1] = borderC;
    }
    if(m->worldX <= 0){ 
        m->map[westG][0] = borderC;
    }   


    //record the gate positions    
    m->n = northG;
    m->s = southG;
    m->e = eastG;
    m->w = westG;

}

void martsOnRoad(tMapTile *m, int y1, int y2, int x1, char pokeM, char pokeC) {
    int pX = getRandMnMx(2, x1 - 2);
    y1++;

    drawRect(m, pokeM, y1, pX, y1 + 1, pX - 1);

    int cX = getRandMnMx(x1, WIDTH - 3);
    y2--;
    drawRect(m, pokeC, y2, cX, y2 - 1, cX + 1);
}

int probability(double percent) {
    // Generate a random number between 0 and 1
    double random_num = (double)rand() / RAND_MAX;

    // Check if the random number is less than the given percentage
    if (random_num < percent) {
        return 1; // Event occurs
    } else {
        return 0; // Event does not occur
    }
}

//takes coordinate, if -1, return a random given the bounds
int coordOrRand(int coordinate, int min, int max)
{
    if(coordinate < 0)
    {
        return getRandMnMx(min, max);
    }
    else
    {
        return coordinate;
    }
}

int getRandMnMx(int min, int max) {
    return getRandLwSz(min, max-min);
}

int getRandLwSz(int lowest, int size) {
    return lowest + rand() % size;
}


void printMap(tMapTile *m) {
    clear();
    printw("%s\n", errorMsg);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char c = m->map[i][j];
            switch(c)//render raw map
            {
                case '+':
                    c = '%';
                    break;
                case '$':
                    c = '^';
                    break;
                case '=':
                    c = '#';
                    break;            
            }

            if(m->cMap[i][j])
            {
                char ct = m->cMap[i][j]->character; //character type
                if(ct == 'a')
                    {c = '@';}
                else
                    {c = ct;}
            }

            printw("%c", c);
        }
        printw("\n");
    }
    printw("\n");
    refresh();
}

void printMapRaw(tMapTile *m) {
    clear();

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printw("%c", m->map[i][j]);
        }
        printw("\n");
    }

    refresh();
}

