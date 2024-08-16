#include "0_pokemonH.h"


using namespace std;


class Pokemon{
    public:
    int id;
    string name;
    int spc_id;
    int height;
    int weight;
    int base_exp;
    int order;
    int is_default;

    void print();
    Pokemon(int id, string identifier, int species_id, int h, int w, int base_experience, int order, int is_default);
    Pokemon(string line);
};

class Move{
    public:
    int id;
    string name;
    int gen;
    int type;
    int power;
    int pp;
    int accuracy;
    int priority;
    int target;
    int damage_class;
    int effect;
    int effect_chance;
    int contest_type;
    int contest_effect;
    int super_contest_effect;

    void print();
    Move(int id, string name, int gen, int type, int power, int pp, int accuracy, 
    int priority, int target, int damage_class, int effect, int effect_chance, 
    int contest_type, int contest_effect, int super_contest_effect);
    Move(string line);
};

class PokemonMove {
public:
    int pokemon_id;
    int version_group;
    int move_id;
    int pokemon_move_method_id;
    int level;
    int order;

    PokemonMove(int pokemon_id, int version_group, int move_id, int pokemon_move_method_id, int level, int order);
    PokemonMove(string line);
    void print();
};

class PokemonSpecies {
public:
    int id;
    string name;
    int evolves_from_species_id;
    int evolution_chain_id;
    int color_id;
    int shape_id;
    int habitat_id;
    int gender_rate;
    int capture_rate;
    int base_happiness;
    int is_baby;
    int hatch_counter;
    int has_gender_differences;
    int growth_rate_id;
    int forms_switchable;
    int is_legendary;
    int is_mythical;
    int order;
    int conquest_order;

    PokemonSpecies(int id, string name, int evolves_from_species_id, int evolution_chain_id, int color_id, int shape_id, int habitat_id, int gender_rate, int capture_rate, int base_happiness, int is_baby, int hatch_counter, int has_gender_differences, int growth_rate_id, int forms_switchable, int is_legendary, int is_mythical, int order, int conquest_order);
    PokemonSpecies(string line);
    void print();
};

class Experience {
public:
    int growth_rate_id;
    int level;
    int experience;

    Experience(int growth_rate_id, int level, int experience);
    Experience(string line);
    void print();
};

class TypeName {
public:
    int type_id;
    int local_language;
    string name;

    TypeName(int type_id, int local_language, string name);
    TypeName(string line);
    void print();
};

class PokemonStats {
public:
    int pokemon_id;
    int stat_id;
    int base_stat;
    int effort;

    PokemonStats(int pokemon_id, int stat_id, int base_stat, int effort);
    PokemonStats(string line);
    void print();
};

class Stats {
public:
    int id;
    int damage_class_id;
    string name;
    int is_battle_only;
    int game_index;

    Stats(int id, int damage_class_id, string name, int is_battle_only, int game_index);
    Stats(string line);
    void print();
};

class PokemonType {
public:
    int pokemon_id;
    int type_id;
    int slot;

    PokemonType(int pokemon_id, int type_id, int slot);
    PokemonType(string line);
    void print();
};


int mainParseCSVs(string option);
vector<string> lineParser(const string &s, char splitter);
void printSVector(vector<string> words);
vector<Pokemon> importPokemonList(istream &file);
void printPokemonList(vector<Pokemon> pokedex);
vector<Move> importMoveList(istream& file);
void printMoveList(vector<Move>& moveList);
vector<PokemonMove> importPokemonMoves(istream& file);
void printPokemonMovesList(vector<PokemonMove>& pokemonMovesList);
vector<PokemonSpecies> importPokemonSpeciesList(istream& file);
void printPokemonSpeciesList(vector<PokemonSpecies>& speciesList);
vector<Experience> importExperienceList(istream& file);
void printExperienceList(vector<Experience>& experienceList);
vector<TypeName> importTypeNameList(istream& file);
void printTypeNameList(vector<TypeName>& typeNameList);
vector<PokemonStats> importPokemonStatsList(istream& file);
void printPokemonStatsList(vector<PokemonStats>& pokemonStatsList);
vector<Stats> importStatsList(istream& file);
void printStatsList(vector<Stats>& statsList);
vector<PokemonType> importPokemonTypeList(istream& file);
void printPokemonTypeList(vector<PokemonType>& pokemonTypeList);


/*Terminal start with csv parameter */
int main(int argc, char* argv[])
{
    if (argc == 2) //TODO: check all input types
    {
        string option = argv[1];
        return mainParseCSVs(option);
    }
    else
    {
        printf("Usage:  ./poke <desired csv>\n");
        return 1;
    }

}

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
                    vector<Pokemon> pokemonList = importPokemonList(file);
                    printPokemonList(pokemonList);
                    break;
                }

                case 1:{
                    vector<Move> moveList = importMoveList(file);
                    printMoveList(moveList);
                    break;
                }

                case 2:{
                    vector<PokemonMove> pokemonMovesList = importPokemonMoves(file);
                    printPokemonMovesList(pokemonMovesList);
                    break;
                }

                case 3:{
                    vector<PokemonSpecies> speciesList = importPokemonSpeciesList(file);
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

void printSVector(vector<string> words)
{
    for (const auto& word : words) {
        cout << word << endl;
    }
}



Pokemon::Pokemon(string line)
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

Pokemon::Pokemon(int id, string identifier, int species_id, int h, int w, int base_experience, int order, int is_default)
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
vector<Pokemon> importPokemonList(istream &file)
{
    string line;
    vector<Pokemon> pokedex;

    getline(file,line);//trash first line

    while (getline(file, line))
    {
        Pokemon p(line);
        pokedex.push_back(p);
    }

    return pokedex;
}

void Pokemon::print() {
    cout << id << ", "
         << name << ", "
         << (spc_id == INT_MAX ? "None" : to_string(spc_id)) << ", "
         << (height == INT_MAX ? "None" : to_string(height)) << ", "
         << (weight == INT_MAX ? "None" : to_string(weight)) << ", "
         << (base_exp == INT_MAX ? "None" : to_string(base_exp)) << ", "
         << (order == INT_MAX ? "None" : to_string(order)) << ", "
         << (is_default == INT_MAX ? "None" : to_string(is_default)) << endl;
}

void printPokemonList(vector<Pokemon> pokedex)
{
    for (Pokemon& p : pokedex) {
        p.print();
    }
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




vector<PokemonMove> importPokemonMoves(istream& file) {
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




vector<PokemonSpecies> importPokemonSpeciesList(istream& file) {
    string line;
    vector<PokemonSpecies> speciesList;

    getline(file, line); // Trash the first line (header)

    while (getline(file, line)) {
        PokemonSpecies species(line);
        speciesList.push_back(species);
    }

    return speciesList;
}

PokemonSpecies::PokemonSpecies(string line) {
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

PokemonSpecies::PokemonSpecies(int id, string name, int evolves_from_species_id, int evolution_chain_id, int color_id, int shape_id, int habitat_id, int gender_rate, int capture_rate, int base_happiness, int is_baby, int hatch_counter, int has_gender_differences, int growth_rate_id, int forms_switchable, int is_legendary, int is_mythical, int order, int conquest_order)
    : id(id), name(name), evolves_from_species_id(evolves_from_species_id), evolution_chain_id(evolution_chain_id), color_id(color_id), shape_id(shape_id), habitat_id(habitat_id), gender_rate(gender_rate), capture_rate(capture_rate), base_happiness(base_happiness), is_baby(is_baby), hatch_counter(hatch_counter), has_gender_differences(has_gender_differences), growth_rate_id(growth_rate_id), forms_switchable(forms_switchable), is_legendary(is_legendary), is_mythical(is_mythical), order(order), conquest_order(conquest_order) {}

void PokemonSpecies::print() {
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

void printPokemonSpeciesList(vector<PokemonSpecies>& speciesList) {
    for (PokemonSpecies& species : speciesList) {
        species.print();
        cout << endl;
    }
}




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

void Experience::print() {
    cout << "Growth Rate ID: " << growth_rate_id << ", Level: " << level << ", Experience: " << experience << endl;
}

void printExperienceList(vector<Experience>& experienceList) {
    for (Experience& exp : experienceList) {
        exp.print();
        cout << endl;
    }
}




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

void TypeName::print() {
    cout << "Type ID: " << type_id << ", Local Language: " << local_language << ", Name: " << name << endl;
}

void printTypeNameList(vector<TypeName>& typeNameList) {
    for (TypeName& typeName : typeNameList) {
        typeName.print();
        cout << endl;
    }
}




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

void PokemonStats::print() {
    cout << "Pokemon ID: " << pokemon_id << ", Stat ID: " << stat_id << ", Base Stat: " << base_stat << ", Effort: " << effort << endl;
}

void printPokemonStatsList(vector<PokemonStats>& pokemonStatsList) {
    for (PokemonStats& pokemonStats : pokemonStatsList) {
        pokemonStats.print();
        cout << endl;
    }
}




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

void Stats::print() {
    cout << id << "," << (damage_class_id == INT_MAX ? "None" : to_string(damage_class_id)) << "," << name << "," << (is_battle_only == INT_MAX ? "None" : to_string(is_battle_only)) << "," << (game_index == INT_MAX ? "None" : to_string(game_index)) << endl;
}

void printStatsList(vector<Stats>& statsList) {
    for (Stats& stats : statsList) {
        stats.print();
        cout << endl;
    }
}




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

void PokemonType::print() {
    cout << "Pokemon ID: " << pokemon_id << ", Type ID: " << type_id << ", Slot: " << slot << endl;
}

void printPokemonTypeList(vector<PokemonType>& pokemonTypeList) {
    for (PokemonType& pokemonType : pokemonTypeList) {
        pokemonType.print();
        cout << endl;
    }
}

