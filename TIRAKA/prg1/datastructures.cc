// Datastructures.cc
//
// Student name:    Katariina Tenhunen
// Student email:   katariina.tenhunen@tuni.fi
// Student number:  H299784

#include "datastructures.hh"

#include <random>

#include <cmath>
#include <QDebug>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::town_count()
{
    return Towns.size();
}

void Datastructures::clear_all()
{
    Towns.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    if(not towns_contains(id))
    {
        // Luodaan uusi kaupunki annetuilla tiedoilla
        Town new_town;
        new_town.id = id;
        new_town.name = name;
        new_town.coordinates = coord;
        new_town.tax = tax;
        new_town.distance = count_distance(coord);

        Towns[id] = new_town;
        return true;
    }
    else
    {
        return false;
    }
}

Name Datastructures::get_town_name(TownID id)
{
    if(towns_contains(id))
    {
        return Towns.at(id).name;
    }
    else
    {
        return NO_NAME;
    }
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    if(towns_contains(id))
    {
        return Towns.at(id).coordinates;
    }
    else
    {
        return NO_COORD;
    }
}

int Datastructures::get_town_tax(TownID id)
{
    if(towns_contains(id))
    {
        return Towns.at(id).tax;
    }
    else
    {
        return NO_VALUE;
    }
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> town_ids;
    for(auto& town : Towns)
    {
        town_ids.push_back(town.first);
    }
    return town_ids;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> town_names;
    for(auto& town : Towns)
    {
        if(town.second.name == name)
        {
            town_names.push_back(town.first);
        }
    }
    return town_names;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    if(towns_contains(id))
    {
        Towns.at(id).name = newname;
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    std::unordered_map<TownID, Town> &towns_ptr = Towns;
    std::vector<TownID> sorted = all_towns();
    std::sort(sorted.begin(), sorted.end(), [towns_ptr](TownID a, TownID b) {
        return towns_ptr.at(a).name < towns_ptr.at(b).name;});
    return sorted;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    std::unordered_map<TownID, Town> &towns_ptr = Towns;
    std::vector<TownID> sorted = all_towns();
    std::sort(sorted.begin(), sorted.end(), [towns_ptr](TownID a, TownID b) {
        return towns_ptr.at(a).distance < towns_ptr.at(b).distance;});
    return sorted;
}

TownID Datastructures::min_distance()
{
    if(town_count() == 0)
    {
        return NO_TOWNID;
    }
    else
    {
        auto min = std::min_element(Towns.begin(), Towns.end(), [](auto a, auto b) {
            return a.second.distance < b.second.distance;});
        return min->first;
    }
}

TownID Datastructures::max_distance()
{
    if(town_count() == 0)
    {
        return NO_TOWNID;
    }
    else
    {
        auto max = std::max_element(Towns.begin(), Towns.end(), [](auto a, auto b) {
            return a.second.distance < b.second.distance;});
        return max->first;
    }
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    if(towns_contains(vassalid) and towns_contains(masterid) and
            Towns.at(vassalid).mastervassal == NO_TOWNID)
    {
        Towns.at(vassalid).mastervassal = masterid;
        Towns.at(masterid).vassals.push_back(vassalid);
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    std::vector<TownID> town_vassals;
    if(towns_contains(id))
    {
        for(auto& town : Towns)
        {
            if(town.second.mastervassal == id)
            {
                town_vassals.push_back(town.first);
            }
        }
    }
    else
    {
        town_vassals.push_back(NO_TOWNID);
    }
    return town_vassals;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<TownID> tax_path;

    if(not towns_contains(id))
    {
        tax_path.push_back(NO_TOWNID);
    }
    else
    {
       tax_path = get_path(id);
    }
    std::reverse(tax_path.begin(), tax_path.end());
    return tax_path;
}

bool Datastructures::remove_town(TownID id)
{
    if(not towns_contains(id))
    {
        return false;
    }
    else
    {
        // Poistettavan kaupungin tiedot
        auto &to_delete = Towns.at(id);
        TownID master_id = to_delete.mastervassal;
        auto &vassals = to_delete.vassals;
        auto &masters_vassals = Towns.at(master_id).vassals;

        if(master_id != NO_TOWNID)
        {
            // Poistetaan kaupunki isäntäkaupungin vasalleista
            auto position = std::find(masters_vassals.begin(),masters_vassals.end(),id);
            masters_vassals.erase(position);
            // Lisätään poistettavan kaupungin vasallit sen isäntäkaupungin vasalleihin ja
            // vaihdetaan vasallien isäntäkaupunki
            for(auto& vassal : vassals)
            {
                masters_vassals.push_back(vassal);
                Towns.at(vassal).mastervassal = master_id;
            }
        }
        else
        {
            for(auto& vassal : vassals)
            {
                Towns.at(vassal).mastervassal = NO_TOWNID;
            }
        }
        Towns.erase(id);
        return true;
    }
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::vector<std::pair<TownID,Distance>> distances;
    // Kerätään kaupungin id:t ja etäisyydet halutusta pisteestä vektoriin
    for(auto &town : Towns)
    {
        Distance d = count_distance(town.second.coordinates,coord);
        distances.push_back(std::make_pair(town.first,d));
    }
    std::sort(distances.begin(),distances.end(),[](auto a, auto b) {
        return a.second < b.second;});

    // Kerätään aiemmasta sortatusta vektorista vain id:t
    std::vector<TownID> sorted;
    for(auto &town : distances)
        {
            sorted.push_back(town.first);
        }
    return sorted;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    std::vector<TownID> towns;
    if(not towns_contains(id))
    {
        towns.push_back(NO_TOWNID);
    }
    else
    {
        if(Towns.at(id).vassals.empty())
        {
            towns.push_back(id);
        }
        else
        {
            std::pair<int, std::vector<TownID>> pair;
            // Etsitään vasallien pisin "jono"
            pair = get_longest_path(id);
            towns = pair.second;
            // Käännetään vektori oikeaan järjestykseen
            std::reverse(towns.begin(), towns.end());
        }
    }
    return towns;
}

int Datastructures::total_net_tax(TownID id)
{
    if(not towns_contains(id))
    {
        return NO_VALUE;
    }

    // Kaupungin kaikki verotulot
    int tax_sum = Towns.at(id).tax;
    if(not Towns.at(id).vassals.empty())
    {
        for(auto& vassal : Towns.at(id).vassals)
        {
            tax_sum += vassal_net_tax(vassal);
        }
    }

    // Tutkitaan maksaako kaupunki toiselle kaupungille veroja
    if(Towns.at(id).mastervassal == NO_TOWNID)
    {
        return tax_sum;
    }
    else
    {
        return tax_sum * 0.9;
    }
}

bool Datastructures::towns_contains(TownID id)
{
    if(Towns.find(id) != Towns.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Datastructures::count_distance(Coord c, Coord d)
{
    int param = pow((c.x - d.x), 2) + pow((c.y - d.y), 2);
    param = sqrt(param);
    return param;
}

int Datastructures::vassal_net_tax(TownID id)
{
    int sum = Towns.at(id).tax;
    if(Towns.at(id).vassals.empty())
    {
        return sum * 0.1;
    }
    else
    {
        std::vector<TownID> vassals = Towns.at(id).vassals;
        for(auto& vassal : vassals)
        {
            sum += vassal_net_tax(vassal);
        }
        return sum * 0.1;
    }
}

std::vector<TownID> Datastructures::get_path(TownID id)
{
    std::vector<TownID> path;
    if(Towns.at(id).mastervassal == NO_TOWNID)
        {
            path.push_back(id);
        }
        else
        {
            path = taxer_path(Towns.at(id).mastervassal);
            path.push_back(id);
    }
    return path;
}

std::pair<int, std::vector<TownID>> Datastructures::get_longest_path(TownID id)
{
    int l = 0;
    std::pair<int, std::vector<TownID>> pair;
    std::vector<TownID> ids;

    if(not Towns.at(id).vassals.empty())
    {
        for(auto& vassal : Towns.at(id).vassals)
        {
            pair = get_longest_path(vassal);
            if(pair.first > l)
            {
                l += pair.first;
                ids = pair.second;
            }
        }
    }
    l += 1;
    ids.push_back(id);

    return std::make_pair(l,ids);
}
