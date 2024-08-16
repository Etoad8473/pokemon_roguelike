#include "0_pokemonH.h"





int mainParseCSVs(string option)
{

    vector<string> roots = {"/share/cs327","$HOME/.poke327",".."};
    string path = "/pokedex/pokedex/data/csv/";
    vector<string> fileNames = {"pokemon","moves","pokemon_moves","pokemon_species","experience","type_names","pokemon_stats","stats","pokemon_types"};

    int fileID;
    int found = 0;
    for(fileID = 0; fileID < fileNames.size(); fileID++)
    {
        if(option.compare(fileNames.at(fileID))==0)
        {
            found = 1;
            break;
        }
    }
    if(found == 0)
    {
        cerr << "invalid input"<<endl;
        return 1;
    }

    ifstream file;
    int i;
    for(i = 0; i < roots.size(); i++)
    {
        string filePath = roots.at(i) + path + fileNames.at(fileID) + ".csv";
        ifstream file(filePath);

        if(file.is_open())
        {
            switch(fileID)
            {
                case 0:{
                    vector<PokemonInfo> pokemonInfoList = importPokemonList(file);
                    printPokemonList(pokemonInfoList);
                    break;
                }

                case 1:{
                    vector<Move> moveList = importMoveList(file);
                    printMoveList(moveList);
                    break;
                }

                case 2:{
                    vector<PokemonMove> pokemonMovesList = importPokemonMoveList(file);
                    printPokemonMovesList(pokemonMovesList);
                    break;
                }

                case 3:{
                    vector<Species> speciesList = importSpeciesList(file);
                    printPokemonSpeciesList(speciesList);
                    break;
                }

                case 4:{
                    vector<Experience> experienceList = importExperienceList(file);
                    printExperienceList(experienceList);
                    break;
                }

                case 5:{
                    vector<TypeName> typeNameList = importTypeNameList(file);
                    printTypeNameList(typeNameList);
                    break;
                }

                case 6:{
                    vector<PokemonStats> pokemonStatsList = importPokemonStatsList(file);
                    printPokemonStatsList(pokemonStatsList);
                    break;
                }

                case 7:{
                    vector<Stats> statsList = importStatsList(file);
                    printStatsList(statsList);
                    break;
                }

                case 8:{
                    vector<PokemonType> pokemonTypeList = importPokemonTypeList(file);
                    printPokemonTypeList(pokemonTypeList);
                    break;
                }

                default:
                    cerr << "something went wrong\n";
                    return 1;
            }
            return 0; //switch worked correctly, finished printing list
        }
    }
    cerr << "file not found"<<endl;
    return 1;

}

vector<string> lineParser(const string &s, char splitter)
{

    vector<string> words;
    stringstream ss(s);
    string word;
    while(getline(ss, word, splitter))
    {
        words.push_back(word);
    }

    return words;

}



PokemonInfo::PokemonInfo(string line)
{
    vector<string> words = lineParser(line, ','); // Split the line into fields
    
    for(string &s: words)//sets empty fields to INT_MAX
    {
        if(s.size()==0)
        {
            s = "2147483647";
        }
    }

    id = stoi(words.at(0));
    name = words.at(1);
    spc_id = stoi(words.at(2));
    height = stoi(words.at(3));
    weight = stoi(words.at(4));
    base_exp = stoi(words.at(5));
    order = stoi(words.at(6));
    is_default = stoi(words.at(7));

}

PokemonInfo::PokemonInfo(int id, string identifier, int species_id, int h, int w, int base_experience, int order, int is_default)
{
    this->id = id;
    name = identifier;
    spc_id = species_id;
    height = h;
    weight = w;
    base_exp = base_experience;
    this->order = order;
    this->is_default = is_default;
}

/*trashes first line*/
vector<PokemonInfo> importPokemonList(istream &file)
{
    string line;
    vector<PokemonInfo> pokedex;

    getline(file,line);//trash first line

    while (getline(file, line))
    {
        PokemonInfo p(line);
        pokedex.push_back(p);
    }

    return pokedex;
}



Move::Move(string line)
{
    vector<string> fields = lineParser(line, ','); // Split the line into fields

    int numFields = 15;

    if(fields.size()<numFields)//add a final field if csv had empty final field
    {
        fields.push_back("");
    }
    for(string &s: fields)//sets empty fields to INT_MAX
    {
        if(s.size()==0)
        {
            s = "2147483647";
        }
    }

    id = stoi(fields.at(0));
    name = fields.at(1);
    gen = stoi(fields.at(2));
    type = stoi(fields.at(3));
    power = stoi(fields.at(4));
    pp = stoi(fields.at(5));
    accuracy = stoi(fields.at(6));
    priority = stoi(fields.at(7));
    target = stoi(fields.at(8));
    damage_class = stoi(fields.at(9));
    effect = stoi(fields.at(10));
    effect_chance = stoi(fields.at(11));
    contest_type = stoi(fields.at(12));
    contest_effect = stoi(fields.at(13));
    super_contest_effect = stoi(fields.at(14));
}

Move::Move(int id, string name, int gen, int type, int power, int pp, int accuracy, int priority, int target, int damage_class, int effect, int effect_chance, int contest_type, int contest_effect, int super_contest_effect)
    : id(id), name(name), gen(gen), type(type), power(power), pp(pp), accuracy(accuracy), priority(priority), target(target), damage_class(damage_class), effect(effect), effect_chance(effect_chance), contest_type(contest_type), contest_effect(contest_effect), super_contest_effect(super_contest_effect) {}

/*trashes first line*/
vector<Move> importMoveList(istream& file) {
    string line;
    vector<Move> moveList;

    getline(file, line); // Trash the first line (header)

    while (getline(file, line)) {
        Move m(line);
        moveList.push_back(m);
    }

    return moveList;
}



vector<PokemonMove> importPokemonMoveList(istream& file) {
    string line;
    vector<PokemonMove> pokemonMovesList;

    getline(file, line); // Trash the first line (header)

    while (getline(file, line)) {
        PokemonMove pm(line);
        pokemonMovesList.push_back(pm);
    }

    return pokemonMovesList;
}

PokemonMove::PokemonMove(string line) {
    vector<string> fields = lineParser(line, ',');

    int numFields = 6;

    if(fields.size()<numFields)//add a final field if csv had empty final field
    {
        fields.push_back("");
    }
    for(string &s: fields)//sets empty fields to INT_MAX
    {
        if(s.size()==0)
        {
            s = "2147483647";
        }
    }

    pokemon_id = stoi(fields.at(0));
    version_group = stoi(fields.at(1));
    move_id = stoi(fields.at(2));
    pokemon_move_method_id = stoi(fields.at(3));
    level = stoi(fields.at(4));
    order = stoi(fields.at(5));
}

PokemonMove::PokemonMove(int pokemon_id, int version_group, int move_id, int pokemon_move_method_id, int level, int order)
    : pokemon_id(pokemon_id), version_group(version_group), move_id(move_id), pokemon_move_method_id(pokemon_move_method_id), level(level), order(order) {}

// void PokemonMove::print() {
//     cout << "Pokemon ID: " << pokemon_id << ", Version Group: " << version_group << ", Move ID: " << move_id << ", Pokemon Move Method ID: " << pokemon_move_method_id << ", Level: " << level << ", Order: " << order << endl;
// }



vector<Species> importSpeciesList(istream& file) {
    string line;
    vector<Species> speciesList;

    getline(file, line); // Trash the first line (header)

    while (getline(file, line)) {
        Species species(line);
        speciesList.push_back(species);
    }

    return speciesList;
}

Species::Species(string line) {
    vector<string> fields = lineParser(line, ',');

    int numFields = 20;

    if(fields.size()<numFields)//add a final field if csv had empty final field
    {
        fields.push_back("");
    }
    for(string &s: fields)//sets empty fields to INT_MAX
    {
        if(s.size()==0)
        {
            s = "2147483647";
        }
    }

    id = stoi(fields.at(0));
    name = fields.at(1);
    evolves_from_species_id = stoi(fields.at(2));
    evolution_chain_id = stoi(fields.at(3));
    color_id = stoi(fields.at(4));
    shape_id = stoi(fields.at(5));
    habitat_id = stoi(fields.at(6));
    gender_rate = stoi(fields.at(7));
    capture_rate = stoi(fields.at(8));
    base_happiness = stoi(fields.at(9));
    is_baby = stoi(fields.at(10));
    hatch_counter = stoi(fields.at(11));
    has_gender_differences = stoi(fields.at(12));
    growth_rate_id = stoi(fields.at(13));
    forms_switchable = stoi(fields.at(14));
    is_legendary = stoi(fields.at(15));
    is_mythical = stoi(fields.at(16));
    order = stoi(fields.at(17));
    conquest_order = stoi(fields.at(18));
}

Species::Species(int id, string name, int evolves_from_species_id, int evolution_chain_id, int color_id, int shape_id, int habitat_id, int gender_rate, int capture_rate, int base_happiness, int is_baby, int hatch_counter, int has_gender_differences, int growth_rate_id, int forms_switchable, int is_legendary, int is_mythical, int order, int conquest_order)
    : id(id), name(name), evolves_from_species_id(evolves_from_species_id), evolution_chain_id(evolution_chain_id), color_id(color_id), shape_id(shape_id), habitat_id(habitat_id), gender_rate(gender_rate), capture_rate(capture_rate), base_happiness(base_happiness), is_baby(is_baby), hatch_counter(hatch_counter), has_gender_differences(has_gender_differences), growth_rate_id(growth_rate_id), forms_switchable(forms_switchable), is_legendary(is_legendary), is_mythical(is_mythical), order(order), conquest_order(conquest_order) {}



vector<Experience> importExperienceList(istream& file) {
    string line;
    vector<Experience> experienceList;

    getline(file, line); // Trash the first line (header)

    while (getline(file, line)) {
        Experience exp(line);
        experienceList.push_back(exp);
    }

    return experienceList;
}

Experience::Experience(string line) {
    vector<string> fields = lineParser(line, ',');

    growth_rate_id = stoi(fields.at(0));
    level = stoi(fields.at(1));
    experience = stoi(fields.at(2));
}

Experience::Experience(int growth_rate_id, int level, int experience)
    : growth_rate_id(growth_rate_id), level(level), experience(experience) {}



vector<TypeName> importTypeNameList(istream& file) {
    string line;
    vector<TypeName> typeNameList;

    getline(file, line); // Trash the first line (header)

    while (getline(file, line)) {
        TypeName typeName(line);
        typeNameList.push_back(typeName);
    }

    return typeNameList;
}

TypeName::TypeName(string line) {
    vector<string> fields = lineParser(line, ',');

    type_id = stoi(fields.at(0));
    local_language = stoi(fields.at(1));
    name = fields.at(2);
}

TypeName::TypeName(int type_id, int local_language, string name)
    : type_id(type_id), local_language(local_language), name(name) {}



vector<PokemonStats> importPokemonStatsList(istream& file) {
    string line;
    vector<PokemonStats> pokemonStatsList;

    getline(file, line); // Trash the first line (header)

    while (getline(file, line)) {
        PokemonStats pokemonStats(line);
        pokemonStatsList.push_back(pokemonStats);
    }

    return pokemonStatsList;
}

PokemonStats::PokemonStats(string line) {
    vector<string> fields = lineParser(line, ',');

    
    int numFields = 4;

    if(fields.size()<numFields)//add a final field if csv had empty final field
    {
        fields.push_back("");
    }
    for(string &s: fields)//sets empty fields to INT_MAX
    {
        if(s.size()==0)
        {
            s = "2147483647";
        }
    }

    pokemon_id = stoi(fields.at(0));
    stat_id = stoi(fields.at(1));
    base_stat = stoi(fields.at(2));
    effort = stoi(fields.at(3));
}

PokemonStats::PokemonStats(int pokemon_id, int stat_id, int base_stat, int effort)
    : pokemon_id(pokemon_id), stat_id(stat_id), base_stat(base_stat), effort(effort) {}



vector<Stats> importStatsList(istream& file) {
    string line;
    vector<Stats> statsList;

    getline(file, line); // Trash the first line (header)

    while (getline(file, line)) {
        Stats stats(line);
        statsList.push_back(stats);
    }

    return statsList;
}

Stats::Stats(string line) {
    vector<string> fields = lineParser(line, ',');

    int numFields = 5;

    if(fields.size()<numFields)//add a final field if csv had empty final field
    {
        fields.push_back("");
    }
    for(string &s: fields)//sets empty fields to INT_MAX
    {
        if(s.size()==0)
        {
            s = "2147483647";
        }
    }

    id = stoi(fields.at(0));
    damage_class_id = stoi(fields.at(1));
    name = fields.at(2);
    is_battle_only = stoi(fields.at(3));
    game_index = stoi(fields.at(4));
}

Stats::Stats(int id, int damage_class_id, string name, int is_battle_only, int game_index)
    : id(id), damage_class_id(damage_class_id), name(name), is_battle_only(is_battle_only), game_index(game_index) {}



vector<PokemonType> importPokemonTypeList(istream& file) {
    string line;
    vector<PokemonType> pokemonTypeList;

    getline(file, line); // Trash the first line (header)

    while (getline(file, line)) {
        PokemonType pokemonType(line);
        pokemonTypeList.push_back(pokemonType);
    }

    return pokemonTypeList;
}

PokemonType::PokemonType(string line) {
    vector<string> fields = lineParser(line, ',');

    pokemon_id = stoi(fields.at(0));
    type_id = stoi(fields.at(1));
    slot = stoi(fields.at(2));
}

PokemonType::PokemonType(int pokemon_id, int type_id, int slot)
    : pokemon_id(pokemon_id), type_id(type_id), slot(slot) {}
