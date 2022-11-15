/* Ohjelman kirjoittaja:
 *
 * Nimi: Katariina Tenhunen
 * Opiskelijanumero: H299784
 * Käyttäjätunnus: kgkate
 * E-Mail: katariina.tenhunen@tuni.fi
 * */

#include "careperiod.hh"
#include <iostream>

CarePeriod::CarePeriod(const std::string& start, Person* patient):
    patient_(patient), start_(start)
{
    end_.set(0, 0, 0);
}

CarePeriod::CarePeriod(const Date &start, Person* patient):
    patient_(patient), start_(start)
{
}

CarePeriod::~CarePeriod()
{
}

bool CarePeriod::is_active() const
{
    return end_.is_default();
}

std::string CarePeriod::get_id() const
{
    return patient_->get_id();
}

bool CarePeriod::has_staffmember(std::string name) const
{
    for( auto member : assigned_staff_ ) {
        if( member.first == name ) {
            return true;
        }
    }
    return false;
}

void CarePeriod::print_small_info() const
{
    start_.print();
    std::cout << " - ";
    if( not is_active() ) {
        end_.print();
    }
    std::cout << std::endl;
    std::cout << "* Patient: " << get_id() << std::endl;
}

void CarePeriod::print_information() const
{
    std::cout << "* Care period: ";
    start_.print();
    std::cout << " - ";
    if( not is_active() ) {
        end_.print();
    }
    std::cout << std::endl;
    std::cout << "  - Staff:";
    if( assigned_staff_.size() == 0 ) {
        std::cout <<" None";
    }
    else {
        for( auto staff_member : assigned_staff_ ) {
            std::cout << " " << staff_member.first;
        }
    }
    std::cout << std::endl;
}

void CarePeriod::assign_staff(Person *staff)
{
    assigned_staff_.insert({staff->get_id(), staff});
}


void CarePeriod::set_end(Date& day)
{
    end_ = day;
}
