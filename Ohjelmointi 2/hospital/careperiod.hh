/* Class CarePeriod
 * ----------
 * COMP.CS.110 SPRING 2021
 * ----------
 * Class for describing a patient's care period in hospital.
 *
 * Ohjelman kirjoittaja:
 *
 * Nimi: Katariina Tenhunen
 * Opiskelijanumero: H299784
 * Käyttäjätunnus: kgkate
 * E-Mail: katariina.tenhunen@tuni.fi
 * */
#ifndef CAREPERIOD_HH
#define CAREPERIOD_HH

#include "person.hh"
#include "date.hh"
#include "utils.hh"
#include <string>

class CarePeriod
{
public:
    // Constructor, start date given as a string (ddmmyyyy).
    CarePeriod(const std::string& start, Person* patient);

    // Constructor, start date given as a Date object.
    CarePeriod(const Date& start, Person* patient);

    // Destructor.
    ~CarePeriod();

    bool is_active() const;

    std::string get_id() const;

    // Returns true if the careperiod has the staff given
    // as parameter assigned.
    bool has_staffmember(std::string name) const;

    // Prints all information about careperiod.
    void print_information() const;

    // Prints start and end date and patietns name.
    void print_small_info() const;

    // Assigns staff to the careperiod.
    void assign_staff(Person* staff);

    // Sets and end day to the careperiod.
    void set_end(Date& day);

private:
    Person* patient_;
    Date start_;
    Date end_;

    // Staff assigned fot the careperiod
    std::map<std::string, Person*> assigned_staff_;
};

#endif // CAREPERIOD_HH
