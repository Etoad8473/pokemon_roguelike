#include "0_pokemonH.h"


Map* fly_cutscene(Map* m);


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
    // Heap* turnHeap = createHeap(8);                   //create heap
    // Map *m = createMapTile(200,200,-1,-1,-1,-1);   //init map
    Map* m = initializeGame(numNPC);           //spawn PC
    // dijkstra(m, hikerW, hikerPMap);                     //run dijkstra for hiker&rival
    // dijkstra(m, rivalW, rivalPMap);                     //^

    //spawn in NPC's +give order
        //add to queue with terrainWeight & order
    
    // spawnAllNPCs(numNPC,m);

    //ship the loading bar:
    //sleep(2); 
    
    startPokemon_cutscene();

    printMap(m);

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
    while(!m->turnHeap->heapIsEmpty() && !quitGame)
    {
        Node* node = m->turnHeap->extractMin();
        Character* c = m->cMap[node->y][node->x];

        Map* prevM = m;
        m = c->moveFunct(m,c);
        if(m == prevM)
        {
            c->nextTurn = c->nextTurn + getTerrainWeight(c->weights,m,c->y,c->x);

            node->currWeight = c->nextTurn;
            node->y = c->y;
            node->x = c->x;

            m->turnHeap->heapInsert(node);
        }
        else
        {
            //print correctly moved maps
            printw("correctly moved maps");
        }
    }

    echo();//dont forget to turn it back on
    endwin();

}

//returns a direction
Map* keyboardInput(Map *m, Character *pc)
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
                printMessage("stand still.");
                break;


            //Pokemart
            case '>':
                pokemart_cutScene(m,pc);
                needInput = 1; //this keeps the game in the same state when returning from pokemart_cutScene
                break;
            
            case 't':
                trainersList_cutscene(m,pc);
                needInput = 1; //this keeps the game in the same state when returning from trainers list
                break;

            case 'f':
                Map* nM;
                nM = fly_cutscene(m);
                if(!(nM))
                {
                    needInput = 1;
                }
                else
                {
                    m = nM;
                }
                break;

            default:
                needInput = 1;
                break;
            
        }
    }

    return m;

}

void pokemart_cutScene(Map *m, Character *pc){
    /*
    check if standing on pokemart_cutScene
    if yes: clear window and refresh with pokemart_cutScene template, wait for '<' input to reset to map
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

void trainersList_cutscene(Map* m, Character* pc)
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

void npcPrintStatement(Map* m, Character* pc, Character* npc)
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
void pokemonBattle_cutscene(Map* m, Character* npc)
{
    printMap(m);
    clear();

    printw("\n\n\n\n\n\n------------This is a pokemon battle with NPC #%d!---------\n(press 'esc' to win battle and continue)\n", npc->turnOrder);
    for(int i = 0; i < npc->pokeRoster->size(); i++)
    {
        Pokemon p = npc->pokeRoster->at(i);
        printw("%s lvl %d\n", p.info->name.c_str(), p.level);
    }
    
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


//6_

/*
return 1 if pressed escape (for need more input)

clear (incase multiple fly attempts), print map + print instructions
turn on echo
getInput w ncurses
turn off echo
print outputs
confirm outputs

flyToTile
*/
Map* fly_cutscene(Map* m)
{
    clear();
    printMap(m);
    printw("You pressed f to fly. Enter coordinates [X,Y] (enter after each inp):\n");
    refresh();

    echo();

    char inputX[4];
    getnstr(inputX, 4);
    int numX = atoi(inputX);

    char inputY[5];
    getnstr(inputY, 4);
    int numY = atoi(inputY);

    noecho();

    printMap(m);
    printw("Fly to [X:%d, Y:%d]? ('enter' to confirm/'esc' to cancel)\n", numX, numY);
    refresh();

    char confirm = getch();
    while(!('\n' == confirm || 27 == confirm))
    {
        confirm = getch();
    }

    if(confirm == '\n')
    {

        

        numX += 200; numY = 200 - numY;//format inputs

        if(numX < 0 || numX >= WORLDSIZE || numY < 0 || numY >= WORLDSIZE)
        {
            printw("EzWarning: out of bounds input parameters passed to flyToTile function\n");
            printMap(m);//TODO: change to printMapNoClear();
            return NULL;
        }
        
        return flyToTile(m,numY, numX, 'w');
    }
    else //input was 'esc'
    {
    
    clear();
    printMap(m);

    return NULL;//do nothing and return to input fetching

    }


}