//
// Created by rita on 20-10-2022.
//

#ifndef HORARIOSLEIC_STUDENT_H
#define HORARIOSLEIC_STUDENT_H

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <iomanip>

#include "UC.h"
#include "Class.h"

class Request;

class Student {
private:
    unsigned studentCode;
    std::string studentName;
    std::map<std::shared_ptr<UC>, std::shared_ptr<Class>> classPerUC;
    std::list<std::shared_ptr<Request>> requestsAwaiting;
    int numRequestsAwaiting;

public:
    //Structs
    //Compares std::shared_ptr<Student> by their student number
    struct studentsByNumber {
        bool operator()(std::shared_ptr<Student> ptr1, std::shared_ptr<Student> ptr2) {
            return ptr1->getStudentCode() < ptr2->getStudentCode();
        };
    };

    //Compares std::shared_ptr<Student> by their name
    struct studentsByName {
        bool operator()(std::shared_ptr<Student> ptr1, std::shared_ptr<Student> ptr2) {
            return ptr1->getStudentName() < ptr2->getStudentName();
        };
    };

    //Compares std::shared_ptr<Student> by number of UCs
    struct studentsByNumberOfUCs {
        bool operator()(std::shared_ptr<Student> ptr1, std::shared_ptr<Student> ptr2) {
            return ptr1->getClassPerUc().size() < ptr2->getClassPerUc().size();
        };
    };

    struct scheduleBlock {
        std::shared_ptr<Class> aClass;
        Slot slot;
    };

    struct scheduleBlockByTime {
        bool operator()(scheduleBlock block1, scheduleBlock block2) {
            return block1.slot < block2.slot;
        }
    };

    //Constructors
    Student();

    Student(unsigned studentCode, std::string studentName);

    //Setters
    void setNumRequestsAwaiting(int n);

    void setStudentCode(unsigned int studentCode);

    void setStudentName(const std::string &studentName);

    void setRequestsAwaiting(const std::list<std::shared_ptr<Request>> &requestsAwaiting);


    //Getters
    int getNumRequestsAwaiting();

    unsigned int getStudentCode() const;

    const std::string &getStudentName() const;

    const std::list<std::shared_ptr<Request>> &getRequestsAwaiting() const;

    const std::map<std::shared_ptr<UC>, std::shared_ptr<Class>> &getClassPerUc() const;

    std::shared_ptr<Class>getClassForUC(std::shared_ptr<UC> uc) const;

    std::vector<scheduleBlock> getSchedule();

    //Operators
    bool operator<(const Student &rhs) const;

    bool operator>(const Student &rhs) const;

    bool operator<=(const Student &rhs) const;

    bool operator>=(const Student &rhs) const;

    bool operator==(const Student &rhs) const;

    bool operator!=(const Student &rhs) const;


    //Modifiers
    void addClass(std::shared_ptr<UC> uc, std::shared_ptr<Class> aClass);

    void removeClass(std::shared_ptr<UC> uc, std::shared_ptr<Class> aClass);

    void changeUcClass(std::shared_ptr<UC> uc, std::shared_ptr<Class> destinationClass);

    void addRequest(std::shared_ptr<Request> request);

    void removeRequest(std::shared_ptr<Request> request);

    //Utilities
    bool isInClass(std::string classCode, std::string ucCode);

    bool isInUc(std::string ucCode);

    bool isInYear(unsigned year);

    void print();

};


#endif //HORARIOSLEIC_STUDENT_H
