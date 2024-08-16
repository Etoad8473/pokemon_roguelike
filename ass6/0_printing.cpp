#include "0_pokemonH.h"


//-------------------------------------------1_mapWorldGeneration-------------------------

void printMap(Map *m) {
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
    printw("World Coordinates- X:%d Y:%d (index y%d,x%d)\n ", m->worldX - 200, 200 - m->worldY, m->worldY, m->worldX);

    //DEBUGGING
    // printHeapShort(m->turnHeap,5);
    // printw("Player Y:%d, X:%d\n",PLAYER->y,PLAYER->x);
    refresh();
}

void printMapRaw(Map *m) {
    clear();

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printw("%c", m->map[i][j]);
        }
        printw("\n");
    }

    refresh();
}

    
//---------------------------------------------3_Dijkstra-------------------------------------


void printPathMap(Node* pMap[HEIGHT][WIDTH])
{
    int y;
    int x;
    for(y = 0; y < HEIGHT; y++)
    {
        for(x=0;x < WIDTH;x++)
        {
            if(pMap[y][x]->currWeight < SHRT_MAX)
                printw("%02d ", (pMap[y][x]->currWeight % 100));
            else   
                printw("-- ");
        }
        printw("\n");
    }
    printw("\n\n");
}

void printHeap(Heap* h)
{
    printw("---HEAP---\nmaxSize: %d\n", h->maxSize);
    printw("currSize: %d\n", h->currSize);

    int i;
    for(i = 0; i< h->maxSize; i++)
    {

        if(h->heapArr[i])
        {
            printw("%d: ", i);
            printDijNode(h->heapArr[i]);
        }
        else
        {
            printw("%d: Null\n", i);
        }
    }
    printw("\n");
}

void printHeapShort(Heap* h, int num)
{
    printw("---HEAP---\nmaxSize: %d\n", h->maxSize);
    printw("currSize: %d\n", h->currSize);

    int i;
    for(i = 0; i< num; i++)
    {

        if(h->heapArr[i])
        {
            printw("%d: ", i);
            printDijNode(h->heapArr[i]);
        }
        else
        {
            printw("%d: Null\n", i);
        }
    }

    printw("\n");
}

void printDijNode(Node *d)
{
    printw("Ind: %d - (Y:%d X:%d), Weight: %d\n", d->heapPos, d->y, d->x, d->currWeight);
}



//---------------------------------------------4_NPCs------------------------------------------



void printCharacter(Character* c)
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

void printCMap(Map* m)
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
