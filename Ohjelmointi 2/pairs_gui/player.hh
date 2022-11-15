/* COMP.CS.110 K2021
 * Luokka: Player
 * --------------
 * Kuvaa yhtä pelaajaa muistipelissä.
 *
 * Ohjelman kirjoittaja:
 *
 * Nimi: Katariina Tenhunen
 * Opiskelijanumero: H299784
 * Käyttäjätunnus: kgkate
 * E-Mail: katariina.tenhunen@tuni.fi
 * */

#ifndef PLAYER_HH
#define PLAYER_HH

#include <string>
#include <vector>
#include <QString>

class Player
{
public:
    // Rakentaja: luo annetun nimisen pelaajan.
    Player(const QString& name);

    // Palauttaa pelaajan nimen.
    QString get_name() const;

    // Palauttaa pelaajan tähän asti keräämien parien määrän.
    unsigned int get_points() const;

    QString get_fruits() const;

    // Siirtää annetun kortin pelilaudalta pelaajalle,
    // eli lisää kortin pelaajan keräämiin kortteihin
    // ja poistaa sen pelilaudalta.
    void add_fruit(const QString& fruit);

    // Muuttaa pelaajan vuoroa
    void change_turn();

    bool is_in_turn() const;

private:
    QString name_;
    int points_ = 0;
    bool has_turn = false;
    std::vector<QString> fruits_;
};

#endif // PLAYER_HH
