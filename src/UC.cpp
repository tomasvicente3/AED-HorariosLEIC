//
// Created by rita on 20-10-2022.
//

#include "UC.h"

//Constructors
UC::UC() {}

UC::UC(std::string ucCode) : ucCode(ucCode) {}

UC::UC(std::string ucCode, std::vector<std::shared_ptr<Class>> classesPerUC) {
    this->ucCode = ucCode;
    this->classesPerUC = classesPerUC;
}


//Setters
void UC::setUcCode(const std::string &ucCode) {
    UC::ucCode = ucCode;
}

void UC::setClassesPerUc(const std::vector<std::shared_ptr<Class>> &classesPerUc) {
    classesPerUC = classesPerUc;
}


//Getters
const std::string &UC::getUcCode() const {
    return ucCode;
}

const std::vector<std::shared_ptr<Class>> &UC::getClassesPerUc() const {
    return classesPerUC;
}


//Operators
bool UC::operator==(const UC &rhs) const {
    return ucCode == rhs.ucCode;
}

bool UC::operator!=(const UC &rhs) const {
    return !(rhs == *this);
}

bool UC::operator<(const UC &rhs) const {
    if (ucCode < rhs.ucCode)
        return true;
    return false;
}


//Modifiers
/**
 * Receives a class and adds it to the vector classesPerUC where all classes from a given uc are.
 * @param aClass
 */
void UC::addClass(std::shared_ptr<Class> aClass) {
    classesPerUC.push_back(aClass);
}

/**
 * Removes a class from the vector classesPerUC where all classes from a given uc are.
 * @param aClass
 */
void UC::removeClass(std::shared_ptr<Class> aClass) {
    std::remove_if(classesPerUC.begin(), classesPerUC.end(), [aClass](std::shared_ptr<Class> aClass1){return *aClass1 == *aClass;});
    classesPerUC.pop_back();
}

//Utilities
/**
 *  Checks if a vector of int numbers is balanced.
 * @param numStudentsPerClass
 * @return Returns true if the difference between any two elements is smaller than 4 and false if it is.
 */
bool UC::checkIfBalanced(std::vector<int> numStudentsPerClass) {
    if (std::max_element(numStudentsPerClass.begin(), numStudentsPerClass.end()) -
        std::min_element(numStudentsPerClass.begin(),
                         numStudentsPerClass.end()) < 4) {
        return true;
    }
    return false;
}

/**
 * Checks if all the classes of a certain Uc are balanced.
 * @return Returns true if they are balanced, and false if they aren't.
 */
bool UC::checkUcClassesBalanced() {
    std::vector<int> numStudentsPerClass;
    for (auto it = classesPerUC.begin(); it < classesPerUC.end(); it++) {
        numStudentsPerClass.push_back((*it)->getNumStudents());
    }
    return checkIfBalanced(numStudentsPerClass);
}

/**
 * prints a uc's code and its classes.
 */
void UC::print(){
    std::cout << "UC Code:\t" << ucCode << "\t";
    std::cout << "Classes:\t";
    for (std::shared_ptr<Class> aClass: classesPerUC){
        std::cout << aClass->getClassCode() << "\t";
    }
    std::cout << "\n";
}
