#include "pokemonH.h"



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
    drawRoads(m, n, s, e, w, 'p','c','#','+','=');

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


void printMap(tMapTile *m, int pY, int pX) {
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

            if(pY && pX)//if not null
            {
                if(i==pY && j==pX)//if on the player spot
                {
                    c = '@';
                }    
            }

            printf("%c", c);
        }
        printf("\n");
    }
}

void printMapRaw(tMapTile *m) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", m->map[i][j]);
        }
        printf("\n");
    }
}