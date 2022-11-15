/* COMP.CS.110 K2021
 * Luokka: MainWindow
 * --------------
 *
 * Ohjelman kirjoittaja:
 *
 * Nimi: Katariina Tenhunen
 * Opiskelijanumero: H299784
 * Käyttäjätunnus: kgkate
 * E-Mail: katariina.tenhunen@tuni.fi
 * */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QPushButton>
#include <QTextBrowser>
#include <QGridLayout>
#include <vector>
#include <QLineEdit>
#include <QSpinBox>
#include <QTimer>
#include <QLabel>
#include <QTextBrowser>
#include "player.hh"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer* timer = new QTimer(this); // Ajastin korttien kääntöä varten
    QTimer* game_timer = new QTimer(this); // Peliajan ajastin
    QWidget* central = new QWidget(this);
    QGridLayout* layout = new QGridLayout(central);

    // Korttirivit
    std::vector<std::vector<std::pair<QPushButton*, QString>>> rows_;
    // Hedelmät ja kuinka monessa kortissa ne sijaitsevat
    std::vector<std::pair<QString, int>> fruits_;

    const int MAX_CARDS = 24;
    const int NARROW_BUTTON_WIDTH = 80;
    const int DEFAULT_BUTTON_HEIGTH = 100;
    const QString SUFFIX = "Resources/";
    const QString PREFIX = ".png";
    const QString POINTS = " points";
    const std::string fruit_filename = "fruitnames.txt";
    const QIcon CARDS_BACK = QIcon("back.png");
    const QString BG_COLOR = "background-color:red";
    const QString FR_COLOR = "background-color:white";

    // Widgetit, joihin tulee päästä käsiksi useammasta
    // funktiosta
    QLineEdit* name1_line = new QLineEdit();
    QLineEdit* name2_line = new QLineEdit();
    QLineEdit* seed_line = new QLineEdit();
    QSpinBox* spin_box = new QSpinBox();
    QPushButton* start_button = new QPushButton("Start");
    QPushButton* close_button = new QPushButton("Close");
    QLabel* time_label = new QLabel();
    QLabel* player_label = new QLabel();
    QLabel* player1_label = new QLabel();
    QLabel* player2_label = new QLabel();
    QLabel* points1_label = new QLabel("0" + POINTS);
    QLabel* points2_label = new QLabel("0" + POINTS);
    QLabel* winner_label = new QLabel();
    QTextBrowser* fruits1 = new QTextBrowser();
    QTextBrowser* fruits2 = new QTextBrowser();

    unsigned int rows = 2;
    unsigned int columns = 2;
    unsigned int seed = 1;
    unsigned int turns = 0;
    int sec = 0;
    int min = 0;
    std::vector<QPushButton*> cards_turned{2};
    std::vector<Player*> players{2};
    std::vector<QPushButton*> cards_found;

    void turn_cards();
    void update_time();
    void init_start();
    void start_game();
    void get_players();
    void get_rows_and_columns();
    void get_seed();
    void get_fruits();
    void add_fruits();
    void button_pressed();
    void update_score(int index, const QString fruit);
    void print_winner();
    bool is_pairs(const QString fruit);
    void set_timer();
    bool is_game_over();
};
#endif // MAINWINDOW_HH
