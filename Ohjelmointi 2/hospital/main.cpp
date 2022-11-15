/* Sairaala
 *
 * Kuvaus:
 *  Ohjelma toteuttaa sairaalan. Sairaalassa on henkilökuntaa, sekä
 * potilaita. Potilaan saapuessa sairaalaan luodaan hoitojakso, johon
 * liitetään potilas. Hoitojaksolle voidaan lisätä henkilökuntaa.
 * Potilaalle voidaan lisäksi lisätä tai poistaa lääkkeitä. Potilaiden
 * poistuessa sairaalasta potilas- ja hoitojaksotiedot jäävät talteen.
 * Henkilökunta ei voi poistua sairaalasta.
 *  Ohjelman aikana käyttäjällä on valittavina useita tiedon hakuun tai
 * henkilöiden muokkaamiseen liittyviä komentoja, joista käyttäjä saa
 * enemmän tietoa komennolla "HELP" tai "H". Lisäksi ohjelma pystyy lukemaan
 * komentoja tiedostosta, sekä muuttamaan sairaalan käyttämää päivämäärää.
 *  Ohjelman suoritus ei pysähdy virheellisen syötteeseen, vaan ohjelma
 * tulostaa aina tilanteeseen sopivan virheilmoituksen ja jää odottamaan
 * uutta komentoa. Jos komentoa ei löydy tai sitä varten ei ole tarpeeksi
 * parametrejä, ilmoittaa ohjelma virheellisestä syötteestä. Jos komennon
 * jälkeen annetut parametrit taas ovat virheelliset, tulostaa ohjelma niitä
 * vastaavan virheilmoituksen.
 *  Ohjelmaa suoritetaan niin kauan, kunnes käyttäjä antaa komennon "QUIT".
 *
 *
 * Ohjelman kirjoittaja:
 *
 * Nimi: Katariina Tenhunen
 * Opiskelijanumero: H299784
 * Käyttäjätunnus: kgkate
 * E-Mail: katariina.tenhunen@tuni.fi
 *
 * */

#include "cli.hh"
#include "hospital.hh"
#include <string>

const std::string PROMPT = "Hosp> ";


int main()
{
    Hospital* hospital = new Hospital();
    Cli cli(hospital, PROMPT);
    while ( cli.exec() ){}

    delete hospital;
    return EXIT_SUCCESS;
}
