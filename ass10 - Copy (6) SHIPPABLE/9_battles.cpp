#include "0_pokemonH.h"



stringstream MENUprint;


class Action;
class AttackAction;
class FleeAction;

int playerRosterStillAlive();
int PC_vs_Pokemon_battleLoop(Pokemon* myPoke, Pokemon* enemyPoke, int canFlee);
Action* decideBattleAction(Pokemon* myPoke, Pokemon* enemyPoke, int *continueBattle, int *canFlee);
Action* fightMenu(Pokemon* myPoke, Pokemon* enemyPoke);
int chooseFromMenu(vector<string> choices, string message, Pokemon* myPoke, Pokemon* enemyPoke);
Action* switchPokemonMenu(Pokemon* myPoke, Pokemon* enemyPoke);
Action* bagMenu(Pokemon* myPoke, Pokemon* enemyPoke, int* continueBattle, int* canFlee);
void printBattleMenu(Pokemon* myPoke, Pokemon* enemyPoke);
AttackAction* generateEnemyAttack(Pokemon* playerPoke, Pokemon* enemyPoke);
int bothPokemonAlive(Pokemon* myPoke, Pokemon* enemyPoke);
Pokemon* nextPokemonInRoster(vector<Pokemon>* roster);


class Action{

    public:
    int order;//lower goes first

    virtual int perform() = 0;
    virtual string toString() = 0;

    protected:
    virtual void calculateOrder() = 0;

};

class FleeAction : public Action{
    public:
    int perform() override{
        return 0;
    }

    string toString() override{
        return "You fled.\n";
    }

    FleeAction(int* continueBattle)
    {
        *continueBattle = 0;
        calculateOrder();
    }

    protected:
    void calculateOrder() override{
        order = 1000;
    }
};

class AttackAction : public Action{

    public:
    int damage;
    Move* move;
    Pokemon* myP;
    Pokemon* enemyP;

    //return value represents a KO, 0 for no, 1 for yes
    int perform() override{

        int knockedOut = 0;

        //try hit,, possible miss
        if(rand()%100 > move->accuracy){
            MENUprint<<move->name<<" missed.\n";
        }
        else//if under the threshold
        {
            //if hit,, do damage, dont go below 0
            enemyP->currHP -= damage;

            //if KO, KO = true
            MENUprint<<myP->info->name<<"'s "<<move->name<<" did "<<to_string(damage)<<" damage.";
            if(enemyP->currHP <= 0)
            {
                enemyP->currHP = 0;
                MENUprint<<" "<<enemyP->info->name<<" was knocked out.";
                knockedOut = 1;
            }
            MENUprint<<endl;

            //print action message (move used & damage done, OR miss, OR k.o.)
        }

        return knockedOut;
    }

    AttackAction(Move* m, Pokemon* myPoke, Pokemon* enemyPoke)
    {
        move = m;
        myP = myPoke;
        enemyP = enemyPoke;
        calculateDamage(m, myPoke, enemyPoke);
        calculateOrder();
    }

    string toString() override
    {
        string out = move->name + " Damage: ";
        out += to_string(damage);
        out += "\n";
        return out;
    }


    protected:
    void calculateDamage(Move* m, Pokemon* myPoke, Pokemon* enemyPoke){
        int l = myPoke->level;
        int p = m->power;
        int a = myPoke->stats[attackSTAT];
        int d = enemyPoke->stats[defenseSTAT];
        float crit = calculateCrit(myPoke);
        float rand = ((float)getRandMnMx(85,100))/100.0;
        float stab = calculateSTAB(m,enemyPoke);
        int type = 1;

        damage = (int)((((((2*l)/5)+2)*p*(a/d))/50)+2)*crit*rand*stab*type;
    }

    void calculateOrder() override{
        order = move->priority * 100 + myP->stats[speedSTAT];
    }
    //returns the semiRandom crit 1.5 multiplier
    float calculateCrit(Pokemon* myPoke){
        int threshold = myPoke->baseStats[speedSTAT]/2;
        int rand = getRandMnMx(0,255);

        if(rand<threshold)
        {return 1.5;}
        else
        {return 1.0;}
        
    }
    //returns Same Type Attack Bonus (STAB) 1.5 multiplier
    float calculateSTAB(Move* m, Pokemon* enemyPoke)
    {
        //TODO
        //if the move's type matches the attacking pokemon's type 1.5, else 1
        return 1.0;
    }
};





class Item : public Action
{

};

class CatchPokemon : public Item 
{
    public:
    int perform() override{
        return 0;
    }

    string toString() override{
        return "You captured a pokemon.\n";
    }

    CatchPokemon(Pokemon* wildP, int catchable, int *continueBattle)
    {
        if(catchable  &&  PLAYER->pokeRoster->size() < 6  &&  PLAYER->pokeballs > 0)
        {
            //straight up capture the pokemon yo
            MENUprint<<"You captured "<<wildP->info->name.c_str()<<"!\n";

            PLAYER->pokeRoster->push_back(*wildP);

            PLAYER->pokeballs -= 1;
        }
        *continueBattle = 0;
        calculateOrder();
    }

    protected:
    void calculateOrder() override{
        order = 1000;
    }
    
};

// class SwitchAction: public Action
// {
//     public:
//     int perform() override
//     {

//     }

//     switchAction(Pokemon* myPoke)
//     {

//     }
// };




//put this inside of wild_pokemon_cutscene and trainer_battle_cutscene
int PC_vs_Pokemon_battleLoop(Pokemon* myPoke, Pokemon* enemyPoke, int canFlee)
{
    //while person and pokemon still alive && continueBattle
    int continueBattle = 1;

    while(bothPokemonAlive(myPoke,enemyPoke) && continueBattle)
    {
        continueBattle = 1;
        //TOOD: change new Pokemon->getFirstPoke()
        Action* a = decideBattleAction(myPoke,enemyPoke, &continueBattle, &canFlee);
        if(!continueBattle && canFlee)
        {
            MESSAGEprint<<"Fled.";
            printw("Fled the encounter. Press anything to quit.");
            refresh();
            getch();
            return 1;
        }

        //AI attack
        AttackAction* enemyAtk = generateEnemyAttack(myPoke, enemyPoke);

        

        if(enemyAtk->order > a->order)
        {
            int KO = enemyAtk->perform();
            if(!KO)
            {
                a->perform();
            }
        }
        else//a is greater or equal to enemyAtk
        {
            //player first, then enemy
            int KO = a->perform();
            if(!KO)
            {
                enemyAtk->perform();
            }
        }
        

        //print Menu
        MENUprint<<"(press anything to continue)\n";
        printBattleMenu(myPoke,enemyPoke);
        
        //pause for input
        getch();

    }

    return 0;
    //if it gets to the end the battle is over
}

int bothPokemonAlive(Pokemon* myPoke, Pokemon* enemyPoke)
{
    return myPoke->currHP && enemyPoke->currHP;
}

AttackAction* generateEnemyAttack(Pokemon* playerPoke, Pokemon* enemyPoke)
{
    int chooseMove = getRandLwSz(0, enemyPoke->movesKnown.size()); 
    return new AttackAction(enemyPoke->movesKnown.at(chooseMove), enemyPoke, playerPoke);
}


Action* decideBattleAction(Pokemon* myPoke, Pokemon* enemyPoke, int *continueBattle, int* canFlee)
{
    Action* a = nullptr;
    
    while(a == nullptr)
    {
        int choice = 1;
        int numChoices;
        vector<string> choices = {"fight","switch","bag","flee"};
        string message = "choose a battle option\n";
        int choiceInd = chooseFromMenu(choices, message, myPoke,enemyPoke);
        switch(choiceInd)
        {
            case 0:
                //fightMenu(&PLAYER->pokeRoster->at(0));//TODO: GET PLAYER'S POKEMON
                a = fightMenu(myPoke, enemyPoke);
                break;
            case 1:
                a = switchPokemonMenu(myPoke, enemyPoke);
                break;            
            case 2:
                a = bagMenu(myPoke,enemyPoke,continueBattle,canFlee);
                break;            
            case 3:
                a = new FleeAction(continueBattle);
                break;            
            default:
                break;
        }

    }

    return a;
}

Action* fightMenu(Pokemon* myPoke, Pokemon* enemyPoke)
{    
    vector<string> moveListString;
    for(int i = 0; i < myPoke->movesKnown.size(); i++){
        moveListString.push_back(myPoke->movesKnown.at(i)->name);
    }

    int choice = chooseFromMenu(moveListString, "Moves:\n",myPoke,enemyPoke);
    if(choice == -1)
    { return nullptr; }
    else
    { 
        Move* m = myPoke->movesKnown.at(choice);

        return new AttackAction(m,myPoke,enemyPoke);
    }
    
}

Action* switchPokemonMenu(Pokemon* myPoke, Pokemon* enemyPoke)
{
    for(int i =0; i < PLAYER->pokeRoster->size();i++)
    {
        MENUprint<<PLAYER->pokeRoster->at(i).info->name.c_str()<<endl;
    }
    MENUprint<<"(press anything to return.)\n";
    printBattleMenu(myPoke,enemyPoke);
    getch();

    return nullptr;
}

Action* bagMenu(Pokemon* myPoke, Pokemon* enemyPoke, int* continueBattle, int* canFlee)
{

    Action* a = nullptr;

    // int choice = 1;
    // vector<string> choices = {"pokeball"};
    // string message = "choose an item type\n";
    // int choiceInd = chooseFromMenu(choices, message, myPoke,enemyPoke);
    // switch(choiceInd)
    // {
    //     case 0:
    //         //fightMenu(&PLAYER->pokeRoster->at(0));//TODO: GET PLAYER'S POKEMON
    //         if(*canFlee && PLAYER->pokeballs>0)
    //         {
    //             a = new CatchPokemon(enemyPoke, *canFlee, continueBattle);
    //         }
    //         else
    //         {
    //             MENUprint<<"failed pokeball throw.\n";
    //         }
    //         break;           
    //     default:
    //         break;
    // }

    printw("\n\nbag menu (press anything to return)");
    refresh();
    getch();

    return a;

}


//returns index, -1 for escape
int chooseFromMenu(vector<string> choices, string message, Pokemon* myPoke, Pokemon* enemyPoke)
{
    char c = 'a';//arbitrary character, will be set later
    int choiceInd = 0;
    int numChoices = choices.size();
    message += "(scroll up with '1' scroll down with '2')\n";

    while(!(c == '\n' || c == ESCAPE_KEY))//if enter on selection or escape key
    {
        if(c == '1')
        {
            choiceInd--;
            if(choiceInd < 0)
            {choiceInd = choices.size()-1;}
        }
        else if(c == '2')
        {
            choiceInd++;
            if(choiceInd >= choices.size())
            {choiceInd = 0;}
        }


        clear();
        MENUprint<<message;
        for(int i = 0; i < numChoices; i++)
        {
            MENUprint << choices.at(i);
            if(i==choiceInd){MENUprint<<"<-\n";}
            else{MENUprint<<"\n";}
        }        
        printBattleMenu(myPoke,enemyPoke);

        c = getch();
    }

    if(c == ESCAPE_KEY)
    {
        return -1;
    }
    else
    {
        return choiceInd; //the index of the choice
    }
}


int playerRosterStillAlive()
{
    //TODO
    int yes = 1;
    return yes;
}


void printBattleMenu(Pokemon* myPoke, Pokemon* enemyPoke)
{
    clear();
    printw("%s\n", MESSAGEprint.str().c_str());
    printw("---------------------------------POKEMON BATTLE---------------------------------\n\n");

    //print battle stats (pokemon names & HPs)
    printw("(Your pokemon) %s\n   HP: %d   Lvl: %d\n",myPoke->info->name.c_str(), myPoke->currHP, myPoke->level);
    printw("(Enemy pokemon) %s\n   HP: %d   Lvl: %d\n",enemyPoke->info->name.c_str(), enemyPoke->currHP, enemyPoke->level);
    printw("\n");

    printw("%s\n", MENUprint.str().c_str());
    MENUprint.str("");//clear it

    printw("%s", DEBUGprint.str().c_str());
    DEBUGprint.str("");
    refresh();
}


//TODO: temporary pokemon battle
void pokemonBattle_cutscene(Map* m, Character* npc)
{
    clear();

    printw("\n\n\n\n\n\n------------This is a pokemon battle with NPC #%d! (press anything to continue) ---------\n\n", npc->turnOrder);
    for(int i = 0; i < npc->pokeRoster->size(); i++)
    {
        Pokemon p = npc->pokeRoster->at(i);
        printw("%s lvl %d\n", p.info->name.c_str(), p.level);
    }
    refresh();
    getch();


    Pokemon* myPoke;
    Pokemon* enemyPoke;
    while((myPoke = nextPokemonInRoster(PLAYER->pokeRoster)) != nullptr && (enemyPoke = nextPokemonInRoster(npc->pokeRoster)) != nullptr)
    {
        printw("%s, go beat up their %s\n(press anything to continue)\n", myPoke->info->name.c_str(), enemyPoke->info->name.c_str());
        refresh();
        getch();

        PC_vs_Pokemon_battleLoop(myPoke, enemyPoke, 0);
    }

    //resume game console
    if(nextPokemonInRoster(npc->pokeRoster) == nullptr)
    {
        npc->defeated = 1;//Defeat npc
    }
    clear();
    printMap(m);
    refresh();
}

Pokemon* nextPokemonInRoster(vector<Pokemon>* roster)
{
    for(int i = 0; i < roster->size(); i++)
    {
        if(roster->at(i).currHP>0)
        {
            return &roster->at(i);
        }
    }
    return nullptr;
}



void wildPokemon_cutscene(Map* m)
{
    clear();

    Pokemon wildP(-1,m->manhattan);

    printw("\n\n\n\n\n\n------------A wild %s has appeared !---------\n(press any button to continue)\n", wildP.info->name.c_str());
    refresh();
    getch();

    Pokemon* myPoke;
    int fled = 0;
    while(!fled && wildP.currHP>0 && (myPoke = nextPokemonInRoster(PLAYER->pokeRoster)) != nullptr)
    {
        fled = PC_vs_Pokemon_battleLoop(myPoke, &wildP, 1);
    }

    //resume the game console
    clear();
    printMap(m);
    refresh();
}


// int main(int argc, char* argv[])
// {
//     initscr();
//     noecho();
//     keypad(stdscr, TRUE); 
//     srand(time(NULL));

//     PC_vs_Pokemon_battleLoop(new Pokemon(-1,20),new Pokemon(-1,20));

//     endwin();
//     return 0;
// }