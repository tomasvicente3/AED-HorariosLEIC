//
// Created by rita on 20-10-2022.
//

#include "Class.h"


//Constructors
Class::Class() {

}

Class::Class(std::string classCode, std::string ucCode) {
    this->classCode = classCode;
    this->ucCode = ucCode;
    year = classCode[0] - '0';
}


//Setters
void Class::setNumStudents(int numStudents) {
    Class::numStudents = numStudents;
}

void Class::setClassCode(const std::string &classCode) {
    Class::classCode = classCode;
}

void Class::setUcCode(const std::string &ucCode) {
    Class::ucCode = ucCode;
}

void Class::setClassSchedule(const std::set<Slot> &classSchedule) {
    this->classSchedule = classSchedule;
}


//Getters
const std::set<Slot> &Class::getClassSchedule() const {
    return classSchedule;
}

int Class::getNumStudents() const {
    return numStudents;
}

std::string Class::getClassCode() const {
    return classCode;
}

std::string Class::getUcCode() const {
    return ucCode;
}

unsigned Class::getYear() const {
    return year;
}


//Operators
bool Class::operator==(const Class &rhs) const {
    return classCode == rhs.classCode && ucCode == rhs.ucCode;
}

bool Class::operator!=(const Class &rhs) const {
    return !(rhs == *this);
}


//Modifiers
/**
 * Adds a time slot to a schedule.
 * @param slot
 */
void Class::addSlotToSchedule(const Slot &slot) {
    classSchedule.insert(slot);
}
/**
 * Removes a time slot from a schedule.
 * @param slot
 */
void Class::removeSlotFromSchedule(const Slot &slot) {
    classSchedule.erase(slot);
}
/**
 * Increments the number of students of a class by one.
 */
void Class::incrementNumStudents() {
    numStudents++;
}
/**
 * Decrements the number of students of a class by one.
 */
void Class::decrementNumStudents() {
    numStudents--;
}
/**
 * Prints a class, the uc its from and the number of students.
 */
void Class::print() {
    std::cout << "The UC " << ucCode<< "'s class " << classCode << " has " << numStudents << " students.";
}


