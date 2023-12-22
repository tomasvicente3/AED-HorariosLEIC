//
// Created by rita on 20-10-2022.
//

#include "Slot.h"

//Constructor
Slot::Slot(std::string weekday, float startHour, float duration, std::string type) : weekday(weekday),
                                                                                     startHour(startHour),
                                                                                     duration(duration), type(type) {}

//Setters
void Slot::setWeekday(const std::string &weekday) {
    Slot::weekday = weekday;
}

void Slot::setStartHour(unsigned int startHour) {
    Slot::startHour = startHour;
}

void Slot::setDuration(unsigned int duration) {
    Slot::duration = duration;
}

void Slot::setType(const std::string &type) {
    Slot::type = type;
}


//Getters
const std::string &Slot::getWeekday() const {
    return weekday;
}

float Slot::getStartHour() const {
    return startHour;
}

float Slot::getDuration() const {
    return duration;
}

const std::string &Slot::getType() const {
    return type;
}


//Operators
bool Slot::operator<(const Slot &rhs) const {

    std::vector<std::string> weekdays{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    if (find(weekdays.begin(), weekdays.end(), weekday) < find(weekdays.begin(), weekdays.end(), rhs.weekday))
        return true;
    if (find(weekdays.begin(), weekdays.end(), rhs.weekday) < find(weekdays.begin(), weekdays.end(), weekday))
        return false;
    if (startHour < rhs.startHour)
        return true;
    if (rhs.startHour < startHour)
        return false;
    if (duration < rhs.duration)
        return true;
    if (rhs.duration < duration)
        return false;
    return false;
}

bool Slot::operator==(const Slot &rhs) const {
    return weekday == rhs.weekday &&
           startHour == rhs.startHour &&
           duration == rhs.duration &&
           type == rhs.type;
}

bool Slot::operator!=(const Slot &rhs) const {
    return !(rhs == *this);
}
//Comparators
/**
 * Checks if two time slots are conflicting, that is if either of them starts before the other ends.
 * @return Returns true if they are conflicting and false if they aren't.
 */
bool Slot::conflictingTime(const Slot &diffSlot) {
    if ((this->startHour <= diffSlot.getStartHour() && diffSlot.getStartHour() < this->startHour + this->duration) ||
        (diffSlot.getStartHour() <= this->startHour &&
         this->startHour < diffSlot.getStartHour() + diffSlot.getDuration())) {
        return true;
    }
    return false;
}

/**
 * Checks if two slots are conflicting, in other words if they are both "TP" and if they occur on conflicting time slots.
 * @param diffSlot
 * @return Returns true if they are both "TP" and if they have conflicting time slots, otherwise returns false.
 */
bool Slot::conflictingSlot(const Slot &diffSlot) {
    if (this->weekday == diffSlot.getWeekday() && this->type == "TP" && diffSlot.getType() == "TP" &&
        conflictingTime(diffSlot)) {
        return true;
    }
    return false;
}

/**
 * Prints a slot and its details.
 */
void Slot::print() {
    std::cout << "This " << type << " lesson will take place on " << weekday << " at ";
    float hour;
    float minute = std::modf(startHour, &hour);
    std::cout << hour << "h" << minute * 60;
    std::cout << " and will last " << duration << " hours." << "\n";
}


