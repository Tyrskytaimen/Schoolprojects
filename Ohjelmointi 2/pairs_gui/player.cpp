/* Ohjelman kirjoittaja:
 *
 * Nimi: Katariina Tenhunen
 * Opiskelijanumero: H299784
 * Käyttäjätunnus: kgkate
 * E-Mail: katariina.tenhunen@tuni.fi
 * */

#include "player.hh"

Player::Player(const QString& name) : name_(name)
{

}

QString Player::get_name() const
{
    return name_;
}

unsigned int Player::get_points() const
{
    return points_;
}

QString Player::get_fruits() const
{
    QString fruits = "";
    for(auto fruit : fruits_)
    {
        fruits = fruits + fruit + "\n";
    }
    return fruits;
}

void Player::add_fruit(const QString &fruit)
{
    fruits_.push_back(fruit);
    ++points_;
}

void Player::change_turn()
{
    if(has_turn)
    {
        has_turn = false;
    }
    else
    {
        has_turn = true;
    }
}

bool Player::is_in_turn() const
{
    return has_turn;
}
