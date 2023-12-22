//
// Created by rita on 20-10-2022.
//
#pragma once

#ifndef HORARIOSLEIC_CLASS_H
#define HORARIOSLEIC_CLASS_H

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <set>

#include "Slot.h"

class Class {
private:
    std::string ucCode;
    int numStudents;
    std::string classCode;
    unsigned year;
    std::set<Slot> classSchedule;

public:

    //Constructors
    Class();

    Class(std::string classCode, std::string ucCode);

    //Setters
    void setNumStudents(int numStudents);

    void setClassCode(const std::string &classCode);

    void setUcCode(const std::string &ucCode);

    void setClassSchedule(const std::set<Slot> &classSchedule);

    //Getters
    const std::set<Slot> &getClassSchedule() const;

    int getNumStudents() const;

    std::string getClassCode() const;

    std::string getUcCode() const;

    unsigned getYear() const;

    //Operators
    bool operator==(const Class &rhs) const;

    bool operator!=(const Class &rhs) const;

    //Modifiers
    void addSlotToSchedule(const Slot &slot);

    void removeSlotFromSchedule(const Slot &slot);

    void incrementNumStudents();

    void decrementNumStudents();

    //Utilities
    void print();

};


#endif //HORARIOSLEIC_CLASS_H
