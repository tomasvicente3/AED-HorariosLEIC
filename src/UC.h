//
// Created by rita on 20-10-2022.
//
#pragma once

#ifndef HORARIOSLEIC_UC_H
#define HORARIOSLEIC_UC_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "Class.h"

class UC {
private:
    std::string ucCode;
    std::vector<std::shared_ptr<Class>> classesPerUC;

public:
    //Constructors
    UC();
    UC(std::string ucCode);
    UC(std::string ucCode, std::vector<std::shared_ptr<Class>> classesPerUC);

    //Setters
    void setUcCode(const std::string &ucCode);
    void setClassesPerUc(const std::vector<std::shared_ptr<Class>> &classesPerUc);


    //Getters
    const std::string &getUcCode() const;
    const std::vector<std::shared_ptr<Class>> &getClassesPerUc() const;


    //Operators
    bool operator==(const UC &rhs) const;
    bool operator!=(const UC &rhs) const;
    bool operator<(const UC &rhs) const;

    //Modifiers
    void addClass(std::shared_ptr<Class> aClass);
    void removeClass(std::shared_ptr<Class> aClass);

    //Utilities
    static bool checkIfBalanced(std::vector<int> numStudentsPerClass);
    bool checkUcClassesBalanced();
    void print();

};


#endif //HORARIOSLEIC_UC_H
