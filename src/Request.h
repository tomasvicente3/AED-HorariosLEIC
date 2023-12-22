//
// Created by rita on 20-10-2022.
//

#ifndef HORARIOSLEIC_REQUEST_H
#define HORARIOSLEIC_REQUEST_H

#include "Student.h"

#include <iostream>

class Request {
private:
    std::shared_ptr<Student> student;
    std::shared_ptr<UC> uc;
    std::shared_ptr<Class> originClass, destinationClass;

public:
    //Constructors
    Request(const std::shared_ptr<Student> &student,  const std::shared_ptr<UC> &uc, const std::shared_ptr<Class> &originClass, const std::shared_ptr<Class> &destinationClass = nullptr);

//Setters
    void setStudent(const std::shared_ptr<Student> &student);

    void setOriginClass(const std::shared_ptr<Class> &originClass);

    void setDestinationClass(const std::shared_ptr<Class> &destinationClass);

    void setUc(const std::shared_ptr<UC> &uc);

    //Getters
    std::shared_ptr<Student> getStudent() const;

    std::shared_ptr<Class> getOriginClass() const;

    std::shared_ptr<Class> getDestinationClass() const;

    std::shared_ptr<UC> getUc() const;

    //Operators
    bool operator==(const Request &rhs) const;

    bool operator!=(const Request &rhs) const;

    //Comparators
    static bool complementingRequest(const std::shared_ptr<Request>& request1, const std::shared_ptr<Request> &request2);

    std::vector<Student::scheduleBlock> stageScheduleChange();

    bool checkReqClassBalanced();

    std::vector<Student::scheduleBlock> stageScheduleChangeOnSchedule(std::vector<Student::scheduleBlock> schedule);

    void changeSchedule();

    static void processListRequests(std::list<std::shared_ptr<Request>> listOfRequests);

    void print();
};


#endif //HORARIOSLEIC_REQUEST_H
