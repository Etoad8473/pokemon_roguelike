#include "0_pokemonH.h"


//-------------------------------------------1_mapWorldGeneration-------------------------

void printMessage(const char* s)
{
    MESSAGEprint.str("");
    MESSAGEprint<<s;
}

void printMap(Map *m)
{
    clear();
    printw("%s\n", MESSAGEprint.str().c_str());
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            char c = m->map[i][j];
            switch (c) // render raw map
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

            if (m->cMap[i][j])
            {
                char ct = m->cMap[i][j]->character; // character type
                if (ct == 'a')
                {
                    c = '@';
                }
                else
                {
                    c = ct;
                }
            }

            printw("%c", c);
        }
        printw("\n");
    }
    printw("World Coordinates- X:%d Y:%d (index y%d,x%d)\n ", m->worldX - 200, 200 - m->worldY, m->worldY, m->worldX);
    GAMESCREEN->renderAllLines(m);
    GAMESCREEN->printGameScreen();
    
    printw("%s", DEBUGprint.str().c_str());
    DEBUGprint.str("");
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



//---------------------------------------------7_pokemonCSV--------------------------------


void printStringVector(vector<string> words)
{
    for (const auto& word : words) {
        cout << word << endl;
    }
}



void PokemonInfo::print() {
    cout << id << ", "
         << name << ", "
         << (spc_id == INT_MAX ? "None" : to_string(spc_id)) << ", "
         << (height == INT_MAX ? "None" : to_string(height)) << ", "
         << (weight == INT_MAX ? "None" : to_string(weight)) << ", "
         << (base_exp == INT_MAX ? "None" : to_string(base_exp)) << ", "
         << (order == INT_MAX ? "None" : to_string(order)) << ", "
         << (is_default == INT_MAX ? "None" : to_string(is_default)) << endl;
}

void printPokemonList(vector<PokemonInfo>& pokedex)
{
    for (PokemonInfo& p : pokedex) {
        p.print();
    }
}


void Move::print() {
    cout << "ID: " << (id == INT_MAX ? "None" : to_string(id)) << endl;
    cout << "Name: " << name << endl;
    cout << "Generation: " << (gen == INT_MAX ? "None" : to_string(gen)) << endl;
    cout << "Type: " << (type == INT_MAX ? "None" : to_string(type)) << endl;
    cout << "Power: " << (power == INT_MAX ? "None" : to_string(power)) << endl;
    cout << "PP: " << (pp == INT_MAX ? "None" : to_string(pp)) << endl;
    cout << "Accuracy: " << (accuracy == INT_MAX ? "None" : to_string(accuracy)) << endl;
    cout << "Priority: " << (priority == INT_MAX ? "None" : to_string(priority)) << endl;
    cout << "Target: " << (target == INT_MAX ? "None" : to_string(target)) << endl;
    cout << "Damage Class: " << (damage_class == INT_MAX ? "None" : to_string(damage_class)) << endl;
    cout << "Effect: " << (effect == INT_MAX ? "None" : to_string(effect)) << endl;
    cout << "Effect Chance: " << (effect_chance == INT_MAX ? "None" : to_string(effect_chance)) << endl;
    cout << "Contest Type: " << (contest_type == INT_MAX ? "None" : to_string(contest_type)) << endl;
    cout << "Contest Effect: " << (contest_effect == INT_MAX ? "None" : to_string(contest_effect)) << endl;
    cout << "Super Contest Effect: " << (super_contest_effect == INT_MAX ? "None" : to_string(super_contest_effect)) << endl;
}

void printMoveList(vector<Move>& moveList) 
{
    for (Move& move : moveList) 
    {
        move.print();
        cout << endl;
    }
}


void PokemonMove::print() {
    cout << "Pokemon ID: " << (pokemon_id == INT_MAX ? "None" : to_string(pokemon_id)) << endl;
    cout << "Version Group: " << (version_group == INT_MAX ? "None" : to_string(version_group)) << endl;
    cout << "Move ID: " << (move_id == INT_MAX ? "None" : to_string(move_id)) << endl;
    cout << "Pokemon Move Method ID: " << (pokemon_move_method_id == INT_MAX ? "None" : to_string(pokemon_move_method_id)) << endl;
    cout << "Level: " << (level == INT_MAX ? "None" : to_string(level)) << endl;
    cout << "Order: " << (order == INT_MAX ? "None" : to_string(order)) << endl;
}

void printPokemonMovesList(vector<PokemonMove>& pokemonMovesList) {
    for (PokemonMove& pokemonMove : pokemonMovesList) {
        pokemonMove.print();
        cout << endl;
    }
}


void Species::print() {
    cout << id << "," << name << "," << (evolves_from_species_id == INT_MAX ? "None" : to_string(evolves_from_species_id)) << ","
         << (evolution_chain_id == INT_MAX ? "None" : to_string(evolution_chain_id)) << ","
         << (color_id == INT_MAX ? "None" : to_string(color_id)) << ","
         << (shape_id == INT_MAX ? "None" : to_string(shape_id)) << ","
         << (habitat_id == INT_MAX ? "None" : to_string(habitat_id)) << ","
         << (gender_rate == INT_MAX ? "None" : to_string(gender_rate)) << ","
         << (capture_rate == INT_MAX ? "None" : to_string(capture_rate)) << ","
         << (base_happiness == INT_MAX ? "None" : to_string(base_happiness)) << ","
         << (is_baby == INT_MAX ? "None" : to_string(is_baby)) << ","
         << (hatch_counter == INT_MAX ? "None" : to_string(hatch_counter)) << ","
         << (has_gender_differences == INT_MAX ? "None" : to_string(has_gender_differences)) << ","
         << (growth_rate_id == INT_MAX ? "None" : to_string(growth_rate_id)) << ","
         << (forms_switchable == INT_MAX ? "None" : to_string(forms_switchable)) << ","
         << (is_legendary == INT_MAX ? "None" : to_string(is_legendary)) << ","
         << (is_mythical == INT_MAX ? "None" : to_string(is_mythical)) << ","
         << (order == INT_MAX ? "None" : to_string(order)) << ","
         << (conquest_order == INT_MAX ? "None" : to_string(conquest_order)) << endl;
}

void printPokemonSpeciesList(vector<Species>& speciesList) {
    for (Species& species : speciesList) {
        species.print();
        cout << endl;
    }
}


void Experience::print() {
    cout << "Growth Rate ID: " << growth_rate_id << ", Level: " << level << ", Experience: " << experience << endl;
}

void printExperienceList(vector<Experience>& experienceList) {
    for (Experience& exp : experienceList) {
        exp.print();
        cout << endl;
    }
}


void TypeName::print() {
    cout << "Type ID: " << type_id << ", Local Language: " << local_language << ", Name: " << name << endl;
}

void printTypeNameList(vector<TypeName>& typeNameList) {
    for (TypeName& typeName : typeNameList) {
        typeName.print();
        cout << endl;
    }
}


void PokemonStats::print() {
    cout << "Pokemon ID: " << pokemon_id << ", Stat ID: " << stat_id << ", Base Stat: " << base_stat << ", Effort: " << effort << endl;
}

void printPokemonStatsList(vector<PokemonStats>& pokemonStatsList) {
    for (PokemonStats& pokemonStats : pokemonStatsList) {
        pokemonStats.print();
        cout << endl;
    }
}


void Stats::print() {
    cout << id << "," << (damage_class_id == INT_MAX ? "None" : to_string(damage_class_id)) << "," << name << "," << (is_battle_only == INT_MAX ? "None" : to_string(is_battle_only)) << "," << (game_index == INT_MAX ? "None" : to_string(game_index)) << endl;
}

void printStatsList(vector<Stats>& statsList) {
    for (Stats& stats : statsList) {
        stats.print();
        cout << endl;
    }
}


void PokemonType::print() {
    cout << "Pokemon ID: " << pokemon_id << ", Type ID: " << type_id << ", Slot: " << slot << endl;
}

void printPokemonTypeList(vector<PokemonType>& pokemonTypeList) {
    for (PokemonType& pokemonType : pokemonTypeList) {
        pokemonType.print();
        cout << endl;
    }
}

