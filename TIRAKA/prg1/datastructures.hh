// Datastructures.hh
//
// Student name:    Katariina Tenhunen
// Student email:   katariina.tenhunen@tuni.fi
// Student number:  H299784

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// Kaupunki
struct Town
{
    TownID id;
    Name name;
    Coord coordinates;
    int tax;
    Distance distance;
    TownID mastervassal = NO_TOWNID;
    std::vector<TownID> vassals;
};

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: vakioaikainen
    // Short rationale for estimate: algoritmi size on kaikissa tapauksissa vakioaikainen
    unsigned int town_count();

    // Estimate of performance: lineaarinen
    // Short rationale for estimate: poistaa jokaisen alkion, jolloin poisto tapahtuu n kertaa
    void clear_all();

    // Estimate of performance: vakioaikainen
    // Short rationale for estimate: funktiot find ja [] ovat molemmat vakioaikaisia ja ne
    // suoritetaan vain kerran
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: vakioaikainen
    // Short rationale for estimate: käytetty vain vakioaikaisia algoritmejä
    Name get_town_name(TownID id);

    // Estimate of performance: vakioaikainen
    // Short rationale for estimate: käytetty vain vakioaikaisia algoritmejä
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: vakioaikainen
    // Short rationale for estimate: käytetty vain vakioaikaisia algoritmejä
    int get_town_tax(TownID id);

    // Estimate of performance: lineaarinen
    // Short rationale for estimate: vektoriin lisäys vakioaikainen ja se toistetaan
    // pahimmassa tapauksessa N kertaa.
    std::vector<TownID> all_towns();

    // Estimate of performance: lineaarinen
    // Short rationale for estimate: vektoriin lisäys vakioaikainen ja se toistetaan
    // pahimmassa tapauksessa N kertaa.
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: vakioaikainen
    // Short rationale for estimate: at() keskimäärin vakioaikainen ja arvon
    // vaihtaminen toiseksi on vakoiaikainen operaatio
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: N*log(N)
    // Short rationale for estimate: sort-algoritmin tehokkuus on N*log(N)
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: N*log(N)
    // Short rationale for estimate: sort-algoritmin tehokkuus on N*log(N)
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: lineaarinen
    // Short rationale for estimate: min_element -algoritmi suorittaa
    // N-1 kpl vertailuja
    TownID min_distance();

    // Estimate of performance: lineaarinen
    // Short rationale for estimate: max_element -algoritmi suorittaa
    // N-1 kpl vertailuja
    TownID max_distance();

    // Estimate of performance: vakioaikainen
    // Short rationale for estimate: find -funktion tehokkuus on keskimäärin vakioaikainen
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: lineaarinen
    // Short rationale for estimate: pahimmassa tapauksessa vektoriin lisäys suoritetaan N-1
    // kertaa ja lisäys itsessään on vakioaikainen.
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: lineaarinen
    // Short rationale for estimate: pahimmassa tapauksessa jokaisessa kaupungissa käydään kerran, eikä
    // läpikäynnissä käytetä muita kuin vakioaikaisia operaatioita
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: lineaarinen
    // Short rationale for estimate: käytetään vain lineaarisia operaatioita
    bool remove_town(TownID id);

    // Estimate of performance: N*log(N)
    // Short rationale for estimate: sort-algoritmin tehokkuus on N*log(N)
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance: lineaarinen
    // Short rationale for estimate: pahimmassa tapauksessa jokaisessa kaupungissa käydään kerran, eikä
    // läpikäynnissä käytetä muita kuin vakioaikaisia operaatioita
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: lineaarinen
    // Short rationale for estimate: pahimmassa tapauksessa jokaisessa kaupungissa käydään kerran, eikä
    // läpikäynnissä käytetä muita kuin vakioaikaisia operaatioita
    int total_net_tax(TownID id);

private:
    std::unordered_map<TownID, Town> Towns;

    bool towns_contains(TownID id);
    int count_distance(Coord c, Coord d = {0,0});
    int vassal_net_tax(TownID id); // Kaupungin saamat verotulot
    std::vector<TownID> get_path(TownID id); // Etsii kaupungit, joille kaupunki maksaa veroja suoraan tai epäsuorasti
    std::pair<int,std::vector<TownID>> get_longest_path(TownID id); // Etsii suurimman kaupungin vasallien "jonon"
};

#endif // DATASTRUCTURES_HH
