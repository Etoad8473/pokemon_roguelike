
#include "pokemonH.h"


dNode_t* hikerPMap[HEIGHT][WIDTH] = {NULL};
dNode_t* rivalPMap[HEIGHT][WIDTH] = {NULL};
// //adjacent offsets
// int adjVer[8]={0,-1,-1,-1,0,1,1,1};
// int adjHor[8]={1,1,0,-1,-1,-1,0,1};

// int hikerW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 15, 10, 15, 15, SHRT_MAX, SHRT_MAX};
// int rivalW[NUMTERRAINTYPES] = {SHRT_MAX, SHRT_MAX, 10, 50, 50, 20, 10, SHRT_MAX, SHRT_MAX, SHRT_MAX, SHRT_MAX};
// char terrTypes[NUMTERRAINTYPES]={'+','$','#','p','c',':','.','%','^','~','='};








// int main(int argc, char* argv[])
// {
//     srand(time(NULL));

//     tMapTile *m = createMapTile(200,200,-1,-1,-1,-1);
//     int playerY = 12;
//     int playerX = 40;
//     spawnCharacterHelper('a',m,playerY,playerX,0);
//     printMap(m);
//     //printf("trying dijkstra\n");

//     initPathMap(hikerPMap);//NEED TO INITIALIZE FOR ALL THIS TO WORK
//     //printPathMap(hikerPMap);

//     dijkstra(m,hikerW,hikerPMap,playerY,playerX);
//     printf("\n--HikerMap--\n");
//     printPathMap(hikerPMap);
//     dijkstra(m,rivalW,hikerPMap,playerY,playerX);
//     printf("\n--RivalMap--\n");
//     printPathMap(hikerPMap);

//     return 0;
// }






void initPathMap(dNode_t* pMap[HEIGHT][WIDTH])//NECESSARY FOR DIJKSTRA TO WORK
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

void resetPathMap(dNode_t* pMap[HEIGHT][WIDTH])
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

void printPathMap(dNode_t* pMap[HEIGHT][WIDTH])
{
    int y;
    int x;
    for(y = 0; y < HEIGHT; y++)
    {
        for(x=0;x < WIDTH;x++)
        {
            if(pMap[y][x]->currWeight < SHRT_MAX)
                printf("%02d ", (pMap[y][x]->currWeight % 100));
            else   
                printf("-- ");
        }
        printf("\n");
    }
    printf("\n\n");
}



//Input: terrain map, weights array, parallel pathMap, player Pos
int dijkstra(tMapTile* m, int weight[], dNode_t* pMap[HEIGHT][WIDTH])
{
    //check if player exists
    if(!playerExists())//if player doesnt exist, throw error
    {
        printf("dijkstra() cancelled\n-player does not exist");
        return 1;
    }

    //get player coordinates
    int pY = player->y; int pX = player->x;
    
    //for all on pathMap,, set Node weight to short.max_value
    resetPathMap(pMap);

    //create minHeap
    heap_t *h = createHeap(8);

    //set pathMap[playerLoc] to weight 0
    //add all to heap
    pMap[pY][pX]->currWeight=0;
    heapInsert(pMap[pY][pX],h);


    //while(!heapEmpty)
        //extract min (do nothing with it)
        //mark it as processed
        //for all adjacent
            //if !processed
                //if terrainDist + parentDist < currWeight
                    //update with new Weight
                    //heapifyUp
                    //setParent
    while(!heapIsEmpty(h))
    {
        dNode_t* parent = extractMin(h);
        //parent->status = 1;

        int i;
        for(i = 0; i < 8; i++)//adjacent nodes
        {
            int nextY = parent->y + adjVer[i];
            int nextX = parent->x + adjHor[i];
            if(nextY < HEIGHT && nextY >= 0 && nextX < WIDTH && nextX >= 0)//check in bounds
            {
                dNode_t* adj = pMap[nextY][nextX];

                if(adj->status < 1)//not processed
                {
                    heapInsert(adj,h);
                    int terrWeight = getTerrainWeight(weight, m, nextY, nextX);
                    int newWeight = terrWeight + parent->currWeight;
                    if(newWeight < adj->currWeight)
                    {
                        adj->currWeight = newWeight;
                        adj->parent = parent;
                        heapifyUp(adj->heapPos, h);
                    }
                }
            }
        }
    }
    
    freeHeap(h);

    return 0;

}


dNode_t* createDNode(int y, int x, int weight, int status)
{
    //TODO: CLAMP X/Y INPUTS

    //TODO: link to rivalMap/hikerMap

    dNode_t* d = (dNode_t*)malloc(sizeof(dNode_t));
    d->heapPos = -1;
    d->currWeight = weight;
    d->y = y;
    d->x = x;
    d->status = status;
    d->order = 0;

    return d;
}

int compareNodeWeight(dNode_t* a, dNode_t* b)
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


int parent(int pos){return (pos-1) / 2;}

int leftChild(int pos){return (2 * pos + 1);}

int rightChild(int pos){return (2 * pos) + 2;}


void swap(int aPos, int bPos, heap_t* h)
{
    //swap node's heapPos index
    int tempPos = h->heapArr[aPos]->heapPos;
    h->heapArr[aPos]->heapPos = h->heapArr[bPos]->heapPos;
    h->heapArr[bPos]->heapPos = tempPos;

    //swap positions in heap
    dNode_t* temp = h->heapArr[aPos];
    h->heapArr[aPos] = h->heapArr[bPos];
    h->heapArr[bPos] = temp;

}

void printHeap(heap_t* h)
{
    printf("---HEAP---\nmaxSize: %d\n", h->maxSize);
    printf("currSize: %d\n", h->currSize);

    int i;
    for(i = 0; i< h->maxSize; i++)
    {

        if(h->heapArr[i])
        {
            printf("%d: ", i);
            printDijNode(h->heapArr[i]);
        }
        else
        {
            printf("%d: Null\n", i);
        }
    }
    printf("\n");
}

void printHeapShort(heap_t* h, int num)
{
    printf("---HEAP---\nmaxSize: %d\n", h->maxSize);
    printf("currSize: %d\n", h->currSize);

    int i;
    for(i = 0; i< num; i++)
    {

        if(h->heapArr[i])
        {
            printf("%d: ", i);
            printDijNode(h->heapArr[i]);
        }
        else
        {
            printf("%d: Null\n", i);
        }
    }

    printf("\n");
}

void printDijNode(dNode_t *d)
{
    printf("Ind: %d - (Y:%d X:%d), Weight: %d\n", d->heapPos, d->y, d->x, d->currWeight);
}



void temporaryInsert(dNode_t *d, heap_t* h)
{
    //check if reaching maxSize,, increase heap size

    h->heapArr[h->currSize] = d;
    d->heapPos = h->currSize;
    h->currSize++;
}

//TODO
heap_t* createHeap(int size)
{
    heap_t *h = (heap_t*)malloc(sizeof(heap_t));
    // if (h == NULL) {
    //     fprintf(stderr, "Memory allocation failed.\n");
    //     exit(1);
    // }

    h->currSize = 0;
    h->maxSize = size;//CHANGE
    h->heapArr = (dNode_t**)calloc(size,sizeof(dNode_t*));
    // if (h->heapArr == NULL) {
    //     fprintf(stderr, "Memory allocation failed.\n");
    //     exit(1);
    // }


    return h;
}

int heapIsEmpty(heap_t* h){
    return h->currSize <= 0;

}

int getTerrainWeight(int weight[], tMapTile* m, int y, int x)
{

    char terr = m->map[y][x];

    int i;
    for(i = 0; i < NUMTERRAINTYPES; i++)
    {
        if(terr == terrTypes[i])
            return weight[i];
    }

    printf("Could not find terrain");
    return SHRT_MAX;

}

dNode_t* peekMin(heap_t* h)
{
    return h->heapArr[0];
}

dNode_t* extractMin(heap_t* h)
{
    if(!h || heapIsEmpty(h))//if heap is null or empty
    {
        printf("trying to pop an empty or null heap");
        return NULL;
    }

    dNode_t* root = peekMin(h);

    int tailInd = h->currSize-1;
    swap(0, tailInd, h);
    h->heapArr[tailInd]=NULL;
    h->currSize--;

    heapifyDown(h,0);//reset heap property

    root->heapPos = (int)NULL;
    root->status=2;
    return root;
}

void heapInsert(dNode_t* d, heap_t* h)
{
    //check if reaching maxSize,, increase heap size
    if(h->currSize >= h->maxSize)
    {
        doubleHeapSize(h);
    }

    h->heapArr[h->currSize] = d;//add node at the next open position
    d->heapPos = h->currSize;
    d->status = 1;
    h->currSize++;

    int curr = h->currSize - 1;//get the added node's current pos
    heapifyUp(curr, h);

}

void heapifyUp(int index, heap_t* h)
{
    //while not root & smaller than parent
    while(index > 0 && compareNodeWeight( h->heapArr[index],h->heapArr[parent(index)]) < 0)
    {
        int parentInd = parent(index);
        swap(parentInd, index, h);

        index = parentInd;
    }
}

void heapifyDown(heap_t* h, int index)
{
    if(h->currSize <= 1)//if root or empty do nothing
        return;
    
    int left = leftChild(index);
    int right = rightChild(index); 
    int smallest = index;
    if(left < h->currSize && compareNodeWeight( h->heapArr[left] , h->heapArr[index])<0)
        smallest = left;
    if(right < h->currSize && compareNodeWeight(h->heapArr[right] , h->heapArr[smallest])<0)
        smallest = right;

    if(smallest != index)
    {
        swap(index, smallest, h);
        heapifyDown(h, smallest);
    }
}   

void doubleHeapSize(heap_t* h)
{

    int newMaxSize = 2*h->maxSize;

    //copy array
    dNode_t** newHeapArr = (dNode_t**)realloc(h->heapArr, newMaxSize * sizeof(dNode_t*));

    //null the remaining array
    int i;
    for(i = h->maxSize; i < newMaxSize; i++)
    {
        newHeapArr[i] = NULL;
    }

    h->maxSize = newMaxSize;
    h->heapArr = newHeapArr;
}

void freeHeap(heap_t* h)
{
    if(!h)//if null
        return;
    free(h->heapArr);
    free(h);
}





//--------testing heap---------//
// int main(int argc, char* argv[])
// {
//     // heap_t* h = createHeap(4);
//     // printf("-----------INITIALIZATION-----------\n");

//     // printHeap(h);

//     // printf("-----------AFTER ADDING 3 NODES-----------\n");


//     // temporaryInsert(createDNode(1,1,10,0), h);
//     // temporaryInsert(createDNode(2,2,10,0), h);
//     // temporaryInsert(createDNode(3,3,10,0), h);

//     // printHeap(h);

//     // // // printf("-----------AFTER SWAP-----------\n");

//     // // // swap(0,1,h);

//     // // // printHeap(h);


//     // printf("-----------HEAP INSERT NODES-----------\n");


//     // heapInsert(createDNode(4,4,11,0), h);
//     // heapInsert(createDNode(5,5,10,0), h);
//     // heapInsert(createDNode(6,6,9,0), h);
//     // heapInsert(createDNode(7,7,5,0), h);
//     // heapInsert(createDNode(8,8,6,0), h);
//     // heapInsert(createDNode(9,9,9,0), h);

//     // printHeap(h);


//     // printf("-----------POP HEAP-----------\n");

//     // printDijNode(extractMin(h));

//     // printHeap(h);


//     // printf("-----------SWAP NULL-----------\n");

//     // swap(1,7,h);

//     // printHeap(h);

//     // printf("-----------doubleHeapSize()-----------\n");

//     // doubleHeapSize(h);
//     // doubleHeapSize(h);
//     // // doubleHeapSize(h);

//     // printHeap(h);
// }