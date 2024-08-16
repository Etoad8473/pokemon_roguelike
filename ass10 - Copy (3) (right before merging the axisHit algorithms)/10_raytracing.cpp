#include "0_pokemonH.h"

#define RESY 10
#define RESX 10
#define HEIGHTOFPLAYER 1.8

int sign(float x);
int floatsEqual(const float a, const float b, float epsilon);



struct pos{
    float y;
    float x;
};



class BlockFace{
    public:
    float height;
    Color colorID;
    char texture;
    int terrainID;

    // BlockFace(float h, Color c, char t){}
    BlockFace(int y, int x, Map* m)
    {

        texture = m->map[y][x];

        terrainID = getTerrainIndex(texture);

        colorID = (Color)terrainID;

        height = terrHeights[terrainID];

        if(m->cMap[y][x] && !cIsPlayer(m->cMap[y][x]))
        {
            if(texture != '^')
            {
                height += 2;
            }
            colorID = ENEMY;
            texture = m->cMap[y][x]->character;
        }

    }

    BlockFace(const BlockFace& b)
    {
        height = b.height;
        colorID = b.colorID;
        texture = b.texture;
        terrainID = b.terrainID;
    }

    private:
    int getTerrainIndex(char terrain)
    {
        int i;

        for(i = 0; i < NUMTERRAINTYPES; i++)
        {
            if(terrTypes[i]==terrain)
            {
                break;
            }
        }

        return i;
    }
};



class Angle
{
    public:
    float gameDir;
    float radians;

    float rise;
    float run;
    int riseSign;
    int runSign;

    float YRowDelta;
    float XRowDelta;
    float YColDelta;
    float XColDelta;

    public:
    Angle(float riseInp, float runInp)
    {
        rise = riseInp;
        run = runInp;
        radians = atan2(rise,run);
        gameDir = radToGameRot(radians);

        fillValues();
    }

    Angle(float gameDirInp)
    {
        gameDir = gameDirInp;
        radians = gameToRadRot(gameDir);
        rise = sin(radians);
        run = cos(radians);

        fillValues();
    }


    Angle* addAngles(Angle first, Angle second)
    {
        //TODO
        return nullptr;
    }

    string toString()
    {
        string out = "";
        out += "gameDir/rads: " + to_string(gameDir) + " " + to_string(radians) + "\n";
        out += "rise/run: " + to_string(rise) + " " + to_string(run) + "\n";
        out += "Row deltas y/x: " + to_string(YRowDelta) + " " + to_string(XRowDelta) + "\n";
        out += "Col deltas y/x: " + to_string(YColDelta) + " " + to_string(XColDelta) + "\n";

        return out;
    }

    private:
    // float calcGameDir(float y, float x)
    // {
    //     return radToGameRot(atan2(y,x));
    // }
    void fillValues()
    {
        riseSign = sign(rise);
        runSign = sign(run);

        YRowDelta = riseSign;
        XRowDelta = (run * riseSign) / rise;

        YColDelta = (rise * runSign) / run;
        XColDelta = runSign;

    }

    float radToGameRot(float rad)
    {
        float gameRot = 8 - (8 * rad / (2*M_PI));
        gameRot = fmod(gameRot, 8);
        return gameRot;
    }

    float gameToRadRot(float gameDir)
    {
        return 2 * M_PI * ((8.0 - gameDir)/8.0);
    }

};


//does not refresh, just sends to buffer
void renderPixel(char texture, Color colorID)
{
    attron(COLOR_PAIR(colorID));
    printw("%c",texture);
    attroff(COLOR_PAIR(colorID));
} //prints colored text



class Hit{
    public:
    float dist;//top down
    BlockFace* closer;
    BlockFace* further;

    // Hit(float d, BlockFace c, BlockFace f) : dist(d), closer(c), further(f) {}

    Hit(float d, pos p, Angle* a, Map* m)
    {
        dist = d;

        int yInt,xInt;
        int yIsInt = 0;
        int xIsInt = 0;
        float wholeY = round(p.y);
        float wholeX = round(p.x);
        if(floatsEqual(p.y,wholeY, 0.01)){
            yIsInt = 1;
            yInt = (int)wholeY;
        }
        if(floatsEqual(p.x,wholeX,0.01)){
            xIsInt = 1;
            xInt = int(wholeX);
        }

        int yClose, yFar, xClose, xFar;

        if(yIsInt && xIsInt) //corner
        {
            if(a->riseSign < 0 && a->runSign > 0)//-,+ = 1
            {
                yClose = yInt;
                xClose = xInt - 1;
                yFar = yInt - 1;
                xFar = xInt;
            }
            else if(a->riseSign < 0 && a->runSign < 0)//-,- = 3
            {
                yClose = yInt;
                xClose = xInt;
                yFar = yInt - 1;
                xFar = xInt - 1;
            }
            else if(a->riseSign > 0 && a->runSign < 0)//+,- = 5
            {
                yClose = yInt - 1;
                xClose = xInt;
                yFar = yInt;
                xFar = xInt - 1;
            }
            else//+,+ = 7
            {
                yClose = yInt - 1;
                xClose = xInt - 1;
                yFar = yInt;
                xFar = xInt;
            }
        }
        else if(!yIsInt && xIsInt) //vert axis hit - horizontal block pair
        {
            if(a->runSign > 0) //run+ = 0
            {
                xClose = xInt - 1;
                xFar = xInt;
                yClose = int(p.y);
                yFar = int(p.y);
            }
            else //run- = 4
            {
                xClose = xInt;
                xFar = xInt - 1;
                yClose = int(p.y);
                yFar = int(p.y);

            }
        }
        else if(yIsInt&&!xIsInt) //horiz axis hit - vertical block pair
        {
            if(a->riseSign < 0) //rise- = 2
            {
                yClose = yInt;
                yFar = yInt - 1;
                xClose = int(p.x);
                xFar = int(p.x);
            }
            else //rise+ = 6
            {
                yClose = yInt - 1;
                yFar = yInt;
                xClose = int(p.x);
                xFar = int(p.x);                
            }
        }
        else //error
        {
            //throw error
        }

        closer = new BlockFace(yClose, xClose, m);
        further = new BlockFace(yFar,xFar,m);

    }

    Hit(const Hit& h)
    {
        dist = h.dist;
        closer = new BlockFace(*h.closer);
        further = new BlockFace(*h.further);
        // printw("H");
    }

    //POSSIBLE MEMORY LEAK NOT DELETING CLOSER/FURTHER REFS

    ~Hit()
    {
        // printw("h");
        delete(closer);
        delete(further);
    }

    string toString()
    {
        return to_string(dist);
    }

    void print()
    {
        renderPixel(closer->texture,closer->colorID);
        renderPixel(further->texture,further->colorID);
    }
};



class Raycast{
    public:
    vector<Hit>* hits;

    Raycast(Angle* a, Player* p, Map* m, vector<Hit>* h)
    {
        hits = h;

        float currDist = 0;

        pos rayOrigin = {p->y + 0.5f, p->x + 0.5f};
        pos firstColHitPos = firstColHit(a, p->y, p->x);
        pos firstRowHitPos = firstRowHit(a, p->y, p->x);

        vector<Hit>* colHits = new vector<Hit>;
        raycastAxis(colHits, rayOrigin, firstColHitPos, a->YColDelta, a->XColDelta,a,m);
        vector<Hit>* rowHits = new vector<Hit>;
        raycastAxis(rowHits, rayOrigin, firstRowHitPos, a->YRowDelta, a->XRowDelta,a,m);

        merge_sorted_arrays(hits, colHits, rowHits);

    }

    ~Raycast()
    {

        // MEMORY ERROR
        //actually ont because hits.clears supposedly deletes
        // for(int i = hits->size() - 1; i >= 0; i--)
        // {
        //     Hit* h = &hits->at(i);
        //     delete(h->closer);
        //     delete(h->further);
        
        //     printw("L");
        //     refresh();
        // }
        // printw("raycast destructor, abt to delete hits\n");
        // refresh();
        
        // printw("CLEARING THE HITS");
        hits->clear();
        // printw("\n\n\n");
        // refresh();
    }

    void print()
    {
        for(int i = 0; i < hits->size(); i++)
        {
            hits->at(i).print();
        }
        printw("\n");
        refresh();
    }

    
    float getPitch(float dist, float height, float playerHeight)
    {
        return (height - playerHeight)/dist;
    }


    private:

    pos firstRowHit(Angle* a, int pY, int pX)
    {
        pos p;
        
        if(a->riseSign > 0)
        {
            p.y = pY + 1;
        }
        else if(a->riseSign < 0)
        {
            p.y = pY;
        }
        else // horizontal
        {
            p.y = 100;
            p.x = 100;
            return p;
        }

        if (a->runSign == 0)
        {
            p.x = pX + 0.5;
        }
        else
        {
            p.x = pX + 0.5 + (0.5*a->XRowDelta);
        }

        return p;

    }

    pos firstColHit(Angle* a, int pY, int pX)
    {
        pos p;

        if(a->runSign > 0)
        {
            p.x = pX + 1;
        }
        else if(a->runSign < 0)
        {
            p.x = pX;
        }
        else // horizontal
        {
            p.y = 100;
            p.x = 100;
            return p;
        }

        if (a->riseSign == 0)
        {
            p.y = pY + 0.5;
        }
        else
        {
            p.y = pY + 0.5 + (0.5*a->YColDelta);
        }

        return p;
    }

    int posInBounds(pos p)
    {
        if(p.y > 0 && p.y < HEIGHT-1)
        {
            if(p.x > 0 && p.x < WIDTH-1)
            {
                return 1;
            }
        }

        return 0;
    }

    float distance(const pos& p1, const pos& p2) {
        float deltaX = p2.x - p1.x;
        float deltaY = p2.y - p1.y;
        
        return sqrt(deltaX * deltaX + deltaY * deltaY);
    }

    void raycastAxis(vector<Hit>* axisHits, pos rayOrigin, pos firstHit, float YDelta, float XDelta, Angle* dir, Map* m)
    {

        pos currP = firstHit;

        while(posInBounds(currP))
        {
            float currDist = distance(rayOrigin, currP);
            Hit h(currDist, currP, dir, m);

            axisHits->push_back(h);

            currP.y += YDelta;
            currP.x += XDelta;
        }

    }

    void merge_sorted_arrays(vector<Hit>* out, const vector<Hit>* arr1, const vector<Hit>* arr2) {
        int i = 0;  // Pointer for arr1
        int j = 0;  // Pointer for arr2

        // Traverse both arrays
        while (i < arr1->size() && j < arr2->size()) //while i & j in-bounds
        {
            if (abs(arr1->at(i).dist - arr2->at(j).dist) < 0.001) //if their dists are equal, push arr[i].dist, iterate over both
            {
                out->push_back(arr1->at(i));
                i++;
                j++;
            }
            else if (arr1->at(i).dist <= arr2->at(j).dist) //if arr1[i].dist is smaller, push arr1[i]
            {
                out->push_back(arr1->at(i));
                i++;
            } 
            else //if arr2[j] is smaller, push arr2[j]
            {
                out->push_back(arr2->at(j));
                j++;
            }
        }

        // Add remaining elements of arr1, if any
        while (i < arr1->size()) 
        {
            out->push_back(arr1->at(i));
            i++;
        }

        // Add remaining elements of arr2, if any
        while (j < arr2->size()) 
        {
            out->push_back(arr2->at(j));
            j++;
        }

    }

};



class Screen{
    public: 
    //low to high rise/run
    float pitchArr[RESY] {};
    //in game rots
    float rotOffsets[RESX] {};

    // float heightOfPlayer;
    float playerRot;
    float inGameHeight;
    float inGameWidth;
    float distFromPlayer;
    char skyTexture;
    vector<Hit>* globalHitVector;

    //2D pixel array
    char pixelTexture[RESY][RESX] {};
    int pixelColors[RESY][RESX] {};

    Screen(float playerH, float sHeight, float sWidth, float sDist, char sky)
    {
        globalHitVector = new vector<Hit>;
        // heightOfPlayer = playerH;
        playerRot = 0;
        inGameWidth = sWidth;
        inGameHeight = sHeight;
        distFromPlayer = sDist;
        skyTexture = sky;

        initializePitches();
        initializeRotOffset();
    }

    void printScreen()
    {
        printw("printing screen\n");
        refresh();

        for(int y = 0; y < RESY; y++)
        {
            for(int x = 0; x < RESX; x++)
            {
                renderPixel(pixelTexture[y][x],(Color)pixelColors[y][x]);
            }
            printw("\n");
        }
    }

    void renderAllLines(Map* m);
    
    void renderColumn(Raycast* r, int columnIndex);

    ~Screen()
    {
        delete(globalHitVector);
        //delete the pitchArr references
        //delete the rotOFfset array references
    }

    

    void fillUpTo(int* pitchID, float nextPitch, int colIndex, BlockFace* block)
    {
        int pID = *pitchID;

        // printw("inside fill pID: %d", pID);//for testing
        refresh();

        while(pitchArr[pID] <= nextPitch && pID < RESY)
        {
            //get correct screen Y index
            int screenYPos = RESY - 1 - pID;

            //paint it in for color and texture
            setPixel(screenYPos, colIndex, block->texture, block->colorID);


            *pitchID += 1;
            pID = *pitchID;
        }
    }

    void fillSky(int* pitchID, int colIndex)
    {
        while(*pitchID < RESY)
        {
            int screenYPos = RESY - 1 - *pitchID;
            
            setPixel(screenYPos, colIndex, skyTexture, SKY);

            *pitchID += 1;
        }
    }

    private:
    void initializePitches()
    {
        float lowestY = -0.5 * inGameHeight;
        float yDelta = inGameHeight / RESY;

        float currY = lowestY;

        for(int i = 0; i < RESY; i++)
        {
            pitchArr[i] = currY/distFromPlayer;
            currY += yDelta;
            // printw("pitches: %.2f %.2f %.2f %.2f ", pitchArr[i], inGameHeight, currY, distFromPlayer); //for testing
        }

        refresh();
    }

    void initializeRotOffset()
    {
        float leftWidth = -0.5* inGameWidth;
        float xDelta = inGameWidth / RESX;

        float currX = leftWidth;

        for(int i = 0; i < RESX; i++)
        {
            Angle a(currX, distFromPlayer);
            rotOffsets[i] = a.gameDir;

            currX += xDelta;
        }
    }

    void setPixel(int y, int x, char texture, Color c)
    {

        pixelTexture[y][x] = texture;
        pixelColors[y][x] = c;
        
        //FOR TESTING
        renderPixel(texture,c);

    }
};



int floatsEqual(const float a, const float b, float epsilon)
{
    float diff = abs(a-b);
    if(diff <= epsilon)
    {
        return 1;
    }
    else
        return 0;
}



void Screen::renderColumn(Raycast *r, int columnIndex)
{
    int pitchInd = 0;
    int hitInd;


    //for testing
    printw("\n");

    for (hitInd = 0; hitInd < r->hits->size(); hitInd++)
    {
        
        Hit h = r->hits->at(hitInd);
        
        float nextPitch;
        

        if (pitchArr[pitchInd] <= 0)
        {
            
            
            nextPitch = r->getPitch(h.dist, h.closer->height, HEIGHTOFPLAYER);
                            

            if (pitchArr[pitchInd] < nextPitch)
            {
                fillUpTo(&pitchInd, nextPitch, columnIndex, h.closer);
            }

        }
        else
        {

            nextPitch = r->getPitch(h.dist, h.further->height, HEIGHTOFPLAYER);
            if (nextPitch > pitchArr[pitchInd])
            {
                fillUpTo(&pitchInd, nextPitch, columnIndex, h.further);
            }
        }

    }

    fillSky(&pitchInd, columnIndex);


}



void Screen::renderAllLines(Map* m)
{
    // Angle* a = new Angle(rotOffsets[4]);
    // Raycast* r = new Raycast(a, PLAYER, m, globalHitVector);

    for(int x = 0; x < RESX; x++)
    {
        Angle* a = new Angle(rotOffsets[x]);
        Raycast* r = new Raycast(a, PLAYER, m, globalHitVector);
        renderColumn(r,x);

        delete(r);

        // clear();
        // printScreen();
        // printw("\n");
        // refresh();
        // sleep(1);

    }
}



int sign(float x) {
    if(abs(x) < 0.001){
        return 0;
    }
    else if (x > 0.0) {
        return 1; // Positive
    } 
    else {
        return -1; // Negative
    }
}



int main(int argc, char* argv[])
{
    initscr();
    noecho();
    keypad(stdscr, TRUE); 
    // start_color();
    // initColors();

    //srand(time(NULL));                                  //init Rand

    char c;

    Map* m = initializeGame(5); // defined in 1
    echo();

    while(c != 'q')
    {
        clear();
        printMap(m);


        Screen* s = new Screen(1.8, 3, 4, .75, 's');


        Angle* a = new Angle(.5);
        Raycast* r = new Raycast(a, PLAYER, m, s->globalHitVector);
        s->renderColumn(r,0);

        a = new Angle(1);
        r = new Raycast(a, PLAYER, m, s->globalHitVector);
        s->renderColumn(r,0);

        a = new Angle(1.5);
        r = new Raycast(a, PLAYER, m, s->globalHitVector);
        s->renderColumn(r,0);
        

        s->renderAllLines(m);
        s->printScreen();

        printw("continue?\n");
        refresh();
        c = getch();

        delete(s);

    }

    echo();
    endwin();

    return 0;
}
