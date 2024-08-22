#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 21
#define WIDTH 80
#define WORLDSIZE 401

typedef struct tMapTile
{
    int worldX,worldY;
    int n,w,s,e;
    char map[HEIGHT][WIDTH];
} tMapTile;



void runner();
void fillMap(tMapTile *m, int n, int s, int e, int w);
void drawRect(tMapTile *m, char terrain, int y1, int x1, int y2, int x2);
void drawBorder(tMapTile *m, char terrain);
void sprinkleDraw(tMapTile *m, char terrain, int number);
void drawRoads(tMapTile *m, int n, int s, int e, int w);
void martsOnRoad(tMapTile *m, int y1, int y2, int x1);
int getRandMnMx(int lowest, int size);
void printMap(tMapTile *m);
void linkMapToWorld(tMapTile *m, int y, int x);
int coordOrRand(int coordinate, int lowest, int size);
tMapTile* createMapTile(int y, int x, int n, int s, int e, int w);
int getRandLwSz(int lowest, int size);
void flyToTile(tMapTile **currMap, int y, int x);
int probability(double percent);


tMapTile *worldMap[401][401] = {NULL};

int main() 
{
    srand(time(NULL));

    runner();

    // tMapTile *m = createMapTile(200,200,-1,-1,-1,-1);
    // printMap(m);

    return 0;
}

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

//TODO
//TODO: add pokecenter percent
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
    sprinkleDraw(m, '^', getRandMnMx(0, 10));
    drawBorder(m, '%');
    drawRoads(m, n, s, e, w);

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
void drawRoads(tMapTile *m, int n, int s, int e, int w) //TODO: pokecenter percent
{


    //Horizontal road coordinates
    int westG = coordOrRand(w, 3, HEIGHT - 4);
    int eastG = coordOrRand(e, 3, HEIGHT - 4);
    int breakH = getRandMnMx(3, WIDTH - 4);

    //mart's random chance
    int posX = m->worldX-200;
    int posY = m->worldY-200;
    int manhattanDist = abs(posX) + abs(posY);
    // printf("\ndistance: %d\n", manhattanDist);
    double percent = 1.0;
    //generate marts with coords
    if(manhattanDist > 200){
        percent = .05;
    }       
    else if(manhattanDist > 0){
        percent = ((-45*manhattanDist)/200+50)/(double)100;
    }

    // printf("\npercent: %f\n", percent);

    if(probability(percent)){
        martsOnRoad(m, westG, eastG, breakH);
    }

    //Draw horizontal road
    drawRect(m, '#', westG, 0, westG, breakH);//draw left horizontal
    drawRect(m, '#', eastG, breakH, eastG, WIDTH - 1);//draw right horizontal
    drawRect(m, '#', westG, breakH, eastG, breakH);//draw vertical break

    //Vertical road coordinates
    int breakV = getRandMnMx(2, HEIGHT - 3);
    int southG = coordOrRand(s, 2, WIDTH - 3);
    int northG = coordOrRand(n, 2, WIDTH - 3);

    //Draw vertical road
    drawRect(m, '#', 0, northG, breakV, northG);//draw top vertical
    drawRect(m, '#', breakV, southG, HEIGHT - 1, southG);//draw bottom vertical
    drawRect(m, '#', breakV, northG, breakV, southG);//draw horizontal break

    //block borders
    if(m->worldY <= 0){ 
        m->map[0][northG] = '%';
    }
    if(m->worldY >= WORLDSIZE-1){ 
        m->map[HEIGHT-1][southG] = '%';
    }
    if(m->worldX >= WORLDSIZE-1){
        m->map[eastG][WIDTH-1] = '%';
    }
    if(m->worldX <= 0){ 
        m->map[westG][0] = '%';
    }   


    //record the gate positions    
    m->n = northG;
    m->s = southG;
    m->e = eastG;
    m->w = westG;

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

void martsOnRoad(tMapTile *m, int y1, int y2, int x1) {
    int pX = getRandMnMx(2, x1 - 2);
    y1++;

    drawRect(m, 'p', y1, pX, y1 + 1, pX - 1);

    int cX = getRandMnMx(x1, WIDTH - 3);
    y2--;
    drawRect(m, 'c', y2, cX, y2 - 1, cX + 1);
}

int getRandMnMx(int min, int max) {
    return getRandLwSz(min, max-min);
}

int getRandLwSz(int lowest, int size) {
    return lowest + rand() % size;
}

void printMap(tMapTile *m) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", m->map[i][j]);
        }
        printf("\n");
    }
}

void printWorld() {
    for (int i = 0; i < 401; i++) {
        for (int j = 0; j < 401; j++) {
            char print = '.';

            if(worldMap[i][j] != NULL)
            { 
                print = '%';
            }

            printf("%c", print);
        }
        printf("\n");
    }
}