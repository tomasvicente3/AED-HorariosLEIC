//
// Created by rita on 20-10-2022.
//

#ifndef HORARIOSLEIC_SLOT_H
#define HORARIOSLEIC_SLOT_H

#include <cmath>
#include <iostream>
#include<string>
#include <vector>
#include <algorithm>

class Slot {
private:
    std::string weekday; //Weekday ser um número? Tinhas de fazer um map depois para converter...
    float startHour, duration; //Manter duration ou usar finishHour? usa o formato que os dados vêm no .csv
    std::string type;
    
public:
    //Constructor
    Slot(std::string weekday, float startHour, float duration, std::string type);

    //Setters 
    void setWeekday(const std::string &weekday);

    void setStartHour(unsigned int startHour);

    void setDuration(unsigned int duration);

    void setType(const std::string &type);

    //Getters
    const std::string &getWeekday() const;

    float getStartHour() const;

    float getDuration() const;

    const std::string &getType() const;

    //Operators
    bool operator<(const Slot &rhs) const;

    bool operator==(const Slot &rhs) const;

    bool operator!=(const Slot &rhs) const;

    //Modifiers
    bool conflictingTime(const Slot &diffSlot);

    bool conflictingSlot(const Slot &diffSlot);

    //Utilities
    void print();

};


#endif //HORARIOSLEIC_SLOT_H
