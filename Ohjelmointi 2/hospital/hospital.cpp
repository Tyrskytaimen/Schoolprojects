/* Ohjelman kirjoittaja:
 *
 * Nimi: Katariina Tenhunen
 * Opiskelijanumero: H299784
 * Käyttäjätunnus: kgkate
 * E-Mail: katariina.tenhunen@tuni.fi
 * */

#include "hospital.hh"
#include "utils.hh"
#include <iostream>
#include <algorithm>
#include <set>

Hospital::Hospital()
{
}

Hospital::~Hospital()
{
    // Deallocating staff
    for( std::map<std::string, Person*>::iterator
         iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        delete iter->second;
    }

    // Remember to deallocate patients also
    for( std::map<std::string, Person*>::iterator
         iter = all_patients_.begin();
         iter != all_patients_.end();
         ++iter )
    {
        delete iter->second;
    }

    // Deallocating careperiods
    for( auto& period : careperiods_ ) {
        delete period;
    }
}

void Hospital::recruit(Params params)
{
    std::string specialist_id = params.at(0);
    if( staff_.find(specialist_id) != staff_.end() )
    {
        std::cout << ALREADY_EXISTS << specialist_id << std::endl;
        return;
    }
    Person* new_specialist = new Person(specialist_id);
    staff_.insert({specialist_id, new_specialist});
    std::cout << STAFF_RECRUITED << std::endl;
}

void Hospital::enter(Params params)
{
    std::string patient_id = params.at(0);
    if( current_patients_.find(patient_id) != current_patients_.end() )
    {
        std::cout << ALREADY_EXISTS << patient_id << std::endl;
        return;
    }
    if( all_patients_.find(patient_id) == all_patients_.end() ) {
        // Creates new patient
        Person* new_patient = new Person(patient_id);
        current_patients_.insert({patient_id, new_patient});
        all_patients_.insert({patient_id, new_patient});
        CarePeriod* new_careperiod = new CarePeriod(utils::today, new_patient);
        careperiods_.push_back(new_careperiod);
    }
    else {
        // Retrieves old patients information
        CarePeriod* new_careperiod = new CarePeriod(utils::today, all_patients_.at(patient_id));
        current_patients_.insert({patient_id, all_patients_.at(patient_id)});
        careperiods_.push_back(new_careperiod);
    }
    std::cout << PATIENT_ENTERED << std::endl;
}

void Hospital::leave(Params params)
{
    std::string patient_id = params.at(0);
    if( current_patients_.find(patient_id) == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }
    current_patients_.erase(patient_id);
    for( auto careperiod : careperiods_ ) {
        if( careperiod->get_id() == patient_id and careperiod->is_active() ) {
            careperiod->set_end(utils::today);
            break;
        }
    }
    std::cout << PATIENT_LEFT << std::endl;
}

void Hospital::assign_staff(Params params)
{
    std::string staff_id = params.at(0);
    std::string patient_id = params.at(1);    
    if( staff_.find(staff_id) == staff_.end() )
    {
        std::cout << CANT_FIND << staff_id << std::endl;
        return;
    }
    if( current_patients_.find(patient_id) == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }
    for( auto careperiod : careperiods_ ) {
        if( careperiod->get_id() == patient_id and careperiod->is_active() ) {
            careperiod->assign_staff(staff_.at(staff_id));
            break;
        }
    }
    std::cout << STAFF_ASSIGNED << patient_id << std::endl;
}

void Hospital::add_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string strength = params.at(1);
    std::string dosage = params.at(2);
    std::string patient = params.at(3);
    if( not utils::is_numeric(strength, true) or
        not utils::is_numeric(dosage, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->add_medicine(medicine, stoi(strength), stoi(dosage));
    std::cout << MEDICINE_ADDED << patient << std::endl;
}

void Hospital::remove_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string patient = params.at(1);
    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->remove_medicine(medicine);
    std::cout << MEDICINE_REMOVED << patient << std::endl;
}

void Hospital::print_patient_info(Params params)
{
    std::string patient_id = params.at(0);
    if( all_patients_.find(patient_id) == all_patients_.end() )
    {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }
    for( auto careperiod : careperiods_ ) {
        if( careperiod->get_id() == patient_id ) {
            careperiod->print_information();
        }
    }
    std::cout << "* Medicines:";
    all_patients_.at(patient_id)->print_medicines("  - ");
}

void Hospital::print_care_periods_per_staff(Params params)
{
    std::string staff_id = params.at(0);
    if( staff_.find(staff_id) == staff_.end() )
    {
        std::cout << CANT_FIND << staff_id << std::endl;
        return;
    }
    bool careperiod_found = false;
    for( auto careperiod : careperiods_ ) {
        if( careperiod->has_staffmember(staff_id) ) {
            careperiod->print_small_info();
            careperiod_found = true;
        }
    }
    if( not careperiod_found ) {
        std::cout << "None" << std::endl;
    }
}

void Hospital::print_all_medicines(Params)
{
    std::map<std::string, std::vector<std::string>> medicines; // All medicine used in the hospital
    for( auto patient : all_patients_ ) {
        std::vector<std::string> patients_medicines = patient.second->get_medicines();
        for( auto medicine : patients_medicines ) {
            if( medicines.find(medicine) != medicines.end() ) {
                medicines.at(medicine).push_back(patient.second->get_id());
            }
            else {
                medicines.insert({medicine, {patient.second->get_id()}});
            }
        }
    }
    if( medicines.size() == 0 ) {
        std::cout << "None" << std::endl;
    }
    else {
        for( auto medicine : medicines ) {
            std::cout << medicine.first << " prescribed for" << std::endl;
            sort(medicine.second.begin(), medicine.second.end());
            for( std::string patient : medicine.second ) {
                std::cout << "* " << patient << std::endl;
            }
        }
    }
}

void Hospital::print_all_staff(Params)
{
    if( staff_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }
    for( std::map<std::string, Person*>::const_iterator iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        std::cout << iter->first << std::endl;
    }
}

void Hospital::print_all_patients(Params)
{
    if( all_patients_.size() == 0 ) {
        std::cout << "None" << std::endl;
        return;
    }
    for( auto patient : all_patients_ ) {
        std::cout << patient.second->get_id() << std::endl;
        for( auto careperiod : careperiods_ ) {
            if( careperiod->get_id() == patient.second->get_id() ) {
               careperiod->print_information();
            }
        }
        std::cout << "* Medicines:";
        patient.second->print_medicines("  - ");
    }
}

void Hospital::print_current_patients(Params)
{
    if( current_patients_.size() == 0 ) {
        std::cout << "None" << std::endl;
        return;
    }
    for( auto patient : current_patients_ ) {
        std::cout << patient.second->get_id() << std::endl;
        for( auto careperiod : careperiods_ ) {
            if( careperiod->get_id() == patient.second->get_id() ) {
               careperiod->print_information();
            }
        }
        std::cout << "* Medicines:";
        patient.second->print_medicines("  - ");
    }
}

void Hospital::set_date(Params params)
{
    std::string day = params.at(0);
    std::string month = params.at(1);
    std::string year = params.at(2);
    if( not utils::is_numeric(day, false) or
        not utils::is_numeric(month, false) or
        not utils::is_numeric(year, false) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.set(stoi(day), stoi(month), stoi(year));
    std::cout << "Date has been set to ";
    utils::today.print();
    std::cout << std::endl;
}

void Hospital::advance_date(Params params)
{
    std::string amount = params.at(0);
    if( not utils::is_numeric(amount, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.advance(stoi(amount));
    std::cout << "New date is ";
    utils::today.print();
    std::cout << std::endl;
}
