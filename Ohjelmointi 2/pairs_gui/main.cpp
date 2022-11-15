/* Muistipeli
 *
 * Kuvaus:
 *  Ohjelma toteuttaa muistipelin. Pelin alussa käyttäjältä
 * kysytään kahden pelaajan nimet, korttien määrää ja siemenlukua,
 * jonka avulla kortit arvotaan satunnaisesti pelilaudalle.
 *  Joka kierroksella vuorossa oleva pelaaja kääntää kaksi
 * korttia, jonka jälkeen selviää ovatko kortit parit vai ei.
 * Jos kyseessä on pari, muuttuvat kortit harmaiksi, pelaaja
 * saa pisteen ja hän saa jatkaa vuoroaan. Jos kortit eivät
 * ole pari, pysyvät ne vielä 2s ajan käännettyinä, ennen
 * kuin ne käännetään takaisin piiloon ja vuoro siirtyy
 * seuraavalle pelaajalle.
 *  Jos käyttäjä ei syötä pelaajien nimiä, tulee nimiksi
 * Player 1 ja Player 2. Lisäksi tyhjän siemenluvun kohdalla
 * ohjelma arpoo sen. Pelin aikana pelaaja ei pysty vuorollaan
 * kääntämään enempää kuin 2 korttia tai kääntämään jo käännettyä
 * korttia.
 *  Peli päättyy silloin, kun pelilaudalla ei ole enää kortteja.
 * Tällöin kerrotaan pelin voittaja, kuinka monta pistettä
 * hänellä on, sekä peliin kulunut aika. Tasapelitilanteessa
 * kerrotaan pistemäärä ja aika.
 *
 *
 * Ohjelman kirjoittaja:
 *
 * Nimi: Katariina Tenhunen
 * Opiskelijanumero: H299784
 * Käyttäjätunnus: kgkate
 * E-Mail: katariina.tenhunen@tuni.fi
 * */

#include "mainwindow.hh"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
