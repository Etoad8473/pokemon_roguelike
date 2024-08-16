#include "0_pokemonH.h"


Node* hikerPMap[HEIGHT][WIDTH] = {NULL};
Node* rivalPMap[HEIGHT][WIDTH] = {NULL};
// //adjacent offsets
// int adjVer[8]={0,-1,-1,-1,0,1,1,1};
// int adjHor[8]={1,1,0,-1,-1,-1,0,1};

// int hikerW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 15, 10, 15, 15, SHRT_MAX, SHRT_MAX};
// int rivalW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, SHRT_MAX};
// char terrTypes[NUMTERRAINTYPES]={'+','$','#','p','c',':','.','%','^','~','='};








// int main(int argc, char* argv[])
// {
//     srand(time(NULL));

//     Map *m = createMapTile(200,200,-1,-1,-1,-1);
//     int playerY = 12;
//     int playerX = 40;
//     spawnCharacterHelper('a',m,playerY,playerX,0);
//     printMap(m);
//     //printw("trying dijkstra\n");

//     initPathMap(hikerPMap);//NEED TO INITIALIZE FOR ALL THIS TO WORK
//     //printPathMap(hikerPMap);

//     dijkstra(m,hikerW,hikerPMap,playerY,playerX);
//     printw("\n--HikerMap--\n");
//     printPathMap(hikerPMap);
//     dijkstra(m,rivalW,hikerPMap,playerY,playerX);
//     printw("\n--RivalMap--\n");
//     printPathMap(hikerPMap);

//     return 0;
// }






void initPathMap(Node* pMap[HEIGHT][WIDTH])//NECESSARY FOR DIJKSTRA TO WORK
{
    int y;
    int x;
    for(y = 0; y < HEIGHT; y++)
    {
        for(x=0;x < WIDTH;x++)
        {
            pMap[y][x] = createDNode(y,x,SHRT_MAX, 0);//TEMP VALUE
        }
    }
}

void resetPathMap(Node* pMap[HEIGHT][WIDTH])
{

    int y;
    int x;
    for(y = 0; y < HEIGHT; y++)
    {
        for(x = 0;x < WIDTH;x++)
        {
            pMap[y][x]->currWeight = SHRT_MAX;
            pMap[y][x]->parent = NULL;
            pMap[y][x]->status = 0;
        }
    }
}


//Input: terrain map, weights array, parallel pathMap, player Pos
int dijkstra(Map* m, int weight[], Node* pMap[HEIGHT][WIDTH])
{
    //check if player exists
    if(!playerExists())//if player doesnt exist, throw error
    {
        printw("dijkstra() cancelled\n-player does not exist");
        return 1;
    }

    //get player coordinates
    int pY = PLAYER->y; int pX = PLAYER->x;
    
    //for all on pathMap,, set Node weight to short.max_value
    resetPathMap(pMap);

    //create minHeap
    Heap h(8);

    //set pathMap[playerLoc] to weight 0
    //add all to heap
    pMap[pY][pX]->currWeight=0;
    h.heapInsert(pMap[pY][pX]);


    //while(!heapEmpty)
        //extract min (do nothing with it)
        //mark it as processed
        //for all adjacent
            //if !processed
                //if terrainDist + parentDist < currWeight
                    //update with new Weight
                    //heapifyUp
                    //setParent
    while(!h.heapIsEmpty())
    {
        Node* parent = h.extractMin();
        //parent->status = 1;

        int i;
        for(i = 0; i < 8; i++)//adjacent nodes
        {
            int nextY = parent->y + adjVer[i];
            int nextX = parent->x + adjHor[i];
            if(nextY < HEIGHT && nextY >= 0 && nextX < WIDTH && nextX >= 0)//check in bounds
            {
                Node* adj = pMap[nextY][nextX];

                if(adj->status < 1)//not processed
                {
                    h.heapInsert(adj);
                    int terrWeight = getTerrainWeight(weight, m, nextY, nextX);
                    int newWeight = terrWeight + parent->currWeight;
                    if(newWeight < adj->currWeight)
                    {
                        adj->currWeight = newWeight;
                        adj->parent = parent;
                        h.heapifyUp(adj->heapPos);
                    }
                }
            }
        }
    }

    return 0;

}


Node* createDNode(int y, int x, int weight, int status)
{
    //TODO: CLAMP X/Y INPUTS

    //TODO: link to rivalMap/hikerMap

    Node* d = (Node*)malloc(sizeof(Node));
    d->heapPos = -1;
    d->currWeight = weight;
    d->y = y;
    d->x = x;
    d->status = status;
    d->order = 0;

    return d;
}

int Heap::compareNodeWeight(Node* a, Node* b) const
{
    int diff = a->currWeight - b->currWeight;

    if(diff == 0)
    {
        return a->order - b->order;
    }
    else
    {
        return diff;
    }

}


int Heap::parent(int pos){return (pos-1) / 2;}

int Heap::leftChild(int pos){return (2 * pos + 1);}

int Heap::rightChild(int pos){return (2 * pos) + 2;}

//MIGHT BE BROKEN
void Heap::swap(int aPos, int bPos)
{
    //swap node's heapPos index
    int tempPos = heapArr[aPos]->heapPos;
    heapArr[aPos]->heapPos = heapArr[bPos]->heapPos;
    heapArr[bPos]->heapPos = tempPos;

    //swap positions in heap
    Node* temp = heapArr[aPos];
    heapArr[aPos] = heapArr[bPos];
    heapArr[bPos] = temp;

}


// void temporaryInsert(Node *d, Heap* h)
// {
//     //check if reaching maxSize,, increase heap size

//     h->heapArr[h->currSize] = d;
//     d->heapPos = h->currSize;
//     h->currSize++;
// }

//OLD
// Heap* createHeap(int size)
// {
//     Heap *h = (Heap*)malloc(sizeof(Heap));
//     // if (h == NULL) {
//     //     fprintw(stderr, "Memory allocation failed.\n");
//     //     exit(1);
//     // }

//     h->currSize = 0;
//     h->maxSize = size;//CHANGE
//     h->heapArr = (Node**)calloc(size,sizeof(Node*));
//     // if (h->heapArr == NULL) {
//     //     fprintw(stderr, "Memory allocation failed.\n");
//     //     exit(1);
//     // }


//     return h;
// }
Heap::Heap(int size)
{

    currSize = 0;
    maxSize = size;//CHANGE
    heapArr = (Node**)calloc(size,sizeof(Node*));

}

Heap::~Heap()
{
    free(heapArr);
}

int Heap::heapIsEmpty(){
    return currSize <= 0;

}

int getTerrainWeight(int weight[], Map* m, int y, int x)
{

    char terr = m->map[y][x];

    int i;
    for(i = 0; i < NUMTERRAINTYPES; i++)
    {
        if(terr == terrTypes[i])
            return weight[i];
    }

    printw("Could not find terrain");
    refresh();
    sleep(1);
    return SHRT_MAX;

}

Node* Heap::peekMin()
{
    return heapArr[0];
}

Node* Heap::extractMin()
{
    if(heapIsEmpty())//if heap is null or empty
    {
        printw("trying to pop an empty or null heap");
        return NULL;
    }

    Node* root = peekMin();

    int tailInd = currSize-1;
    swap(0, tailInd);
    heapArr[tailInd]=NULL;
    currSize--;

    heapifyDown(0);//reset heap property

    root->heapPos = 0;
    root->status=2;
    return root;
}

void Heap::heapInsert(Node* d)
{
    //check if reaching maxSize,, increase heap size
    if(currSize >= maxSize)
    {
        doubleHeapSize();
    }

    heapArr[currSize] = d;//add node at the next open position
    d->heapPos = currSize;
    d->status = 1;
    currSize++;

    int curr = currSize - 1;//get the added node's current pos
    heapifyUp(curr);

}

void Heap::heapifyUp(int index)
{
    //while not root & smaller than parent
    while(index > 0 && compareNodeWeight(heapArr[index],heapArr[parent(index)]) < 0)
    {
        int parentInd = parent(index);
        swap(parentInd, index);

        index = parentInd;
    }
}

void Heap::heapifyDown(int index)
{
    if(currSize <= 1)//if root or empty do nothing
        return;
    
    int left = leftChild(index);
    int right = rightChild(index); 
    int smallest = index;
    if(left < currSize && compareNodeWeight( heapArr[left] , heapArr[index])<0)
        smallest = left;
    if(right < currSize && compareNodeWeight(heapArr[right] , heapArr[smallest])<0)
        smallest = right;

    if(smallest != index)
    {
        swap(index, smallest);
        heapifyDown(smallest);
    }
}   

void Heap::doubleHeapSize()
{

    int newMaxSize = 2*maxSize;

    //copy array
    Node** newHeapArr = (Node**)realloc(heapArr, newMaxSize * sizeof(Node*));

    //null the remaining array
    int i;
    for(i = maxSize; i < newMaxSize; i++)
    {
        newHeapArr[i] = NULL;
    }

    maxSize = newMaxSize;
    heapArr = newHeapArr;
}






//--------testing heap---------//
// int main(int argc, char* argv[])
// {
//     // Heap* h = createHeap(4);
//     // printw("-----------INITIALIZATION-----------\n");

//     // printHeap(h);

//     // printw("-----------AFTER ADDING 3 NODES-----------\n");


//     // temporaryInsert(createDNode(1,1,10,0), h);
//     // temporaryInsert(createDNode(2,2,10,0), h);
//     // temporaryInsert(createDNode(3,3,10,0), h);

//     // printHeap(h);

//     // // // printw("-----------AFTER SWAP-----------\n");

//     // // // swap(0,1,h);

//     // // // printHeap(h);


//     // printw("-----------HEAP INSERT NODES-----------\n");


//     // heapInsert(createDNode(4,4,11,0), h);
//     // heapInsert(createDNode(5,5,10,0), h);
//     // heapInsert(createDNode(6,6,9,0), h);
//     // heapInsert(createDNode(7,7,5,0), h);
//     // heapInsert(createDNode(8,8,6,0), h);
//     // heapInsert(createDNode(9,9,9,0), h);

//     // printHeap(h);


//     // printw("-----------POP HEAP-----------\n");

//     // printDijNode(extractMin(h));

//     // printHeap(h);


//     // printw("-----------SWAP NULL-----------\n");

//     // swap(1,7,h);

//     // printHeap(h);

//     // printw("-----------doubleHeapSize()-----------\n");

//     // doubleHeapSize(h);
//     // doubleHeapSize(h);
//     // // doubleHeapSize(h);

//     // printHeap(h);
// }