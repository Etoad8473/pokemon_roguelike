#include "pokemonH.h"

void gameRunner(int numNPC);
void pokemart(tMapTile *m, characterT *pc);
void trainersList(tMapTile* m, characterT* pc);
void npcPrintStatement(tMapTile* m, characterT* pc, characterT* npc);



int main(int argc, char* argv[])
{

    // while( (ch = getch()) != 'q'){
    //     printw("you pressed %c\n", ch);
    //     refresh();
    // }
    
    gameRunner(15);


    
    return 0;

}

void gameRunner(int numNPC)
{
    char ch;
    initscr();
    noecho();
    keypad(stdscr, TRUE); 

    if(numNPC == -1)
        numNPC = DEFAULT_NUM_TRAINERS;
    
    printw("Number of trainers: %d\n\n\n\n\n\n---------------LOADING GAME-------------\n", numNPC);
    refresh();

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

    //ship the loading bar:
    //sleep(2); 
    


    printMap(m);

    //DEBUGGING: spawning more npc's
    // char cha;
    // while((cha = getch()) != 'n')
    // {
    //     characterT* npc = spawnNPC(getRandNPCType(), m,turnHeap,HEIGHT/2,WIDTH/2);
    //     printMap(m);
    //     printw("%d", getTerrainWeight(npc->weights,m,npc->y,npc->x));
    //     refresh();
    // }

    /*
    while heap!empty
        pop heap
        get character from XY
        move them
        update their turnCount
        push to heap
        print heap
        print map
        wait a second
    */
    while(!heapIsEmpty(turnHeap) && !quitGame)
    {
        dNode_t* node = extractMin(turnHeap);
        characterT* c = m->cMap[node->y][node->x];

        c->moveFunct(m,c);
        c->nextTurn = c->nextTurn + getTerrainWeight(c->weights,m,c->y,c->x);

        node->currWeight = c->nextTurn;
        node->y = c->y;
        node->x = c->x;

        heapInsert(node, turnHeap);
    }


    // while( (ch = getch()) != 'q'){
    //     switch(ch){
    //         case('w'):
    //             pc->currDir = 2;
    //             break;
    //         case('a'):
    //             pc->currDir = 4;
    //             break;
    //         case('s'):
    //             pc->currDir = 6;
    //             break;
    //         case('d'):
    //             pc->currDir = 0;
    //             break;
    //     }

    //     playerMove(m, pc);  

    // }

    echo();//dont forget to turn it back on
    endwin();

}

//returns a direction
void keyboardInput(tMapTile *m, characterT *pc)
{
    char ch;
    int needInput = 1;

    while(needInput)//if invalid input keep asking for input
    {
        ch = getch();
        needInput = 0;

        switch(ch)
        {
            case 'q':
                quitGame = 1;
                break;
            

            //player input
            case '6':            
            case 'l':
                pc->currDir = 0;
                break;
            
            case '9':            
            case 'u':
                pc->currDir = 1;
                break;
            
            case '8':            
            case 'k':
                pc->currDir = 2;
                break;
            
            case '7':            
            case 'y':
                pc->currDir = 3;
                break;
            
            case '4':            
            case 'h':
                pc->currDir = 4;
                break;
            
            case '1':            
            case 'b':
                pc->currDir = 5;
                break;
            
            case '2':            
            case 'j':
                pc->currDir = 6;
                break;
            
            case '3':            
            case 'n':
                pc->currDir = 7;
                break;
                            
            case '5':            
            case ' ':
                pc->currDir = 8;
                // setMessage("stand still.");
                strcpy(errorMsg, "stand still.");
                break;


            //Pokemart
            case '>':
                pokemart(m,pc);
                needInput = 1; //this keeps the game in the same state when returning from pokemart
                break;
            
            case 't':
                trainersList(m,pc);
                needInput = 1; //this keeps the game in the same state when returning from trainers list
                break;


            default:
                needInput = 1;
                break;
            
        }
    }

}

void pokemart(tMapTile *m, characterT *pc){
    /*
    check if standing on pokemart
    if yes: clear window and refresh with pokemart template, wait for '<' input to reset to map
    */
    if(m->map[pc->y][pc->x] == 'M')
    {
        clear();
        printw("\n\n\n\n\n\n-------Pokemart-------\n(press '<' to exit)\n");
        refresh();

        char ch;
        while((ch = getch()) != '<')//keep fetching getch() until it equals '<'
        {
            clear();
            printw("\n\n\n\n\n\n-------Pokemart-------\n(press '<' to exit)\n bruh you pressed %c\n", ch);
            refresh();
        }

        //resume the game console
        clear();
        printMap(m);
        refresh();
    }
    else
    {
        //TODO: throw error msg
    }

}

void trainersList(tMapTile* m, characterT* pc)
{
    clear();
    printw("Trainer list (press 'esc' to exit):\n");

    int start = 0;
    int displayNum = 10;//size of npc list display (the number is arbitrary)
    int i;
    for(i = start; i < start + displayNum; i++)
    {
        npcPrintStatement(m, pc, m->npcList[i]);
    }
    if(displayNum < m->numNPCs)
        printw("...\n");

    refresh();

    int ch;

    while((ch = getch()) != 27)//waiting for escape key
    {
        if(ch == KEY_DOWN && start + displayNum < m->numNPCs)
            start++;
        else if(ch == KEY_UP && start > 0)
            start--;
        else   
            continue;

        clear();
        printw("Trainer list (press 'esc' to exit):\n");
        if(start > 0)
            printw("...\n");

        for(i = start; i < start + displayNum; i++)
        {
            npcPrintStatement(m, pc, m->npcList[i]);
        }

        if(start + displayNum < m->numNPCs)
            printw("...\n");


        refresh();
    }
    //resume the game console
    clear();
    printMap(m);
    refresh();

}

void npcPrintStatement(tMapTile* m, characterT* pc, characterT* npc)
{
    if(!npc){
        printw("No NPC found\n");
        return;
    }

    int pY = pc->y;
    int pX = pc->x;
    int nY = npc->y;
    int nX = npc->x;

    printw("%c, id #%d,",npc->character, npc->turnOrder);
    int verDiff = nY - pY;
    int horDiff = nX - pX;

    if(verDiff > 0)
        printw(" %d south", verDiff);
    else
        printw(" %d north", verDiff * -1);


    if(horDiff > 0)
        printw(" and %d east\n", horDiff);
    else
        printw(" and %d west\n", horDiff * -1);



    refresh();

}

//TODO: temporary pokemon battle
void pokemonBattle(tMapTile* m, characterT* npc)
{
    printMap(m);
    clear();

    printw("\n\n\n\n\n\n------------This is a pokemon battle with NPC #%d!---------\n(press 'esc' to win battle and continue)\n", npc->turnOrder);
    refresh();

    //resume the game console
    while(getch() != 27){}//searching for escape key
    npc->defeated = 1;//Defeat npc
    clear();
    printMap(m);
    refresh();
}

// void setMessage(char* string){
//     errorMsg = string;
// }
