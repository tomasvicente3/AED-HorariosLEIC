//
// Created by rita on 20-10-2022.
//

#ifndef HORARIOSLEIC_SCHEDULER_H
#define HORARIOSLEIC_SCHEDULER_H

#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <set>
#include "Student.h"
#include "Request.h"

class Scheduler {
private:
    std::set<std::shared_ptr<Student>, Student::studentsByNumber> students;
    std::list<std::shared_ptr<Class>> classes;
    std::list<std::shared_ptr<UC>> ucs;
    std::queue<std::shared_ptr<Request>> requests;
    std::list<std::shared_ptr<Request>> rejectedRequests;


public:
    static const unsigned CLASS_CAP = 20;

    //Constructor
    Scheduler();


    //Getters
    std::set<std::shared_ptr<Student>, Student::studentsByNumber> getStudents() const;

    std::list<std::shared_ptr<Class>> getClasses() const;

    std::list<std::shared_ptr<UC>> getUcs() const;

    std::queue<std::shared_ptr<Request>> getRequests() const;

    std::list<std::shared_ptr<Request>> getRejectedRequests() const;

    //Setters
    void setStudents(const std::set<std::shared_ptr<Student>, Student::studentsByNumber> &students);

    void setClasses(const std::list<std::shared_ptr<Class>> &classes);

    void setUcs(const std::list<std::shared_ptr<UC>> &ucs);

    void setRequests(const std::queue<std::shared_ptr<Request>> &requests);

    void setRejectedRequests(const std::list<std::shared_ptr<Request>> &rejectedRequests);

    //Finders
    std::shared_ptr<Student> findStudent(unsigned int studentCode);

    std::shared_ptr<Class> findClass(std::string classCode, std::string ucCode);

    std::shared_ptr<UC> findUc(std::string ucCode);

    std::shared_ptr<Request> findRequest(std::shared_ptr<Student> student, std::shared_ptr<UC> uc, std::shared_ptr<Class> originClass, std::shared_ptr<Class> destinationClass);


        //Add and delete
    bool addStudent(std::shared_ptr<Student> student);

    bool removeStudent(std::shared_ptr<Student> student);

    bool addClass(std::shared_ptr<Class> aClass);

    bool removeClass(std::shared_ptr<Class> aClass);

    bool addUc(std::shared_ptr<UC> uc);

    bool removeUc(std::shared_ptr<UC> uc);

    bool addRequest(std::shared_ptr<Request> request);

    bool removeRequest(std::shared_ptr<Request> request);

    //Utilities
    void extractFileInfo(std::string classesFile, std::string classesPerUCFile, std::string studentsClassesFile);

    void writeFileInfo(std::string classesFile, std::string classesPerUCFile, std::string studentsClassesFile);

    void processRequests(bool &changesToWriteToFiles);

    bool scheduleConflicts(std::vector<Student::scheduleBlock> schedule);

    std::pair<std::list<std::shared_ptr<Request>>, std::list<std::shared_ptr<Request>>>
    getOptimalRequestCombination(std::list<std::shared_ptr<Request>> listOfRequests);


    //Listings
    std::vector<std::shared_ptr<Student>> getStudentsInClass(std::string classCode, std::string ucCode);

    std::vector<std::shared_ptr<Student>> getStudentsInUc(std::string ucCode);

    std::vector<std::shared_ptr<Student>> getStudentsInYear(unsigned year);

    std::vector<std::shared_ptr<Student>> getStudentsInMoreThanXUcs(const int &numUcs, const int &order = 0);

    unsigned int getClassOccupation(std::shared_ptr<Class> aClass);

    std::map<std::shared_ptr<Class>, unsigned> getAllClassesOccupation(std::shared_ptr<UC> uc);

    unsigned int getUCOccupation(std::string ucCode);

    std::map<std::shared_ptr<UC>, unsigned> getAllUCsOccupation();

    unsigned int getYearOccupation(unsigned int year);

    std::map<unsigned, unsigned> getAllYearsOccupation();

};


#endif //HORARIOSLEIC_SCHEDULER_H
