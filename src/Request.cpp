//
// Created by rita on 20-10-2022.
//

#include "Request.h"

//Constructors

Request::Request(const std::shared_ptr<Student> &student, const std::shared_ptr<UC> &uc,
                 const std::shared_ptr<Class> &originClass, const std::shared_ptr<Class> &destinationClass) : student(
        student), originClass(originClass), destinationClass(destinationClass), uc(uc) {}

//Setters
void Request::setStudent(const std::shared_ptr<Student> &student) {
    Request::student = student;
}

void Request::setOriginClass(const std::shared_ptr<Class> &originClass) {
    Request::originClass = originClass;
}

void Request::setDestinationClass(const std::shared_ptr<Class> &destinationClass) {
    Request::destinationClass = destinationClass;
}

void Request::setUc(const std::shared_ptr<UC> &uc) {
    Request::uc = uc;
}


//Getters
std::shared_ptr<Student> Request::getStudent() const {
    return student;
}

std::shared_ptr<Class> Request::getOriginClass() const {
    return originClass;
}

std::shared_ptr<Class> Request::getDestinationClass() const {
    return destinationClass;
}

std::shared_ptr<UC> Request::getUc() const {
    return uc;
}


//Operators
bool Request::operator==(const Request &rhs) const {
    return student == rhs.student &&
           originClass == rhs.originClass &&
           destinationClass == rhs.destinationClass &&
           uc == rhs.uc;
}

bool Request::operator!=(const Request &rhs) const {
    return !(rhs == *this);
}

//Comparators

/**
 * Compares two requests and checks if they are complementing, that is if they were made by two different people that want to trade to each other's class
 * @param request a
 * @param request b
 * @return Returns true if the two requests are complementing and false if they arent.
 */
bool Request::complementingRequest(const std::shared_ptr<Request> &request1, const std::shared_ptr<Request> &request2) {
    if (request1->student != request2->getStudent() && request1->originClass == request2->getDestinationClass() &&
        request1->destinationClass == request2->getOriginClass())
        return true;
    return false;
}

/**
 * Checks to see if the classes of a certain Uc will stay balanced after a student request to trade classes.
 * @return Returns true if the classes will remain balanced after the trade request is realized and false if they won't.
 */
bool Request::checkReqClassBalanced() {
    std::vector<int> numStudentsPerClass;
    std::vector<std::shared_ptr<Class>> classesInUc = uc->getClassesPerUc();
    for (auto it = classesInUc.begin(); it < classesInUc.end(); it++) {
        if ((*it) == destinationClass) {
            numStudentsPerClass.push_back((*it)->getNumStudents() + 1);
        } else if ((*it) == originClass) {
            numStudentsPerClass.push_back((*it)->getNumStudents() - 1);
        } else {
            numStudentsPerClass.push_back((*it)->getNumStudents());
        }
    }
    return UC::checkIfBalanced(numStudentsPerClass);
}

//Modifiers
/**
 * Stages a temporary schedule made from the original student's schedule.
 * @return Returns a vector of schedule blocks, std::vector<Student::scheduleBlock>, that acts as the temporary schedule.
 */
std::vector<Student::scheduleBlock> Request::stageScheduleChange() {
    Student temp = *student;
    if (destinationClass == nullptr) temp.removeClass(uc, originClass); //Remove student from class
    else if (originClass == nullptr) temp.addClass(uc, destinationClass); //Add student to class
    else temp.changeUcClass(uc, destinationClass); //Change student from one class to another

    return temp.getSchedule();
}

/**
 * Changes the schedule of a student.
 */
void Request::changeSchedule() {
    if (originClass == nullptr) { //Add student to a class
        student->addClass(uc, destinationClass);
        return;
    }
    if (destinationClass == nullptr) { //Remove student from a class
        student->removeClass(uc, originClass);
        return;
    }
    student->changeUcClass(uc, destinationClass); //Move a student from one class to another
}

/**
 * Receives a schedule of a student and makes changes to it according to the request.
 * @param schedule
 * @return Returns a vector of scheduleblocks, std::vector<Student::scheduleBlock>, representing the new schedule.
 */
std::vector<Student::scheduleBlock> Request::stageScheduleChangeOnSchedule(std::vector<Student::scheduleBlock> schedule) {

    if (originClass != nullptr) { //If removing or changing a class
        for (auto it = schedule.begin(); it != schedule.end(); it++) {
            Student::scheduleBlock block = *it;
            if (block.aClass == originClass) { //Remove a class/previous class
                schedule.erase(it);
            }
        }
    }
    if (destinationClass != nullptr) { //If adding or changing a class
        for (Slot slot: destinationClass->getClassSchedule())
            schedule.push_back({destinationClass, slot}); //Add a class/new class
    }

    std::sort(schedule.begin(), schedule.end(), Student::scheduleBlockByTime());
    return schedule;
}

/**
 * Receives a list of requests and processes it applying the changes to a schedule
 * @param listOfRequests
 */
void Request::processListRequests(std::list<std::shared_ptr<Request>> listOfRequests) {
    for (std::shared_ptr<Request> request: listOfRequests) {
        request->changeSchedule();
    }
}

/**
 * Prints a request and its details.
 */
void Request::print() {
    std::cout << "Student: " << student->getStudentCode() << " | " << student->getStudentName()
              << " Request: ";
    if (originClass == nullptr) std::cout << "Add to " << destinationClass->getClassCode();
    else if (destinationClass == nullptr) std::cout << "Remove from " << originClass->getClassCode();
    else std::cout << "Change from " << originClass->getClassCode() << " to " << destinationClass->getClassCode();
    std::cout << " in the UC with the code " << uc->getUcCode() << "\n";
}
