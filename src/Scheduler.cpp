//
// Created by rita on 20-10-2022.
//



#include <functional>
#include "Scheduler.h"


//Constructor

Scheduler::Scheduler() = default;

//Getters

std::set<std::shared_ptr<Student>, Student::studentsByNumber> Scheduler::getStudents() const {
    return students;
}

std::list<std::shared_ptr<Class>> Scheduler::getClasses() const {
    return classes;
}

std::list<std::shared_ptr<UC>> Scheduler::getUcs() const {
    return ucs;
}

std::queue<std::shared_ptr<Request>> Scheduler::getRequests() const {
    return requests;
}

std::list<std::shared_ptr<Request>> Scheduler::getRejectedRequests() const {
    return rejectedRequests;
}

//Setters

void Scheduler::setStudents(const std::set<std::shared_ptr<Student>, Student::studentsByNumber> &students) {
    Scheduler::students = students;
}

void Scheduler::setClasses(const std::list<std::shared_ptr<Class>> &classes) {
    Scheduler::classes = classes;
}

void Scheduler::setUcs(const std::list<std::shared_ptr<UC>> &ucs) {
    Scheduler::ucs = ucs;
}

void Scheduler::setRequests(const std::queue<std::shared_ptr<Request>> &requests) {
    Scheduler::requests = requests;
}

void Scheduler::setRejectedRequests(const std::list<std::shared_ptr<Request>> &rejectedRequests) {
    Scheduler::rejectedRequests = rejectedRequests;
}

//Finders
/**
 *  Searches for a student in the list of students.
 * @param studentCode
 * @return Returns the shared pointer of a student or a null pointer if the student can't be found.
 */
std::shared_ptr<Student> Scheduler::findStudent(unsigned studentCode) {
    auto itr = find_if(students.begin(), students.end(),
                       [studentCode](std::shared_ptr<Student> ptr) { return ptr->getStudentCode() == studentCode; });
    if (itr == students.end()) return nullptr;
    return *itr;
}

/**
 * Searches for the given class in a given uc.
 * @param classCode
 * @param ucCode
 * @return Returns a shared pointer of the said class or a null pointer if the class can't be found.
 */
std::shared_ptr<Class> Scheduler::findClass(std::string classCode, std::string ucCode) {
    auto itr = find_if(classes.begin(), classes.end(), [classCode, ucCode](std::shared_ptr<Class> ptr) {
        return ptr->getClassCode() == classCode && ptr->getUcCode() == ucCode;
    });
    if (itr == classes.end()) return nullptr;
    return *itr;
}

/**
 * Searches for a given uc.
 * @param ucCode
 * @return Returns a shared pointer for the uc or a null pointer in case the uc can't be found.
 */
std::shared_ptr<UC> Scheduler::findUc(std::string ucCode) {
    auto itr = find_if(ucs.begin(), ucs.end(),
                       [ucCode](std::shared_ptr<UC> ptr) { return ptr->getUcCode() == ucCode; });
    if (itr == ucs.end()) return nullptr;
    return *itr;
}

/**
 * Receives all request parameters and with them looks for a already existing request
 * @param student
 * @param uc
 * @param originClass
 * @param destinationClass
 * @return Returns a shared pointer for the request if found, if not returns a null pointer.
 */
std::shared_ptr<Request>
Scheduler::findRequest(std::shared_ptr<Student> student, std::shared_ptr<UC> uc, std::shared_ptr<Class> originClass,
                       std::shared_ptr<Class> destinationClass) {
    std::shared_ptr<Request> tempRequest(new Request(student, uc, originClass, destinationClass));
    for (size_t i = 0; i < requests.size(); i++) {
        std::shared_ptr<Request> currRequest = requests.front();
        if (*currRequest == *tempRequest) return currRequest;
    }
    return nullptr;
}


//Add and delete

/**
 * Tries to insert a student in the vector containing all students.
 * @param student
 * @return Returns true if the student was successfully inserted and false if it wasn't.
 */
bool Scheduler::addStudent(std::shared_ptr<Student> student) {
    return students.insert(student).second;

}

/**
 * Tries to remove a student form the vector with all students.
 * @param student
 * @return Returns true if it was successful and false if the procedure failed.
 */
bool Scheduler::removeStudent(std::shared_ptr<Student> student) {
    bool result = students.erase(student);
    for (auto pair: student->getClassPerUc()) pair.second->decrementNumStudents();
    student.reset();
    return result;
}

/**
 * Tries to insert a class in the classes vector.
 * @param aClass
 * @return Returns true if the class was inserted amd false if the class was already there.
 */
bool Scheduler::addClass(std::shared_ptr<Class> aClass) {
    if (findClass(aClass->getClassCode(), aClass->getUcCode()) != nullptr)
        return false; //The class is already found in the classes vector
    classes.push_back(aClass);
    return true;
}

/**
 * Tries to remove a class from the classes vector.
 * @param aClass
 * @return Returns true if the class was removed successfully and false if the class wasn't on the vector.
 */
bool Scheduler::removeClass(std::shared_ptr<Class> aClass) {
    if (findClass(aClass->getClassCode(), aClass->getUcCode()) == nullptr)
        return false; //The class was not found in the classes vector

    std::shared_ptr<UC> uc = findUc(aClass->getUcCode());
    uc->removeClass(aClass);

    for (std::shared_ptr<Student> student: getStudentsInClass(aClass->getClassCode(), aClass->getUcCode())) {
        student->removeClass(uc, aClass);
    }

    std::remove_if(classes.begin(), classes.end(),
                   [aClass](std::shared_ptr<Class> class1) { return *aClass == *class1; });
    classes.pop_back();
    aClass.reset();
    return true;
}

/**
 * Tries to add an uc to the ucs vector.
 * @param uc
 * @return Returns true if the uc was inserted and false if the uc was already in the vector.
 */
bool Scheduler::addUc(std::shared_ptr<UC> uc) {
    if (findUc(uc->getUcCode()) != nullptr) return false; //The uc is already found in the ucs vector
    ucs.push_back(uc);
    return true;
}

/**
 * Tries to remove an uc from the ucs vector.
 * @param uc
 * @return Returns true if the uc was removed and false if the uc was not present in the vector.
 */
bool Scheduler::removeUc(std::shared_ptr<UC> uc) {
    if (findUc(uc->getUcCode()) == nullptr) return false; //The class was not found in the classes vector

    std::vector<std::shared_ptr<Class>> classesPerUcCopy = uc->getClassesPerUc();
    for (std::shared_ptr<Class> aClass: classesPerUcCopy) {
        removeClass(aClass);
    } //Classes no longer exist if their associated UC is removed

    std::remove_if(ucs.begin(), ucs.end(),
                   [uc](std::shared_ptr<UC> uc1) { return uc1->getUcCode() == uc->getUcCode(); });
    ucs.pop_back();
    uc.reset();

    return true;
}

/**
 * Adds a new request to the request queue.
 * @param request
 * @return Returns true if the request was added and false if the request already existed.
 */
bool Scheduler::addRequest(std::shared_ptr<Request> request) {
    if (findRequest(request->getStudent(), request->getUc(), request->getOriginClass(),
                    request->getDestinationClass()) != nullptr)
        return false; //Request already exists in requests queue
    requests.push(request);
    request->getStudent()->addRequest(request);
    return true;
}


//Extract info from files
/**
 * Receives info from the three files and extracts it.
 * @param classesFile
 * @param classesPerUCFile
 * @param studentsClassesFile
 */
void
Scheduler::extractFileInfo(std::string classesFile, std::string classesPerUCFile, std::string studentsClassesFile) {
    std::ifstream classes(classesFile);
    std::ifstream classesPerUC(classesPerUCFile);
    std::ifstream studentsClasses(studentsClassesFile);

    std::string currentParam, currentLine;
    std::string prevUcCode, ucCode;
    std::string classCode;
    std::vector<std::shared_ptr<Class>> currentClasses;

    //classes_per_uc.csv

    int counter = 1;

    getline(classesPerUC, currentParam); //Ignore first line with just descriptors
    getline(classesPerUC, prevUcCode, ','); //Initialize currentUcCode with first UcCode
    ucCode = prevUcCode;


    while (getline(classesPerUC, currentLine)) {
        std::istringstream iss(currentLine);
        while (getline(iss, currentParam, ',')) {
            switch (counter++) {
                case 0: {
                    ucCode = currentParam;
                    if (currentParam == "L.EIC025" && classCode == "3LEIC14") {
                        int a;
                        a++;
                    }
                    break;
                }
                case 1: {
                    classCode = currentParam;
                    counter = 0;
                    break;
                }
            }
            if (counter == 0) {
                if (ucCode != prevUcCode) {
                    std::shared_ptr<UC> newUC(new UC(prevUcCode, currentClasses));
                    addUc(newUC);
                    prevUcCode = ucCode;
                    currentClasses.clear();
                }
                std::shared_ptr<Class> currentClass(new Class(classCode, ucCode));
                currentClasses.push_back(currentClass);
                addClass(currentClass);

            }


        }
    }

    std::shared_ptr<UC> newUC(new UC(ucCode, currentClasses)); //Create last uc read
    addUc(newUC);

    //classes.csv

    currentParam = "";
    currentLine = "";
    classCode = "";

    float startHour, duration;
    std::string weekday, type;
    std::shared_ptr<UC> uc;
    std::shared_ptr<Class> aClass;

    counter = 0;
    getline(classes, currentParam); //Ignore first line with just descriptors

    while (getline(classes, currentLine)) {
        std::istringstream iss(currentLine);
        //ASSUMINDO QUE SE APAGOU CADEIRA PROJETO UP UMA VEZ QUE NÃO TEM ESTUDANTES (PERMITIDO PROF)
        while (getline(iss, currentParam, ',')) {
            switch (counter++) {
                case 0: {
                    classCode = currentParam;
                    break;
                }
                case 1: {
                    uc = findUc(currentParam);
                    aClass = findClass(classCode, currentParam);
                    break;
                }
                case 2: {
                    weekday = currentParam;
                    break;
                }
                case 3: {
                    startHour = stof(currentParam);
                    break;
                }
                case 4: {
                    duration = stof(currentParam);
                    break;
                }
                case 5: {
                    type = currentParam;
                    counter = 0;
                    break;
                }
            }

            if (counter == 0) {
                Slot currentSlot(weekday, startHour, duration, type);
                aClass->addSlotToSchedule(currentSlot);
            }
        }
    }


    //students_classes.csv

    currentParam = "";
    currentLine = "";
    unsigned prevStudentCode = 1;
    unsigned studentCode;
    std::shared_ptr<Student> currentStudent;
    std::string studentName;
    uc = nullptr;
    aClass = nullptr;

    counter = 1;

    getline(studentsClasses, currentParam); //Ignore first line with just descriptors
    getline(studentsClasses, currentParam, ','); //Initialize currentParam with first StudentCode
    studentCode = stoul(currentParam);

    while (getline(studentsClasses, currentLine)) {
        std::istringstream iss(currentLine);
        while (getline(iss, currentParam, ',')) {
            switch (counter++) {
                case 0: {
                    studentCode = stoul(currentParam);
                    break;
                }
                case 1: {
                    studentName = currentParam;
                    break;
                }
                case 2: {
                    uc = findUc(currentParam);
                    break;
                }
                case 3: {
                    aClass = findClass(currentParam, uc->getUcCode());
                    counter = 0;
                    break;
                }
            }

            if (counter == 0) {
                if (studentCode !=
                    prevStudentCode) { //Just read a different Student than previous or the first student read
                    currentStudent = static_cast<std::shared_ptr<Student>>(new Student(studentCode, studentName));
                    students.insert(currentStudent);
                    prevStudentCode = studentCode;

                }
                currentStudent->addClass(uc, aClass);
            }
        }
    }
}
/**
 * Writes the information stored in the program's data structures to the designated files. Complexity: O(n*m) in which n is the number of students and m is the largest number of classes a student has.
 * @param classesFile
 * @param classesPerUCFile
 * @param studentsClassesFile
 */
void Scheduler::writeFileInfo(std::string classesFile, std::string classesPerUCFile, std::string studentsClassesFile) {
    std::ofstream classes, classesPerUC, studentsClasses;

    classes.open(classesFile, std::ofstream::out | std::ofstream::trunc); //Clear old data
    classesPerUC.open(classesPerUCFile, std::ofstream::out | std::ofstream::trunc);
    studentsClasses.open(studentsClassesFile, std::ofstream::out | std::ofstream::trunc);

    classes << "ClassCode,UcCode,Weekday,StartHour,Duration,Type\n";
    classesPerUC << "UcCode,ClassCode\n";
    studentsClasses << "StudentCode,StudentName,UcCode,ClassCode\n";

    this->classes.sort([](std::shared_ptr<Class> class1, std::shared_ptr<Class> class2) {
        return class1->getUcCode() < class2->getUcCode();
    }); //Order classes by alphabetical ucCode for easier info extraction when file is read next

    for (std::shared_ptr<Class> aClass: this->classes) {
        //classes_per_uc.csv
        classesPerUC << aClass->getUcCode() << "," << aClass->getClassCode() << '\n';

        //classes.csv
        for (Slot slot: aClass->getClassSchedule()) {
            classes << aClass->getClassCode() << "," << aClass->getUcCode() << "," << slot.getWeekday() << ","
                    << slot.getStartHour() << "," << slot.getDuration() << "," << slot.getType() << '\n';
        }
    }

    //students_classes.csv
    for (std::shared_ptr<Student> student: this->students) {
        for (auto pairUcClass: student->getClassPerUc()) {
            studentsClasses << student->getStudentCode() << "," << student->getStudentName() << ",";
            studentsClasses << pairUcClass.first->getUcCode() << "," << pairUcClass.second->getClassCode() << '\n';
        }
    }
}

/**
 * Given a class and an uc it belongs to, gets the students in said class.
 * @param classCode
 * @param ucCode
 * @return Returns a vector with the students in the class or an empty vector if none are found.
 */
std::vector<std::shared_ptr<Student>> Scheduler::getStudentsInClass(std::string classCode, std::string ucCode) {
    if (findClass(classCode, ucCode) == nullptr) {
        std::cout << "A class for this UC with that code doesn't exist!";
        return {};
    }
    std::vector<std::shared_ptr<Student>> studentsInClass;
    for (std::shared_ptr<Student> student: students) {
        if (student->isInClass(classCode, ucCode)) studentsInClass.push_back(student);
    }
    return studentsInClass;
}

/**
 * Given an uc gets the students in said uc.
 * @param ucCode
 * @return Returns a vector with all students in the uc or an empty vector if none are found.
 */
std::vector<std::shared_ptr<Student>> Scheduler::getStudentsInUc(std::string ucCode) {
    if (findUc(ucCode) == nullptr) {
        std::cout << "A UC with that code doesn't exist!";
        return {};
    }

    std::vector<std::shared_ptr<Student>> studentsInUc;
    for (std::shared_ptr<Student> student: students) {
        if (student->isInUc(ucCode)) studentsInUc.push_back(student);
    }
    return studentsInUc;
}

/**
 * Given an year gets the studets in said year.
 * @param year
 * @return Returns a vector with all the students in said year, if none are found returns an empty vector.
 */
std::vector<std::shared_ptr<Student>> Scheduler::getStudentsInYear(unsigned year) {
    if (year > 3 || year < 1) {
        std::cout << "Please enter a valid bachelor's degree year!";
        return {};
    }

    std::vector<std::shared_ptr<Student>> studentsInYear;
    for (std::shared_ptr<Student> student: students) {
        if (student->isInYear(year)) studentsInYear.push_back(student);
    }
    return studentsInYear;
}

/**
 * Receives a number of ucs and gets the students in those ucs. Complexity (n * log(n)): in which n is the number of students in more than x UCs.
 * @param numUcs
 * @param order
 * @return Returns a vector with all the students in the given ucs, if none are found returns an empty vector.
 */
std::vector<std::shared_ptr<Student>> Scheduler::getStudentsInMoreThanXUcs(const int &numUcs, const int &order) {
    //0 - Alphabetical Order
    //1 - Decreasing number of UCs

    if (numUcs >= ucs.size()) {
        std::cout << "Please enter a number of UCs equal or lower to the total of " << ucs.size() - 1 << "!\n";
        return {};
    }

    std::vector<std::shared_ptr<Student>> studentsInMoreThanXUcs;
    for (std::shared_ptr<Student> student: students) {
        if (student->getClassPerUc().size() > numUcs) studentsInMoreThanXUcs.push_back(student);
    }

    if (order == 0) {
        std::sort(studentsInMoreThanXUcs.begin(), studentsInMoreThanXUcs.end(), Student::studentsByName());
        return studentsInMoreThanXUcs;
    }
    std::sort(studentsInMoreThanXUcs.begin(), studentsInMoreThanXUcs.end(), Student::studentsByNumberOfUCs());
    return studentsInMoreThanXUcs;
}

/**
 * Processes requests made by students.
 */
void Scheduler::processRequests(bool &changesToWriteToFiles) {
    for (size_t i = 0; i < requests.size(); i++) {
        std::shared_ptr<Request> currRequest = requests.front();
        requests.pop();

        std::shared_ptr<Student> student = currRequest->getStudent();

        if (student->getNumRequestsAwaiting() == 1) {
            if (currRequest->getDestinationClass() == nullptr ||
                (!Scheduler::scheduleConflicts(currRequest->stageScheduleChange()) &&
                 currRequest->getDestinationClass()->getNumStudents() + 1 <= CLASS_CAP &&
                 currRequest->checkReqClassBalanced())) { //The request won't add a student to a class or adding them doesn't go over the cap/alter the balance of the UC
                currRequest->changeSchedule();
                std::cout << "Accepted: ";
                currRequest->print();
                changesToWriteToFiles = true;
            } else {
                rejectedRequests.push_back(currRequest);
            }

        } else {
            auto pairSatisfiedNotSatisfied = getOptimalRequestCombination(
                    currRequest->getStudent()->getRequestsAwaiting());
            std::list<std::shared_ptr<Request>> requestsToSatisfy = pairSatisfiedNotSatisfied.first;
            rejectedRequests.merge(pairSatisfiedNotSatisfied.second);
            Request::processListRequests(requestsToSatisfy);
        }

    }

    for (auto it = rejectedRequests.begin(); it != rejectedRequests.end(); it++) {
        auto it2 = std::find_if(rejectedRequests.begin(), rejectedRequests.end(),
                                std::bind(Request::complementingRequest, *it, std::placeholders::_1));
        if (it2 != rejectedRequests.end()) {
            (*it)->changeSchedule();
            (*it2)->changeSchedule();
            rejectedRequests.erase(it);
            rejectedRequests.erase(it2);
            std::cout << "Accepted: ";
            (*it)->print();
            std::cout << "Accepted: ";
            (*it2)->print();
            changesToWriteToFiles = true;
        } else {
            std::cout << "Rejected: ";
            (*it)->print();

        }
    }
}

/**
 * Tries possible combinations of requests made by a student in order to find the best combination possible
 * @param listOfRequests
 * @return returns a pair of vectors of requests, one with all satisfied requests and another with all requests that can't be satisfied in the optimal combination.
 */
std::pair<std::list<std::shared_ptr<Request>>, std::list<std::shared_ptr<Request>>>
Scheduler::getOptimalRequestCombination(std::list<std::shared_ptr<Request>> listOfRequests) {
    std::list<std::shared_ptr<Request>> currentRequestsToSatisfy, finalRequestsToSatisfy;
    std::list<std::shared_ptr<Request>> currentRequestsNotSatisfied, finalRequestsNotSatisfied;
    std::vector<Student::scheduleBlock> currSchedule;

    for (auto it = listOfRequests.begin(); it != listOfRequests.end(); it++) {
        currentRequestsToSatisfy.push_back(*it);
        currSchedule = (*it)->stageScheduleChange();

        for (auto it2 = it; it2 != listOfRequests.end(); it2++) {
            if (it2 != it) {
                std::vector<Student::scheduleBlock> tempSchedule = (*it2)->stageScheduleChangeOnSchedule(
                        currSchedule);
                if ((*it2)->getDestinationClass() == nullptr ||
                    (!scheduleConflicts(tempSchedule) &&
                     (*it2)->getDestinationClass()->getNumStudents() + 1 <= CLASS_CAP) &&
                    (*it2)->checkReqClassBalanced()) { //If there are no conflicts/cap or balance problems when adding on this change
                    currentRequestsToSatisfy.push_back(*it2);
                    currSchedule = tempSchedule;
                } else currentRequestsNotSatisfied.push_back(*it2);
            }
        }
        if (currentRequestsToSatisfy.size() > finalRequestsToSatisfy.size()) {
            finalRequestsToSatisfy = currentRequestsToSatisfy;
            finalRequestsNotSatisfied = currentRequestsNotSatisfied;
        }
    }
    return std::make_pair(finalRequestsToSatisfy, finalRequestsNotSatisfied);
}


/**
 * Checks if there are any slot conflicts in a given schedule. Complexity : O(n²) in which n is the number of schedule blocks in the vector schedule.
 * @param schedule
 * @return Returns true if there are any conflicts and false if there aren't.
 */
bool Scheduler::scheduleConflicts(std::vector<Student::scheduleBlock> schedule) {
    std::vector<Student::scheduleBlock>::iterator it;
    std::vector<Student::scheduleBlock>::iterator it2;
    for (it = schedule.begin(); it < schedule.end() - 1; it++) {
        for (it2 = it + 1; it2 < schedule.end(); it2++) {
            if (it->slot.conflictingSlot(it2->slot)) {
                return true;
            }
        }
    }
    return false;
}

//Ocupations
/**
 *Gets the number of students of a class.
 * @param aClass
 * @return Returns a number of students of a class.
 */
unsigned Scheduler::getClassOccupation(std::shared_ptr<Class> aClass) {
    return aClass->getNumStudents();
}

/**
 * Gets the number of students of all classes of a given uc.
 * @param uc
 * @return Returns a map between each class and its occupation.
 */
std::map<std::shared_ptr<Class>, unsigned> Scheduler::getAllClassesOccupation(std::shared_ptr<UC> uc) {

    std::map<std::shared_ptr<Class>, unsigned> final;
    for (std::shared_ptr<Class> aClass: uc->getClassesPerUc()) {
        final[aClass] = aClass->getNumStudents();
    }
    return final;
}

/**
 * Gets the occupation of an uc.
 * @param ucCode
 * @return Returns the occupation of given uc.
 */
unsigned Scheduler::getUCOccupation(std::string ucCode) {
    return getStudentsInUc(ucCode).size();
}

/**
 * Gets the occupation of all ucs.
 * @return Returns a map between each uc and its occupation.
 */
std::map<std::shared_ptr<UC>, unsigned> Scheduler::getAllUCsOccupation() {
    std::map<std::shared_ptr<UC>, unsigned> final;
    for (std::shared_ptr<UC> uc: ucs) {
        final[uc] = getStudentsInUc(uc->getUcCode()).size();
    }
    return final;
}

/**
 * Gets the occupation of a given year.
 * @param year
 * @return Returns the occupation of a given year.
 */
unsigned Scheduler::getYearOccupation(unsigned year) {
    return getStudentsInYear(year).size();
}

/**
 * Gets the occupation of all years.
 * @return Returns a map between an year and its occupation.
 */
std::map<unsigned, unsigned> Scheduler::getAllYearsOccupation() {
    std::map<unsigned, unsigned> final;
    for (unsigned year = 1; year <= 3; year++) {
        final[year] = getStudentsInYear(year).size();
    }
    return final;
}



