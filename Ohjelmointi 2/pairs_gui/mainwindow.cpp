/* Ohjelman kirjoittaja:
 *
 * Nimi: Katariina Tenhunen
 * Opiskelijanumero: H299784
 * Käyttäjätunnus: kgkate
 * E-Mail: katariina.tenhunen@tuni.fi
 * */

#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QPoint>
#include <QCursor>
#include <QIntValidator>
#include <QDebug>
#include <fstream>
#include <random>
#include <time.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Le FruitGame");

    init_start();
    get_fruits();

    connect(timer, &QTimer::timeout, this, &MainWindow::turn_cards);
    connect(game_timer, &QTimer::timeout, this, &MainWindow::update_time);
}

MainWindow::~MainWindow()
{
    delete ui;
    for(auto player : players)
    {
        delete player;
    }
}

// Kääntää käännetyt kortit piiloon.
void MainWindow::turn_cards()
{
    timer->stop();

    // Yhdistetään painonapit uudelleen slottiin.
    connect(cards_turned.at(0), &QPushButton::clicked,
            this, &MainWindow::button_pressed);
    connect(cards_turned.at(1), &QPushButton::clicked,
            this, &MainWindow::button_pressed);

    if(players.at(0)->is_in_turn())
    {
        player_label->setText(players.at(0)->get_name());
    }
    else
    {
        player_label->setText(players.at(1)->get_name());
    }

    // Käännetään käännetyt kortit piiloon.
    cards_turned.at(0)->setIcon(CARDS_BACK);
    cards_turned.at(0)->setStyleSheet(BG_COLOR);
    cards_turned.at(1)->setIcon(CARDS_BACK);
    cards_turned.at(1)->setStyleSheet(BG_COLOR);

    // Otetaan loput painonapit taad käyttöön.
    for(auto row : rows_)
    {
        for(auto card : row)
        {
            if(card.first != cards_turned.at(0) and card.first != cards_turned.at(1)
                    and std::find(cards_found.begin(), cards_found.end(), card.first)
                    == cards_found.end())
            {
                card.first->setEnabled(true);
            }
        }
    }
}

// Lisää pelin ajastimeen aikaa ja päivittää
// kellon.
void MainWindow::update_time()
{
    ++sec;
    if(sec == 60)
    {
        ++min;
        sec = 0;
    }

    QString time = "";
    if(sec < 10)
    {
        time = QString::number(min) + ":0" + QString::number(sec);
    }
    else
    {
        time = QString::number(min) + ":" + QString::number(sec);
    }

    time_label->setText(time);
}

// Lisää ikkunan avautuessa widgetit näkymään.
void MainWindow::init_start()
{
    QLabel* player1_label = new QLabel("Player 1:");
    QLabel* player2_label = new QLabel("Player 2:");
    QLabel* seed_label = new QLabel("Seed:");
    QLabel* cards_label = new QLabel("Cards:");
    QLabel* timer_label = new QLabel("Time used:");
    QLabel* turn_label = new QLabel("In turn:");

    layout->addWidget(player1_label, 0, 0);
    layout->addWidget(player2_label, 1, 0);
    layout->addWidget(seed_label, 2, 0);
    layout->addWidget(cards_label, 2, 2);
    layout->addWidget(timer_label, 4, 0);
    layout->addWidget(turn_label, 5, 0);
    layout->addWidget(player_label, 5, 1);
    layout->addWidget(name1_line, 0, 1, 1, 2);
    layout->addWidget(name2_line, 1, 1, 1, 2);
    layout->addWidget(seed_line, 2, 1, 1, 2);
    layout->addWidget(spin_box, 2, 3);
    layout->addWidget(start_button, 3, 0);
    layout->addWidget(close_button, 3, 1);
    layout->addWidget(time_label, 4, 1);

    name1_line->setFixedWidth(2 * NARROW_BUTTON_WIDTH);
    name2_line->setFixedWidth(2 * NARROW_BUTTON_WIDTH);
    seed_line->setFixedWidth(NARROW_BUTTON_WIDTH);
    spin_box->setFixedWidth(40);
    start_button->setFixedWidth(NARROW_BUTTON_WIDTH);
    close_button->setFixedWidth(NARROW_BUTTON_WIDTH);

    spin_box->setMaximum(MAX_CARDS);
    spin_box->setMinimum(4);
    spin_box->setSingleStep(2);
    spin_box->setValue(16);

    QIntValidator* ival = new QIntValidator(1, 999, this);
    seed_line->setValidator(ival);

    connect(start_button, &QPushButton::clicked, this, &MainWindow::start_game);
    connect(close_button, &QPushButton::clicked, this, &MainWindow::close);

    setCentralWidget(central);
}

// Slotti start-painonapille. Tulostaa pelilaudan
// ja tulostaulukot.
void MainWindow::start_game()
{
    start_button->setEnabled(false);
    name1_line->setEnabled(false);
    name2_line->setEnabled(false);
    seed_line->setEnabled(false);
    spin_box->setEnabled(false);
    game_timer->start(1000);

    get_players();
    get_seed();
    get_rows_and_columns();

    for(unsigned int r = 0; r < rows; ++r)
    {
        std::vector<std::pair<QPushButton*, QString>> row;

        for(unsigned int c = 0; c < columns; ++c)
        {
           QPushButton* pushButton = new QPushButton();
           row.push_back({pushButton, ""});
           layout->addWidget(pushButton, 6 + r, c);
           pushButton->setFixedWidth(NARROW_BUTTON_WIDTH);
           pushButton->setFixedHeight(DEFAULT_BUTTON_HEIGTH);
           connect(pushButton, &QPushButton::clicked,
                   this, &MainWindow::button_pressed);
           pushButton->setIcon(QIcon(CARDS_BACK));
           pushButton->setIconSize(QSize(NARROW_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGTH));
           pushButton->setStyleSheet(BG_COLOR);
        }
        rows_.push_back(row);
    }
    add_fruits();

    layout->addWidget(player1_label, 7 + rows, 0);
    layout->addWidget(player2_label, 7 + rows, 2);
    layout->addWidget(points1_label, 7 + rows, 1);
    layout->addWidget(points2_label, 7 + rows, 3);
    layout->addWidget(fruits1, 8 + rows, 0, 1, 2);
    layout->addWidget(fruits2, 8 + rows, 2, 1, 2);

    player1_label->setText(players.at(0)->get_name() + ":");
    player2_label->setText(players.at(1)->get_name() + ":");
}

// Hakee pelaajien nimet, sekä luo pelaaja-oliot
void MainWindow::get_players()
{
    QString name1 = name1_line->text();
    QString name2 = name2_line->text();

    if(name1.size() == 0)
    {
        name1 = "Player 1";
    }
    if(name2.size() == 0)
    {
        name2 = "Player 2";
    }

    Player* player1 = new Player(name1);
    Player* player2 = new Player(name2);

    players.at(0) = player1;
    players.at(1) = player2;
    players.at(0)->change_turn();
    player_label->setText(players.at(0)->get_name());
}

// Hakee korttien halutun määrän ja laskee
// korttirivien ja -pylväiden määrän.
void MainWindow::get_rows_and_columns()
{
    int num1 = 2;
    int num2 = 2;
    int dist = 12;
    int dist2 = 0;
    QString cards_str = spin_box->text();

    if(cards_str.size() == 0)
    {
        rows = 4;
        columns = 4;
        return;
    }

    int cards = cards_str.toInt();
    for(int i = 2; i < 5; ++i)
    {
        for(int n = 2; n < 12; ++n)
        {
            if(i * n == cards)
            {
                if(i - n < 0)
                {
                    dist2 = n - i;
                }
                else
                {
                    dist2 = i - n;
                }
                if(dist2 < dist)
                {
                    num1 = n;
                    num2 = i;
                }
            }
        }
    }
    rows = num2;
    columns = num1;
}

// Hakee käyttäjän antaman siemenen tai arpoo
// sen, jos syöte on tyhjä.
void MainWindow::get_seed()
{
    QString seed_str = seed_line->text();
    if(seed_str.size() == 0)
    {
        std::default_random_engine randomEng(time(0));
        std::uniform_int_distribution<int> distr(1, 30);
        distr(randomEng);

        seed = distr(randomEng);
    }
    else
    {
        seed = seed_str.toInt();
    }

}

// Hakee tiedostosta käytössä olevien hedelmien (korttien)
// nimet.
void MainWindow::get_fruits()
{
    std::ifstream file(fruit_filename);
    if(not file)
    {
        qDebug() << "Tiedoston avaus ei onnistu";
        return;
    }
    std::string line = "";
    while(getline(file, line))
    {
        fruits_.push_back({QString::fromStdString(line), 0});
    }
}

// Lisää kortteihin kuvat
void MainWindow::add_fruits()
{
    std::default_random_engine randomEng(seed);
    std::uniform_int_distribution<int> distr(0, rows * columns / 2 - 1);
    distr(randomEng);

    for(auto& row : rows_)
    {
        for(auto& card : row)
        {
            int num = distr(randomEng);
            while(fruits_.at(num).second >= 2)
            {
                num = distr(randomEng);
            }
            card.second = fruits_.at(num).first;
            ++fruits_.at(num).second;
        }
    }
}

// Slotti kortin (painonapin) painamiselle
void MainWindow::button_pressed()
{
    QPoint global_click_position = QCursor::pos();

    int local_x = global_click_position.x() - geometry().x();
    int local_y = global_click_position.y() - geometry().y();
    QPoint local_click_position = QPoint(local_x, local_y);

    // Etsitään painettua nappia
    for(auto row : rows_)
    {
        for(auto card : row)
        {
            if(card.first->geometry().contains(local_click_position))
            {
                QString filename = SUFFIX + card.second + PREFIX;
                card.first->setIcon(QIcon(filename));
                card.first->setStyleSheet(FR_COLOR);
                ++turns;

                // Kaksi korttia käännetty
                if(turns % 2 == 0)
                {
                    cards_turned.at(1) = card.first;
                    if(is_pairs(card.second))
                    {
                        if(players.at(0)->is_in_turn())
                        {
                            update_score(0, card.second);
                        }
                        else
                        {
                            update_score(1, card.second);
                        }
                        cards_turned.at(0)->setEnabled(false);
                        cards_turned.at(1)->setEnabled(false);
                        cards_found.push_back(cards_turned.at(0));
                        cards_found.push_back(cards_turned.at(1));

                        if(is_game_over())
                        {
                            game_timer->stop();
                            print_winner();
                        }
                    }
                    else
                    {
                        players.at(0)->change_turn();
                        players.at(1)->change_turn();
                        card.first->disconnect();
                        set_timer();
                    }
                }
                else
                {
                    cards_turned.at(0) = card.first;
                    card.first->disconnect();
                }
                return;
            }
        }
    }
}

// Päivittää pelin pisteet.
void MainWindow::update_score(int index, const QString fruit)
{
    players.at(index)->add_fruit(fruit);
    QString fruits = players.at(index)->get_fruits();

    if(index == 0)
    {
        points1_label->setText(QString::number(players.at(index)->get_points()) + POINTS);
        fruits1->setText(fruits);
    }
    else
    {
        points2_label->setText(QString::number(players.at(index)->get_points()) + POINTS);
        fruits2->setText(fruits);
    }
}

// Pelin loppuessa tulostaa voittajan tiedot.
void MainWindow::print_winner()
{
    int points1 = players.at(0)->get_points();
    int points2 = players.at(1)->get_points();
    if(points1 > points2)
    {
        winner_label->setText("The winner is " + players.at(0)->get_name() +
                              " with " + points1_label->text() + "! Time used: " +
                              time_label->text() + ".");
    }
    else if(points2 > points1)
    {
        winner_label->setText("The winner is " + players.at(1)->get_name() +
                              " with " + points2_label->text() + "! Time used: " +
                              time_label->text() + ".");
    }
    else
    {
        winner_label->setText("It's a tie with " + points1_label->text() + "! Time used: " +
                              time_label->text() + ".");
    }
    layout->addWidget(winner_label, 10 + rows, 0, 1, 4);
    winner_label->setAlignment(Qt::AlignCenter);
}

// Palauttaa tiedon, onko attribuuttina annettu hedelmä (jälkimmäinen käännetty
// kortti) pari ensimmäisen kortin hedelmän kanssa.
bool MainWindow::is_pairs(const QString fruit)
{
    for(auto row : rows_)
    {
        for(auto card : row)
        {
            if (card.first == cards_turned.at(0) and card.second == fruit)
            {
                 return true;
            }
        }
    }
    return false;
}

// Aloittaa ajastimen ja asettaa piilotetut kortit pois käytöstä.
void MainWindow::set_timer()
{
    for(auto row : rows_)
    {
        for(auto card : row)
        {
            if(card.first != cards_turned.at(0) and card.first != cards_turned.at(1))
            {
                card.first->setEnabled(false);
            }
        }
    }
    timer->start(2000);
}

// Palauttaa tiedon siitä, onko pelilaudalla enää löytämättömiä
// kortteja.
bool MainWindow::is_game_over()
{
    for(auto row : rows_)
        {
            for(auto card : row)
            {
                if (card.first->isEnabled())
                {
                     return false;
                }
            }
        }
    return true;
}
