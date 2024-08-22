#include "0_pokemonH.h"


using namespace std;

Pokedex::Pokedex(){
    string path1 = "/pokedex/pokedex/data/csv/pokemon.csv";
    string path2 = "/pokedex/pokedex/data/csv/moves.csv";
    string path3 = "/pokedex/pokedex/data/csv/pokemon_moves.csv";
    string path4 = "/pokedex/pokedex/data/csv/pokemon_species.csv";
    string path5 = "/pokedex/pokedex/data/csv/experience.csv";
    string path6 = "/pokedex/pokedex/data/csv/type_names.csv";
    string path7 = "/pokedex/pokedex/data/csv/pokemon_stats.csv";
    string path8 = "/pokedex/pokedex/data/csv/stats.csv";
    string path9 = "/pokedex/pokedex/data/csv/pokemon_types.csv";

    vector<string> roots = {"/share/cs327","$HOME/.poke327",".."};

    //test all 3 root directories
    int rootDir;
    int found = 0;
    for(rootDir = 0; rootDir < roots.size(); rootDir++)
    {

        string filePath = roots.at(rootDir) + path1;
        ifstream file(filePath);
        if(file.is_open())
        {
            file.close();
            found = 1;
            break;
        }
    }
    if(!found)
    {
        cerr<<"Couldn't find root dir"<<endl;
        return;
    }

    //fill out pokedex
    string filePath1 = roots.at(rootDir) + path1;
    ifstream file1(filePath1);
    pokemonInfoList = importPokemonList(file1);
    file1.close();

    string filePath2 = roots.at(rootDir) + path2;
    ifstream file2(filePath2);
    moveList = importMoveList(file2);
    file2.close();

    string filePath3 = roots.at(rootDir) + path3;
    ifstream file3(filePath3);
    pokemonMoveList = importPokemonMoveList(file3);
    file3.close();

    string filePath4 = roots.at(rootDir) + path4;
    ifstream file4(filePath4);
    speciesList = importSpeciesList(file4);
    file4.close();

    string filePath5 = roots.at(rootDir) + path5;
    ifstream file5(filePath5);
    experienceList = importExperienceList(file5);
    file5.close();

    string filePath6 = roots.at(rootDir) + path6;
    ifstream file6(filePath6);
    typeNameList = importTypeNameList(file6);
    file6.close();

    string filePath7 = roots.at(rootDir) + path7;
    ifstream file7(filePath7);
    pokemonStatsList = importPokemonStatsList(file7);
    file7.close();

    string filePath8 = roots.at(rootDir) + path8;
    ifstream file8(filePath8);
    statsList = importStatsList(file8);
    file8.close();

    string filePath9 = roots.at(rootDir) + path9;
    ifstream file9(filePath9);
    pokemonTypeList = importPokemonTypeList(file9);
    file9.close();


    for(int i = 0; i < moveList.size(); i++)
    {
        Move* m = &moveList.at(i);

        moveTable[m->id]= m;
    }

}


Pokedex* POKEDEX = new Pokedex();


/*
to get random pokemon set csvInd to -1
*/
Pokemon::Pokemon(int csvInd, int manhattanDist)
{
    csvIndex = csvInd;
    gender = (rand()%2==0);
    shiny = (rand()%8192==0);

    //choose pokemon
    if(csvInd == -1)//if -1 get rand pokemon
    {
        int totalPokemon = POKEDEX->pokemonInfoList.size();
        csvIndex = rand()%totalPokemon;
        csvInd = csvIndex; //just in case
        info = &POKEDEX->pokemonInfoList.at(csvIndex);
    }
    else{info = &POKEDEX->pokemonInfoList.at(csvIndex);}

    setInitialLevelNStats(manhattanDist);

    currHP = stats[0];

    initializeMoves(level);

}

void Pokemon::setBaseStats()
{
    //Base stats
    int statIndex = csvIndex*6;
    int i;
    for(i = 0; i < 6; i++){
        baseStats[i] = POKEDEX->pokemonStatsList.at(statIndex+i).base_stat;
    }

    //IVs
    for(i = 0; i < 6; i++){
        IVStats[i] = rand()%16;
    }

}

void Pokemon::setLevel(int lvl)
{
    level = lvl;

    stats[0] = (((baseStats[0] + IVStats[0])*2)*level)/100 + level + 10;

    for(int i = 1; i < 6; i++)
    {
        stats[i] = (((baseStats[i] + IVStats[i])*2)*level)/100 + 5;
    }
}

void Pokemon::levelUp(){
    setLevel(++level);
}

//set initial level based on manhattan distance
void Pokemon::setInitialLevelNStats(int manhattanDist)
{
    setBaseStats();

    int min=0, max=100;
    if(manhattanDist <= 200)
    {
        min = 1;
        max = manhattanDist/2;
        if(max == 0)
        {max = 1;}
    }
    else
    {
        min = (manhattanDist-200)/2;
        max = 100;
    }
     //manhattanDist > 200
    setLevel(getRandMnMx(min,max));
}

/*
TODO: make more performant by remembering the moves
*/
void Pokemon::initializeMoves(int lvl)
{
    //get vector of possible moves
        //get pokemon_id
        //traverse pokedex's pokemon_moves
            //if(conditions) add it

    unordered_map<int, int> possibleMoves;//just a hashmap
    vector<int> posMovList;
    for(PokemonMove pMove : POKEDEX->pokemonMoveList)
    {
        if(pMove.pokemon_id == info->spc_id && pMove.pokemon_move_method_id == 1 && pMove.level <= lvl)
        {
            possibleMoves[pMove.move_id] = pMove.move_id;//this filters duplicates
        }
    }

    //converts hashMap to vector
    for(const auto& pair: possibleMoves)
    {
        posMovList.push_back(pair.first);
    }

    //if empty
        //add struggle to learned moves
    //if not empty
        //while(not empty && learned moves < 2)
            //add move at random
            //remove move from possible

    if(posMovList.size()==0)
    {movesKnown.push_back(POKEDEX->moveTable[165]);}//teach it struggle
    else
    {
        while(posMovList.size() != 0 && movesKnown.size() < 2)
        {
            int index = rand()%posMovList.size(); //gen random index within possibleMoveList size

            Move* m = POKEDEX->moveTable[posMovList.at(index)];//lookup move

            movesKnown.push_back(m); //add to move set

            posMovList.erase(posMovList.begin() + index); //remove from possibleMoveList
        }
    }
}

string Pokemon::toString()
{
    stringstream output;

    output<<csvIndex<<endl;
    output << "Gender: "<< gender << endl;
    output << "Shiny: "<< shiny << endl;
    output << "Name: "<< info->name << " " << info->id << endl;

    output<<"BStats:";

    int i;
    for(i = 0; i < 6; i++){ output<< " " << baseStats[i];}

    output<<endl;

    output<<"IVStats:";

    for(i = 0; i < 6; i++){ output<< " " << IVStats[i];}

    output<<endl;
    output<<"Stats:";

    for(i = 0; i < 6; i++){ output<< " " << stats[i];}

    output<<endl;

    output << "Level: " << level << endl;

    output << "Moves:";
    for(i = 0; i < movesKnown.size(); i++){ output<< " " << movesKnown.at(i)->name;}


    output<<endl;

    return output.str();

}


void startPokemon_cutscene()
{
    clear();

    Pokemon choices[3] = {Pokemon(-1,0),Pokemon(-1,0),Pokemon(-1,0)};

    printw("Choose a number 1-3 (press 'enter' to select)\n");
    
    for(int i = 0; i < 3; i++)
    {
        printw("%s\n",choices[i].info->name.c_str());
    }
    refresh();

    //resume the game console
    char c;
    int choice = 1;
    while((c=getch()) != '\n')
    {
        int digit = c - '0';
        if(digit<=3&&digit>=1)
        {
            choice = digit;
        }
        clear();
        printw("Choose a number 1-3 (press 'enter' to select)\n");
        for(int i = 0; i < 3; i++)
        {
            printw("%s",choices[i].info->name.c_str());
            if(i+1==choice){printw("<-\n");}
            else{printw("\n");}
        }
        refresh();

    }//searching for escape key

    int choiceInd = choice - 1;
    PLAYER->pokeRoster->push_back(choices[choiceInd]);

    clear();
}





