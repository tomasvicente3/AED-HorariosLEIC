//
// Created by rita on 20-10-2022.
//


#include "Student.h"

//Constructors
Student::Student() {}

Student::Student(unsigned studentCode, std::string studentName) : studentCode(studentCode), studentName(studentName),
                                                                  numRequestsAwaiting(0) {}

//Setters
void Student::setNumRequestsAwaiting(int n) {
    this->numRequestsAwaiting = n;
}

void Student::setStudentCode(unsigned int studentCode) {
    Student::studentCode = studentCode;
}

void Student::setStudentName(const std::string &studentName) {
    Student::studentName = studentName;
}

void Student::setRequestsAwaiting(const std::list<std::shared_ptr<Request>> &requestsAwaiting) {
    Student::requestsAwaiting = requestsAwaiting;
}


//Getters
int Student::getNumRequestsAwaiting() {
    return numRequestsAwaiting;
}

unsigned int Student::getStudentCode() const {
    return studentCode;
}

const std::string &Student::getStudentName() const {
    return studentName;
}

const std::list<std::shared_ptr<Request>> &Student::getRequestsAwaiting() const {
    return requestsAwaiting;
}

const std::map<std::shared_ptr<UC>, std::shared_ptr<Class>> &Student::getClassPerUc() const {
    return classPerUC;
}

/**
 * Gets the class a student is registered in given a certain uc.
 * @param uc
 * @return Returns a shared pointer of the class the student is in.
 */
std::shared_ptr<Class> Student::getClassForUC(std::shared_ptr<UC> uc) const {
    auto it = std::find_if(classPerUC.begin(), classPerUC.end(),
                           [uc](std::pair<std::shared_ptr<UC>, std::shared_ptr<Class>> pair) {
                               return *(pair.first) == *uc;
                           });
    if (it == classPerUC.end()) { return nullptr; }
    return it->second;
}

/**
 * Gets the schedule of a given student. Complexity: O(n*m) in which n is the number of Ucs a student is in and m is the biggest number of slots that one of their classes have.
 * @return Returns a vector of schedule blocks that represents the schedule of a given student.
 */
std::vector<Student::scheduleBlock> Student::getSchedule() {
    std::vector<scheduleBlock> final;

    //percorrer as turmas (e cadeiras correspondentes)
    for (auto itC = classPerUC.begin(); itC != classPerUC.end(); itC++) {
        std::shared_ptr<Class> classP = itC->second;
        std::set<Slot> setSlotsP = classP->getClassSchedule();

        //percorrer os slots de cada aula que têm
        for (auto itS = setSlotsP.begin(); itS != setSlotsP.end(); itS++) {
            final.push_back({itC->second, *itS});
        }
    }
    std::sort(final.begin(), final.end(), scheduleBlockByTime());
    return final;
}

//Operators
bool Student::operator<(const Student &rhs) const {
    if (studentCode < rhs.studentCode)
        return true;
    if (rhs.studentCode < studentCode)
        return false;
    if (studentName < rhs.studentName)
        return true;
    if (rhs.studentName < studentName)
        return false;
    return false;
}

bool Student::operator>(const Student &rhs) const {
    return rhs < *this;
}

bool Student::operator<=(const Student &rhs) const {
    return !(rhs < *this);
}

bool Student::operator>=(const Student &rhs) const {
    return !(*this < rhs);
}

bool Student::operator==(const Student &rhs) const {
    return studentCode == rhs.studentCode &&
           studentName == rhs.studentName &&
           classPerUC == rhs.classPerUC &&
           numRequestsAwaiting == rhs.numRequestsAwaiting;
}

bool Student::operator!=(const Student &rhs) const {
    return !(rhs == *this);
}

//Modifiers
/**
 * Adds a class from a certain uc to a student
 * @param uc
 * @param aClass
 */
void Student::addClass(std::shared_ptr<UC> uc, std::shared_ptr<Class> aClass) {
    classPerUC.insert({uc, aClass});
    aClass->incrementNumStudents();
}

/**
 * Removes a class from a specified uc from a student
 * @param uc
 * @param aClass
 */
void Student::removeClass(std::shared_ptr<UC> uc, std::shared_ptr<Class> aClass) {
    classPerUC.erase(uc);
    aClass->decrementNumStudents();
}


/**
 * Changes the class from a certain uc in which the student is registered.
 * @param uc
 * @param destinationClass
 */
void Student::changeUcClass(std::shared_ptr<UC> uc, std::shared_ptr<Class> destinationClass) {
    auto it = std::find_if(classPerUC.begin(), classPerUC.end(),
                           [uc](std::pair<std::shared_ptr<UC>, std::shared_ptr<Class>> pair) {
                               return *(pair.first) == *uc;
                           });
    if (it == classPerUC.end()) {
        std::cout << "Please enter a UC this student belongs to!" << '\n';
        return;
    }
    it->second->decrementNumStudents();
    it->second = destinationClass;
    destinationClass->incrementNumStudents();
}

/**
 * Adds a request to a student.
 * @param request
 */
void Student::addRequest(std::shared_ptr<Request> request) {
    requestsAwaiting.push_back(request);
    numRequestsAwaiting++;
}


//Utilities
/**
 *  Checks if a student is in registered in a given class.
 * @param classCode
 * @param ucCode
 * @return Returns true if the student is present and false if it isn't.
 */
bool Student::isInClass(std::string classCode, std::string ucCode) {
    for (std::pair<const std::shared_ptr<UC>, std::shared_ptr<Class>> pair: classPerUC) {
        if (pair.second->getClassCode() == classCode && pair.second->getUcCode() == ucCode) return true;
    }
    return false;
}

/**
 *  Checks if a student is in a given uc.
 * @param ucCode
 * @return Returns true if the student is in the said Uc and false if it isn't.
 */
bool Student::isInUc(std::string ucCode) {
    for (std::pair<const std::shared_ptr<UC>, std::shared_ptr<Class>> pair: classPerUC) {
        if (pair.first->getUcCode() == ucCode) return true;
    }
    return false;
}

/**
 *  Checks if a student is in a given year.
 * @return Returns true if the student is in the year or false if it isn't.
 */
bool Student::isInYear(unsigned year) {
    for (std::pair<const std::shared_ptr<UC>, std::shared_ptr<Class>> pair: classPerUC) {
        if (pair.second->getYear() == year) return true;
    }
    return false;
}

/**
 * Prints a student,its classes and requests.
 */
void Student::print() {
    std::cout << std::setw(10) << "Student: " << std::setw(25) << studentName << std::setw(10) << "\tCode: "
              << std::setw(15) << studentCode;
    if (classPerUC.size() == 0) std::cout << std::setw(10) << "No classes";
    else {
        std::cout << std::setw(10) << "Classes:\t";
        for (auto it = classPerUC.begin(); it != classPerUC.end(); it++) {
            std::cout << it->first->getUcCode() << "->" << " " << it->second->getClassCode() << "\t";
        }
    }
    if (numRequestsAwaiting == 0) std::cout << " No requests pending" << "\n";
    else std::cout << " " << numRequestsAwaiting << " requests pending" << "\n";
}



