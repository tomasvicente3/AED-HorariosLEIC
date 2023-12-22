//
// Created by rita on 24-10-2022.
//

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include "Scheduler.h"

std::string const CLASSES_FILE_PATH = "../schedule/classes.csv";
std::string const CLASSES_PER_UC_FILE_PATH = "../schedule/classes_per_uc.csv";
std::string const STUDENTS_CLASSES_PATH = "../schedule/students_classes.csv";

unsigned const COLUMN_WIDTH = 45;
unsigned const COLUMNS_PER_LINE = 3;

bool CHANGES = false;

using namespace std;

void studentAlreadyExists() {
    cout << "A student with that number already exists!" << endl;
}

void studentDoesntExist() {
    cout << "A student with that number doesn't exist!" << endl;
}

void ucAlreadyExists() {
    cout << "A UC with that code already exists!" << endl;
}

void ucDoesntExist() {
    cout << "A UC with that code doesn't exist!" << endl;
}

void classAlreadyExists() {
    cout << "A class for this UC with that code already exists!" << endl;
}

void classDoesntExist() {
    cout << "A class for this UC with that code doesn't exist!" << endl;
}

/**
 * Checks if the input give by the user is appropriate or not.
 * @param checkLength
 * @return Returns true if is is appropriate and false if it isn't.
 */
bool checkForAppropriateInput(int checkLength = 0) {
    //checkLength = 0 Don't check for length 1
    //checkLength = 1 Check for length 1
    //checkLength = 2 Check for max length 2

    if (!cin) // User didn't input what expected
    {
        cin.clear(); // reset failbit
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Skip bad input
        cout << "Please enter an appropriate input." << endl;
        return false;
    }

    if (checkLength == 1) {
        string line;
        getline(cin, line);
        if (line.length() > 1) {
            cout << "Please enter an appropriate input." << endl;
            return false;
        }
    }
    if (checkLength == 2) {
        string line;
        getline(cin, line);
        if (line.length() > 2) {
            cout << "Please enter an appropriate input." << endl;
            return false;
        }
    }
    return true;
}

/**
 * Receives a scheduler and with it builds the menu for the listings.
 * @param scheduler
 * @return Returns the last command that was made.
 */
unsigned listingsMenu(Scheduler &scheduler) {
    unsigned char commandIn;

    while (commandIn != 'q') {

        //Header
        cout << endl;
        cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "LISTI";
        cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "NGS" << endl;

        cout << setw(COLUMN_WIDTH) << setfill(' ') << "List all students: [1]" << setw(COLUMN_WIDTH)
             << "List a class's students: [2]" << setw(COLUMN_WIDTH)
             << "List an UC's students: [3]" << setw(COLUMN_WIDTH) << endl;
        cout << setw(COLUMN_WIDTH) << "List all students in a given year: [4]" << setw(COLUMN_WIDTH) << left
             << "List all students in more than n UC's: [5]" << setw(COLUMN_WIDTH)
             << "Check a student's schedule: [6]" << endl;
        cout << setw(COLUMN_WIDTH) << "List all UCs: [7]" << setw(COLUMN_WIDTH)
             << "List all UC's occupation: [8]" << setw(COLUMN_WIDTH)
             << "Check an UC's occupation: [9]" << endl;
        cout << setw(COLUMN_WIDTH) << "List all classes: [A]" << setw(COLUMN_WIDTH)
             << "List an UC's classes' occupation: [B]" << setw(COLUMN_WIDTH)
             << "Check a class's occupation: [C]" << endl;
        cout << setw(COLUMN_WIDTH) << "List all years' occupation: [D]" << setw(COLUMN_WIDTH)
             << "Check a year's occupation: [E]" << endl;
        cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;


        cout << endl << "Press the appropriate key to the function you'd like to access: ";
        cin >> commandIn;
        if (!checkForAppropriateInput(1)) {
            commandIn = '\0';
            continue;

        }
        switch (commandIn) {
            case '1': {
                for (shared_ptr<Student> student: scheduler.getStudents()) student->print();
                break;
            }
            case '2': {
                string classCode, ucCode;
                cout << "Enter the code of the class whose student's you'd like to check: ";
                cin >> classCode;
                if (!checkForAppropriateInput()) break;
                cout << "Enter the code of the UC this class refers to: ";
                cin >> ucCode;
                if (!checkForAppropriateInput()) break;
                vector<shared_ptr<Student>> studentsInClass = scheduler.getStudentsInClass(classCode, ucCode);
                if (studentsInClass.empty()) break;
                for (shared_ptr<Student> student: studentsInClass) student->print();
                break;
            }
            case '3': {
                string ucCode;
                cout << "Enter the code of the UC whose student's you'd like to check: ";
                cin >> ucCode;
                if (!checkForAppropriateInput()) continue;
                vector<shared_ptr<Student>> studentsInUC = scheduler.getStudentsInUc(ucCode);
                if (studentsInUC.empty()) break;
                for (shared_ptr<Student> student: studentsInUC) student->print();
                break;
            }
            case '4': {
                unsigned year;
                cout << "Enter the year whose student's you'd like to check: ";
                cin >> year;
                if (!checkForAppropriateInput()) continue;
                vector<shared_ptr<Student>> studentsInYear = scheduler.getStudentsInYear(year);
                if (studentsInYear.empty()) break;
                for (shared_ptr<Student> student: studentsInYear) student->print();
                break;
            }
            case '5': {
                unsigned numUCs, order;
                cout << "Enter the number of UCs you'd like to check for: ";
                cin >> numUCs;
                if (!checkForAppropriateInput()) break;
                cout << "Enter a 0 for alphabetical ordering of the results, or a 1 for decreasing number of UCs: ";
                cin >> order;
                if (!checkForAppropriateInput() || (order != 0 && order != 1)) break;
                vector<shared_ptr<Student>> studentsInMoreThanXUcs = scheduler.getStudentsInMoreThanXUcs(numUCs, order);
                if (studentsInMoreThanXUcs.empty()) break;
                for (shared_ptr<Student> student: studentsInMoreThanXUcs) student->print();
                break;
            }
            case '6': {
                unsigned studentNum;
                cout << "Enter the number of the student whose schedule you'd like to check: ";
                cin >> studentNum;
                if (!checkForAppropriateInput()) break;
                shared_ptr<Student> studentToCheck = scheduler.findStudent(studentNum);
                if (studentToCheck == nullptr) {
                    studentDoesntExist();
                    break;
                }
                vector<Student::scheduleBlock> studentSchedule = studentToCheck->getSchedule();
                for (Student::scheduleBlock block: studentSchedule) {
                    float startHour = block.slot.getStartHour();
                    float endHour = block.slot.getStartHour() + block.slot.getDuration();
                    cout << block.slot.getWeekday() << ": " << (int) startHour << ":" << setw(2)
                         << setfill('0') << (startHour - (int) startHour) * 60 << "-"
                         << (int) (endHour) << ":" << setw(2) << setfill('0') << (endHour - (int) endHour) * 60
                         << " -> " << block.aClass->getUcCode() << " | " << block.aClass->getClassCode() << endl;
                    cout << setfill(' ');
                }
                break;
            }
            case '7': {
                for (shared_ptr<UC> uc: scheduler.getUcs()) uc->print();
                break;
            }
            case '8': {
                map<shared_ptr<UC>, unsigned> occupations = scheduler.getAllUCsOccupation();
                cout << "UcCode: NumStudents" << endl << endl;
                for (auto pair: occupations) {
                    cout << pair.first->getUcCode() << ": " << pair.second << endl;
                }
                break;
            }
            case '9': {
                string ucCode;
                unsigned occupation;
                cout << "Enter the code of the UC whose occupation you'd like to check: ";
                cin >> ucCode;
                if (!checkForAppropriateInput()) break;
                else if (scheduler.findUc(ucCode) == nullptr) {
                    ucDoesntExist();
                    break;
                }
                occupation = scheduler.getUCOccupation(ucCode);
                cout << "UcCode: NumStudents" << endl;
                cout << ucCode << ": " << occupation << endl;
                break;
            }
            case 'A': {
                for (shared_ptr<Class> aClass: scheduler.getClasses()) aClass->print();
                break;
            }
            case 'B': {
                string ucCode;
                cout << "Enter a uc code to check its classses' occupations: ";
                cin >> ucCode;
                if (!checkForAppropriateInput()) break;
                shared_ptr<UC> uc = scheduler.findUc(ucCode);
                if (uc == nullptr) {
                    ucDoesntExist();
                    break;
                }
                map<shared_ptr<Class>, unsigned> occupations = scheduler.getAllClassesOccupation(uc);
                cout << "UcCode | ClassCode: NumStudents" << endl << endl;
                for (auto pair: occupations) {
                    cout << pair.first->getUcCode() << " | " << pair.first->getClassCode() << ": " << pair.second
                         << endl;
                }
                break;
            }
            case 'C': {
                string classCode, ucCode;
                unsigned occupation;
                cout << "Enter the code of the class whose occupation you'd like to check: ";
                cin >> classCode;
                if (!checkForAppropriateInput()) break;
                cout << "Enter the code of the UC this class refers to: ";
                cin >> ucCode;
                if (!checkForAppropriateInput()) break;
                shared_ptr<UC> ucOfTheClass = scheduler.findUc(ucCode);
                if (ucOfTheClass == nullptr) {
                    ucDoesntExist();
                    break;
                }
                shared_ptr<Class> classToCheck = scheduler.findClass(classCode, ucCode);
                if (classToCheck == nullptr) {
                    classDoesntExist();
                    break;
                }
                occupation = scheduler.getClassOccupation(classToCheck);
                cout << classToCheck->getUcCode() << " | " << classToCheck->getClassCode() << ": " << occupation
                     << endl;
                break;
            }
            case 'D': {
                cout << "Year: NumStudents" << endl;
                std::map<unsigned, unsigned> yearsOccupation = scheduler.getAllYearsOccupation();
                for (auto pair: yearsOccupation) {
                    cout << pair.first << ": " << pair.second << endl;
                }
                break;
            }
            case 'E': {
                unsigned year, occupation;
                cout << "Enter the year whose occupation you'd like to check: ";
                cin >> year;
                if (!checkForAppropriateInput()) break;
                if (year > 3 || year < 1) cout << "Please enter a valid bachelor's degree year!";
                else {
                    occupation = scheduler.getYearOccupation(year);
                    cout << "Year: NumStudents" << endl;
                    cout << year << ": " << occupation << endl;
                }
                break;

            }
            case 'b': {
                return '\0';
            }
            case 'q': {
                cout << "Thank you for using LEIC Schedule Management System!" << endl;
                break;
            }
            default:
                cout << "Please press one of listed keys." << endl;
                break;
        }
    }
    return commandIn;
}

/**
 * Receives a scheduler and with it builds the menu for the data alterations.
 * @param scheduler
 * @return Returns the last command that was made.
 */
unsigned dataAlterationsMenu(Scheduler &scheduler) {
    unsigned char commandIn;
    string menu = "hub";

    while (commandIn != 'q') {
        //Header
        cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "DATA ALT";
        cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "ERATIONS" << endl;

        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Alter students: [1]" << setw(COLUMN_WIDTH)
             << "Alter UCs: [2]" << setw(COLUMN_WIDTH) << "Alter Classes: [3]" << endl;
        cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;


        while (commandIn != 'q' && menu == "hub") {
            cout << endl << "Press the appropriate key to the function you'd like to access: ";
            cin >> commandIn;
            if (!checkForAppropriateInput(1)) {
                commandIn = '\0';
                continue;

            }
            switch (commandIn) {
                case '1': {
                    menu = "students";
                    break;
                }
                case '2': {
                    menu = "ucs";
                    break;
                }
                case '3': {
                    menu = "classes";
                    break;
                }
                case 'b': {
                    return '\0';
                }
                case 'q': {
                    cout << "Thank you for using LEIC Schedule Management System!" << endl;
                    break;
                }
                default:
                    cout << "Please press one of listed keys." << endl;
                    break;
            }
        }

        if (menu == "students") {
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "STUDENT A";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "LTERATIONS" << endl;

            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Add a new student: [1]" << setw(COLUMN_WIDTH)
                 << "Remove a student: [2]" << setw(COLUMN_WIDTH) << "Alter a student's number: [3]" << endl;
            cout << setw(COLUMN_WIDTH) << "Alter a student's name: [4]" << setw(COLUMN_WIDTH) << endl;
            cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }

        while (commandIn != 'q' && menu == "students") {
            cout << endl << "Press the appropriate key to the function you'd like to access: ";
            cin >> commandIn;
            if (!checkForAppropriateInput(1)) {
                commandIn = '\0';
                continue;

            }

            switch (commandIn) {
                case '1': {
                    unsigned studentNum;
                    string studentName;
                    cout << "Enter the number of the student you'd like to add: ";
                    cin >> studentNum;

                    if (!checkForAppropriateInput()) break;
                    else if (scheduler.findStudent(studentNum) != nullptr) studentAlreadyExists();
                    else {
                        cout << "Enter the name of the student you'd like to add: ";
                        cin >> studentName;
                        if (!checkForAppropriateInput()) break;
                        shared_ptr<Student> newStudent(new Student(studentNum, studentName));
                        scheduler.addStudent(newStudent);
                        cout << "Student successfully added." << endl;
                        CHANGES = true;
                    }
                    break;
                }
                case '2': {
                    unsigned studentNum;
                    cout << "Enter the number of the student you'd like to remove: ";
                    cin >> studentNum;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<Student> studentToRemove = scheduler.findStudent(studentNum);
                    if (studentToRemove == nullptr) studentDoesntExist();
                    else {
                        scheduler.removeStudent(studentToRemove);
                        cout << "Student successfully removed." << endl;
                        CHANGES = true;
                    }
                    break;
                }
                case '3': {
                    unsigned studentNum, newStudentNum;
                    cout << "Enter the number of the student you'd like to alter: ";
                    cin >> studentNum;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<Student> studentToAlter = scheduler.findStudent(studentNum);
                    if (studentToAlter == nullptr) studentDoesntExist();
                    else {
                        cout << "Enter the new number for this student: ";
                        cin >> newStudentNum;
                        if (!checkForAppropriateInput()) break;
                        else if (scheduler.findStudent(newStudentNum) != nullptr) studentAlreadyExists();
                        else {
                            scheduler.removeStudent(studentToAlter);
                            studentToAlter->setStudentCode(newStudentNum);
                            scheduler.addStudent(studentToAlter);
                            cout << "Student successfully altered." << endl;
                            CHANGES = true;
                        }
                    }
                    break;
                }
                case '4': {
                    unsigned studentNum;
                    string newStudentName;
                    cout << "Enter the number of the student you'd like to alter: ";
                    cin >> studentNum;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<Student> studentToAlter = scheduler.findStudent(studentNum);
                    if (studentToAlter == nullptr) studentDoesntExist();
                    else {
                        cout << "Enter the new name for this student: ";
                        cin >> newStudentName;
                        if (!checkForAppropriateInput()) break;
                        scheduler.removeStudent(studentToAlter);
                        studentToAlter->setStudentName(newStudentName);
                        scheduler.addStudent(studentToAlter);
                        cout << "Student successfully altered." << endl;
                        CHANGES = true;
                    }
                    break;
                }
                case 'b': {
                    menu = "hub";
                    break;
                }
                case 'q': {
                    cout << "Thank you for using LEIC Schedule Management System!" << endl;
                    break;
                }
                default: {
                    cout << "Please press one of listed keys." << endl;
                    break;
                }
            }
        }


        if (menu == "ucs") {
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "UC ALTE";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "RATIONS" << endl;

            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Add a new UC: [1]" << setw(COLUMN_WIDTH)
                 << "Remove a UC: [2]" << setw(COLUMN_WIDTH) << "Alter an UC's code: [3]" << endl;
            cout << setw(COLUMN_WIDTH) << "Add a class to a given UC: [4]" << setw(COLUMN_WIDTH)
                 << "Remove a class from a given UC: [5]" << endl;
            cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }
        while (commandIn != 'q' && menu == "ucs") {
            cout << endl << "Press the appropriate key to the function you'd like to access: ";
            cin >> commandIn;
            if (!checkForAppropriateInput(1)) {
                commandIn = '\0';
                continue;

            }

            switch (commandIn) {
                case '1': {
                    string ucCode;
                    cout << "Enter the code of the UC you'd like to add: ";
                    cin >> ucCode;

                    if (!checkForAppropriateInput()) break;
                    else if (scheduler.findUc(ucCode) != nullptr) ucAlreadyExists();
                    else {
                        shared_ptr<UC> newUC = static_cast<shared_ptr<UC>>(new UC(ucCode));
                        scheduler.addUc(newUC);
                        cout << "UC successfully added." << endl;
                        CHANGES = true;
                    }
                    break;
                }
                case '2': {
                    string ucCode;
                    cout << "Enter the code of the UC you'd like to remove: ";
                    cin >> ucCode;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<UC> ucToRemove = scheduler.findUc(ucCode);
                    if (ucToRemove == nullptr) ucDoesntExist();
                    else {
                        scheduler.removeUc(ucToRemove);
                        cout << "UC successfully removed." << endl;
                        CHANGES = true;
                    }
                    break;
                }
                case '3': {
                    string ucCode, newUcCode;
                    cout << "Enter the code of the uc you'd like to alter: ";
                    cin >> ucCode;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<UC> ucToAlter = scheduler.findUc(ucCode);
                    if (ucToAlter == nullptr) ucDoesntExist();
                    else {
                        cout << "Enter the new code for this UC: ";
                        cin >> newUcCode;
                        if (!checkForAppropriateInput()) break;
                        else if (scheduler.findUc(newUcCode) != nullptr) ucAlreadyExists();
                        else {
                            ucToAlter->setUcCode(newUcCode);
                            cout << "UC successfully altered." << endl;
                            CHANGES = true;
                        }
                    }
                    break;
                }
                case '4': {
                    string ucCode, classCode;
                    cout << "Enter the code of the UC you'd like to alter: ";
                    cin >> ucCode;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<UC> ucToAlter = scheduler.findUc(ucCode);
                    if (ucToAlter == nullptr) ucDoesntExist();
                    else {
                        cout << "Enter the code of the class you'd like to add: ";
                        cin >> classCode;
                        if (!checkForAppropriateInput()) break;
                        shared_ptr<Class> classToAdd = scheduler.findClass(classCode, ucCode);
                        if (classToAdd != nullptr) classAlreadyExists();
                        else {
                            classToAdd = static_cast<shared_ptr<Class>>(new Class(classCode,
                                                                                  ucCode));
                            scheduler.addClass(classToAdd);

                            ucToAlter->addClass(classToAdd);
                            cout << "UC successfully altered." << endl;
                            CHANGES = true;
                        }
                    }
                    break;
                }
                case '5': {
                    string ucCode, classCode;
                    cout << "Enter the code of the UC you'd like to alter: ";
                    cin >> ucCode;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<UC> ucToAlter = scheduler.findUc(ucCode);
                    if (ucToAlter == nullptr) ucDoesntExist();
                    else {
                        cout << "Enter the code of the class you'd like to remove: ";
                        cin >> classCode;
                        if (!checkForAppropriateInput()) break;
                        if (scheduler.findClass(classCode, ucCode) == nullptr) classDoesntExist();
                        else {
                            shared_ptr<Class> classToRemove = scheduler.findClass(classCode, ucCode);
                            scheduler.removeClass(
                                    classToRemove); //In our concept of a class it no longer exists if not in a uc
                            cout << "UC successfully altered." << endl;
                            CHANGES = true;
                        }
                    }
                    break;
                }
                case 'b': {
                    menu = "hub";
                    break;
                }
                case 'q': {
                    cout << "Thank you for using LEIC Schedule Management System!" << endl;
                    break;
                }
                default:
                    cout << "Please press one of listed keys." << endl;
                    break;
            }
        }

        if (menu == "classes") {
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "CLASS ALT";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "ERATIONS" << endl;

            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Add a new class: [1]" << setw(COLUMN_WIDTH)
                 << "Remove a class: [2]" << setw(COLUMN_WIDTH) << "Alter a class's code: [3]" << endl;
            cout << setw(COLUMN_WIDTH) << "Add a block to a class's schedule: [4]" << setw(COLUMN_WIDTH)
                 << "Remove a block from a class's schedule: [5]" << setw(COLUMN_WIDTH)
                 << "Alter a block in a class's schedule: [6]" << endl;
            cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }


        while (commandIn != 'q' && menu == "classes") {
            cout << endl << "Press the appropriate key to the function you'd like to access: ";
            cin >> commandIn;
            if (!checkForAppropriateInput(1)) {
                commandIn = '\0';
                continue;

            }
            switch (commandIn) {
                case '1': {
                    string classCode, ucCode;
                    cout << "Enter the code of the class you'd like to add: ";
                    cin >> classCode;
                    if (!checkForAppropriateInput()) break;
                    cout << "Enter the code of the UC this class refers to: ";
                    cin >> ucCode;
                    if (!checkForAppropriateInput()) break;
                    shared_ptr<UC> ucOfTheClass = scheduler.findUc(ucCode);
                    if (ucOfTheClass == nullptr) ucDoesntExist();
                    else if (scheduler.findClass(classCode, ucCode) != nullptr) classAlreadyExists();
                    else {
                        shared_ptr<Class> newClass(new Class(classCode, ucCode));
                        scheduler.addClass(newClass);
                        ucOfTheClass->addClass(newClass);
                        cout << "Class successfully added." << endl;
                        CHANGES = true;
                    }
                    break;
                }
                case '2': {
                    string classCode, ucCode;
                    cout << "Enter the code of the class you'd like to remove: ";
                    cin >> classCode;
                    if (!checkForAppropriateInput()) break;
                    cout << "Enter the code of the UC this class refers to: ";
                    cin >> ucCode;
                    if (!checkForAppropriateInput()) break;
                    shared_ptr<UC> ucOfTheClass = scheduler.findUc(ucCode);
                    if (ucOfTheClass == nullptr) ucDoesntExist();
                    else {
                        shared_ptr<Class> classToRemove = scheduler.findClass(classCode, ucCode);
                        if (classToRemove == nullptr) classDoesntExist();
                        else {
                            scheduler.removeClass(classToRemove);
                            cout << "Class successfully removed." << endl;
                            CHANGES = true;
                        }
                    }
                    break;
                }
                case '3': {
                    string ucCode, classCode, newClassCode;
                    cout << "Enter the code of the class you'd like to alter: ";
                    cin >> classCode;
                    if (!checkForAppropriateInput()) break;
                    cout << "Enter the code of the UC this class refers to: ";
                    cin >> ucCode;
                    if (!checkForAppropriateInput()) break;
                    shared_ptr<UC> ucOfTheClass = scheduler.findUc(ucCode);
                    if (ucOfTheClass == nullptr) ucDoesntExist();
                    else {
                        shared_ptr<Class> classToAlter = scheduler.findClass(classCode, ucCode);
                        if (classToAlter == nullptr) classDoesntExist();
                        else {
                            cout << "Enter the new code for this class: ";
                            cin >> newClassCode;
                            if (!checkForAppropriateInput()) break;
                            else if (scheduler.findClass(newClassCode, ucCode) != nullptr) {
                                classAlreadyExists();
                                break;
                            }
                            classToAlter->setClassCode(newClassCode);
                            cout << "Class successfully altered." << endl;
                            CHANGES = true;
                        }
                        break;
                    }
                }
                case '4': {
                    string ucCode, classCode, weekday, type;
                    float startHour, endHour, duration;
                    std::vector<std::string> weekdays{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
                                                      "Saturday", "Sunday"};

                    cout << "Enter the code of the UC you'd like to alter: ";
                    cin >> ucCode;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<UC> ucToAlter = scheduler.findUc(ucCode);
                    if (ucToAlter == nullptr) {
                        ucDoesntExist();
                        break;
                    } else {
                        cout << "Enter the code of the class you'd like to add a slot to: ";
                        cin >> classCode;
                        if (!checkForAppropriateInput()) break;
                        if (scheduler.findClass(classCode, ucCode) == nullptr) {
                            classDoesntExist();
                            break;
                        } else {
                            shared_ptr<Class> classToAlter = scheduler.findClass(classCode, ucCode);
                            std::set<Slot> scheduleToAlter = classToAlter->getClassSchedule();
                            cout << "Enter the weekday of the slot you'd like to add: ";
                            cin >> weekday;
                            if (!checkForAppropriateInput()) break;
                            else if (find(weekdays.begin(), weekdays.end(), weekday) == weekdays.end()) {
                                cout << "Please enter a valid weekday with a capital first letter!" << endl;
                                break;
                            }
                            cout
                                    << "Enter (in hours) the starting hour and the end hour of the slot you'd like to add: ";
                            cin >> startHour;
                            if (!checkForAppropriateInput()) break;
                            cin >> endHour;
                            if (!checkForAppropriateInput()) break;
                            duration = endHour - startHour;
                            cout << "Enter the type of the class you'd like to add: ";
                            cin >> type;
                            if (type != "T" && type != "TP" && type != "PL") {
                                cout << "Please enter a valid type for this slot!" << endl;
                                break;
                            }

                            bool invalid = false;
                            Slot addedSlot(weekday, startHour, duration, type);
                            for (Slot slot: scheduleToAlter) {
                                if (slot.conflictingSlot(addedSlot)) {
                                    cout << "This slot conflicts with an already existing one for this class!" << endl;
                                    invalid = true;
                                    break;
                                }
                            }
                            if (!invalid) {
                                classToAlter->addSlotToSchedule(addedSlot);
                                cout << "Slot successfully added." << endl;
                                CHANGES = true;
                            }
                        }
                    }
                    break;
                }
                case '5': {
                    string ucCode, classCode, weekday, type;
                    float startHour, endHour, duration;
                    std::vector<std::string> weekdays{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
                                                      "Saturday", "Sunday"};

                    cout << "Enter the code of the UC you'd like to alter: ";
                    cin >> ucCode;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<UC> ucToAlter = scheduler.findUc(ucCode);
                    if (ucToAlter == nullptr) {
                        ucDoesntExist();
                        break;
                    } else {
                        cout << "Enter the code of the class whose slot you'd like to remove: ";
                        cin >> classCode;
                        if (!checkForAppropriateInput()) break;
                        if (scheduler.findClass(classCode, ucCode) == nullptr) {
                            classDoesntExist();
                            break;
                        } else {
                            shared_ptr<Class> classToAlter = scheduler.findClass(classCode, ucCode);
                            std::set<Slot> scheduleToAlter = classToAlter->getClassSchedule();
                            cout << "Enter the weekday of the slot you'd like to remove: ";
                            cin >> weekday;
                            if (!checkForAppropriateInput()) break;
                            else if (find(weekdays.begin(), weekdays.end(), weekday) == weekdays.end()) {
                                cout << "Please enter a valid weekday with a capital first letter!" << endl;
                                break;
                            }
                            cout
                                    << "Enter (in hours) the starting hour and the end hour of the slot you'd like to remove: ";
                            cin >> startHour;
                            if (!checkForAppropriateInput()) break;
                            cin >> endHour;
                            if (!checkForAppropriateInput()) break;
                            duration = endHour - startHour;
                            cout << "Enter the type of the class you'd like to remove: ";
                            cin >> type;
                            if (type != "T" && type != "TP" && type != "PL") {
                                cout << "Please enter a valid type for this slot!" << endl;
                                break;
                            }

                            bool removed = false;
                            Slot erasedSlot(weekday, startHour, duration, type);
                            for (Slot slot: scheduleToAlter) {
                                if (slot == erasedSlot) {
                                    classToAlter->removeSlotFromSchedule(erasedSlot);
                                    cout << "Slot successfully removed." << endl;
                                    removed = true;
                                    CHANGES = true;
                                    break;
                                }
                            }
                            if (!removed) {
                                cout << "The slot you tried to remove doesn't exist!" << endl;
                            }
                        }
                    }
                    break;
                }
                case '6': {
                    string ucCode, classCode, weekday, newType, newWeekday;
                    float startHour, endHour, duration, newStartHour, newEndHour, newDuration;
                    std::vector<std::string> weekdays{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
                                                      "Saturday", "Sunday"};

                    cout << "Enter the code of the UC you'd like to alter: ";
                    cin >> ucCode;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<UC> ucToAlter = scheduler.findUc(ucCode);
                    if (ucToAlter == nullptr) {
                        ucDoesntExist();
                        break;
                    } else {
                        cout << "Enter the code of the class whose slot you'd like to alter: ";
                        cin >> classCode;
                        if (!checkForAppropriateInput()) break;
                        if (scheduler.findClass(classCode, ucCode) == nullptr) {
                            classDoesntExist();
                            break;
                        } else {
                            shared_ptr<Class> classToAlter = scheduler.findClass(classCode, ucCode);
                            std::set<Slot> scheduleToAlter = classToAlter->getClassSchedule();
                            std::list<Slot> potentialSlots;
                            cout << "Enter the new weekday for the slot you'd like to alter: ";
                            cin >> weekday;
                            if (!checkForAppropriateInput()) break;
                            else if (find(weekdays.begin(), weekdays.end(), weekday) == weekdays.end()) {
                                cout << "Please enter a valid weekday with a capital first letter!" << endl;
                                break;
                            }
                            for (auto itr = scheduleToAlter.begin(); itr != scheduleToAlter.end(); itr++) {
                                if (itr->getWeekday() == weekday) potentialSlots.push_back(*itr);
                            }
                            if (potentialSlots.size() == 0) {
                                cout << "There is no slot from this class in this weekday!" << endl;
                                break;
                            } else {
                                cout
                                        << "Enter (in hours) the starting hour and the end hour of the slot you'd like to alter: ";
                                cin >> startHour;
                                if (!checkForAppropriateInput()) break;
                                cin >> endHour;
                                if (!checkForAppropriateInput()) break;
                                duration = endHour - startHour;

                                for (auto itr = potentialSlots.begin(); itr != potentialSlots.end(); itr++) {
                                    if (itr->getStartHour() != startHour &&
                                        itr->getDuration() != duration)
                                        potentialSlots.erase(itr);
                                }
                                if (potentialSlots.size() == 0) {
                                    cout
                                            << "There is no slot starting at that time or with that end hour in this day!"
                                            << endl;
                                    break;
                                } else {
                                    cout << "Enter the weekday for the class to take place: ";
                                    cin >> newWeekday;
                                    if (!checkForAppropriateInput()) break;
                                    cout << "Enter the starting hour and the end hour for the slot: ";
                                    cin >> newStartHour;
                                    if (!checkForAppropriateInput()) break;
                                    cin >> newEndHour;
                                    newDuration = newEndHour - newStartHour;
                                    if (!checkForAppropriateInput()) break;
                                    cout << "Enter the type of class the slot will now refer to: ";
                                    cin >> newType;
                                    if (newType != "T" && newType != "TP" && newType != "PL") {
                                        cout << "Please enter a valid type for this slot!" << endl;
                                        break;
                                    }

                                    bool invalid = false;
                                    Slot alteredSlot(newWeekday, newStartHour, newDuration, newType);

                                    for (Slot slot: scheduleToAlter) {
                                        if (slot != potentialSlots.front() && slot.conflictingSlot(alteredSlot)) {
                                            cout << "This slot conflicts with an already existing one for this class!"
                                                 << endl;
                                            break;
                                            invalid = true;
                                        }
                                    }
                                    if (!invalid) {
                                        classToAlter->removeSlotFromSchedule(potentialSlots.front());
                                        classToAlter->addSlotToSchedule(alteredSlot);
                                        cout << "Slot successfully altered." << endl;
                                        CHANGES = true;
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
                case 'b': {
                    menu = "hub";
                    break;
                }
                case 'q': {
                    cout << "Thank you for using LEIC Schedule Management System!" << endl;
                    break;
                }
                default:
                    cout << "Please press one of listed keys." << endl;
                    break;
            }
        }
    }
    return commandIn;
}

/**
 * Receives a scheduler and with it builds the menu for the request.
 * @param scheduler
 * @return Returns the last command that was made.
 */
unsigned char requestsMenu(Scheduler &scheduler) {
    unsigned char commandIn;
    string menu = "hub";
    while (commandIn != 'q') {
        if (menu == "hub") {
            //Header
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "MANAGE R";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "EQUESTS" << endl;

            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Process all requests: [1]" << setw(COLUMN_WIDTH)
                 << "Check pending requests: [2]"
                 << setw(COLUMN_WIDTH) << "Check rejected requests: [3]" << endl;
            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Add a request: [4]" << setw(COLUMN_WIDTH);
            cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }

        while (commandIn != 'q' && menu == "hub") {
            cout << endl << "Press the appropriate key to the function you'd like to access: ";
            cin >> commandIn;
            if (!checkForAppropriateInput(1)) {
                commandIn = '\0';
                continue;
            }
            switch (commandIn) {
                case '1': {
                    if (scheduler.getRequests().size() == 0)
                        cout << "There are currently no pending requests!" << endl;
                    else scheduler.processRequests(CHANGES);
                    break;
                }
                case '2': {
                    queue<shared_ptr<Request>> requests = scheduler.getRequests();
                    if (requests.size() == 0)cout << "There are currently no pending requests!" << endl;
                    else {
                        for (size_t i = 0; i < requests.size(); i++) {
                            requests.front()->print();
                        }
                    }
                    break;
                }
                case '3': {
                    if (scheduler.getRejectedRequests().size() == 0)
                        cout << "There are currently no rejected requests!";
                    else {
                        for (shared_ptr<Request> request: scheduler.getRejectedRequests()) {
                            request->print();
                        }
                    }
                    break;
                }
                case '4': {
                    menu = "placeRequest";
                    break;
                }
                case 'b': {
                    return '\0';
                }
                case 'q': {
                    return commandIn;
                }
                default: {
                    cout << "Please press one of listed keys." << endl;
                    break;
                }

            }
        }


        if (menu == "placeRequest") {
            //Header
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "PLACE A";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << " REQUEST" << endl;

            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Add a student to a class: [1]" << setw(COLUMN_WIDTH)
                 << "Remove a student from an UC: [2]"
                 << setw(COLUMN_WIDTH) << "Alter student's class for a given UC: [3]" << endl;
            cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }

        while (commandIn != 'q' && menu == "placeRequest") {
            cout << endl << "Press the appropriate key to the function you'd like to access: ";
            cin >> commandIn;
            if (!checkForAppropriateInput(1)) {
                commandIn = '\0';
                continue;
            }

            switch (commandIn) {
                case '1': {
                    unsigned studentNum;
                    string ucCode, classCode;
                    shared_ptr<UC> uc;
                    shared_ptr<Class> newClass;
                    cout << "Enter the number of the student you'd like to alter: ";
                    cin >> studentNum;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<Student> studentToAlter = scheduler.findStudent(studentNum);
                    if (studentToAlter == nullptr) studentDoesntExist();
                    else {
                        cout << "Enter the UC whose class you'd like to alter: ";
                        cin >> ucCode;
                        if (!checkForAppropriateInput()) break;
                        uc = scheduler.findUc(ucCode);
                        if (uc == nullptr) ucDoesntExist();
                        else {
                            cout << "Enter the new class for this UC: ";
                            cin >> classCode;
                            if (!checkForAppropriateInput()) break;
                            newClass = scheduler.findClass(classCode, ucCode);
                            if (newClass == nullptr) classDoesntExist();
                            else if (studentToAlter->isInClass(classCode, ucCode)) {
                                cout << "The student is already in this class!" << endl;
                                break;
                            } else {
                                shared_ptr<Request> newRequest(new Request(studentToAlter, uc, nullptr, newClass));
                                scheduler.addRequest(newRequest);
                                cout << "Request successfully placed." << endl;
                            }
                        }
                    }
                    break;
                }
                case '2': {
                    unsigned studentNum;
                    string ucCode, classCode;
                    shared_ptr<UC> uc;
                    shared_ptr<Class> newClass;
                    cout << "Enter the number of the student you'd like to alter: ";
                    cin >> studentNum;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<Student> studentToAlter = scheduler.findStudent(studentNum);
                    if (studentToAlter == nullptr) studentDoesntExist();
                    else {
                        cout << "Enter the UC whose class you'd like to alter: ";
                        cin >> ucCode;
                        if (!checkForAppropriateInput()) break;
                        uc = scheduler.findUc(ucCode);
                        if (uc == nullptr) ucDoesntExist();
                        else {
                            shared_ptr<Request> newRequest(
                                    new Request(studentToAlter, uc, studentToAlter->getClassForUC(uc), nullptr));
                            scheduler.addRequest(newRequest);
                            cout << "Request successfully placed." << endl;
                        }
                    }
                    break;
                }
                case '3': {
                    unsigned studentNum;
                    string ucCode, classCode;
                    shared_ptr<UC> uc;
                    shared_ptr<Class> newClass;
                    cout << "Enter the number of the student you'd like to alter: ";
                    cin >> studentNum;
                    if (!checkForAppropriateInput()) break;

                    shared_ptr<Student> studentToAlter = scheduler.findStudent(studentNum);
                    if (studentToAlter == nullptr) studentDoesntExist();
                    else {
                        cout << "Enter the UC whose class you'd like to alter: ";
                        cin >> ucCode;
                        if (!checkForAppropriateInput()) break;
                        uc = scheduler.findUc(ucCode);
                        if (uc == nullptr) ucDoesntExist();
                        else {
                            cout << "Enter the new class for this UC: ";
                            cin >> classCode;
                            if (!checkForAppropriateInput()) break;
                            newClass = scheduler.findClass(classCode, ucCode);
                            if (newClass == nullptr) classDoesntExist();
                            else if (studentToAlter->isInClass(classCode, ucCode)) {
                                cout << "The student is already in this class!" << endl;
                                break;
                            } else {
                                shared_ptr<Request> newRequest(
                                        new Request(studentToAlter, uc, studentToAlter->getClassForUC(uc),
                                                    newClass));
                                scheduler.addRequest(newRequest);
                                cout << "Request successfully placed." << endl;
                            }
                        }
                    }
                    break;

                }
                case 'b': {
                    menu = "hub";
                    break;
                }
                case 'q': {
                    return commandIn;
                }
                default: {
                    cout << "Please press one of listed keys." << endl;
                    break;
                }
            }
        }

    }
    return commandIn;
}

/**
 * Receives a scheduler and with it builds the main menu.
 * @param scheduler
 */
void mainMenu(Scheduler &scheduler) {
    unsigned char commandIn = '\0';
    string line;

    while (commandIn != 'q') {
        if (commandIn == '\0') { //If program just started or returned from a different menu, print header

            //Header
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "LEIC SCHEDULE M";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "ANAGEMENT SYSTEM" << endl;

            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Check listings: [1]" << setw(COLUMN_WIDTH)
                 << "Alter data: [2]"
                 << setw(COLUMN_WIDTH) << "Manage requests: [3]" << endl;
            cout << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }
        cout << endl << "Press the appropriate key to the function you'd like to access: ";
        cin >> commandIn;
        if (!checkForAppropriateInput(1)) {
            commandIn = '\0';
            continue;
        }
        switch (commandIn) {
            case '1': {
                commandIn = listingsMenu(scheduler);
                break;
            }
            case '2': {
                commandIn = dataAlterationsMenu(scheduler);
                break;
            }
            case '3': {
                commandIn = requestsMenu(scheduler);
                break;
            }
            case 'q': {
                cout << "Thank you for using LEIC Schedule Management System!";
                break;
            }
            default: {
                cout << "Please press one of listed keys." << endl;
                break;
            }
        }
    }
}

int main() {
    Scheduler scheduler;
    scheduler.extractFileInfo(CLASSES_FILE_PATH, CLASSES_PER_UC_FILE_PATH, STUDENTS_CLASSES_PATH);
    mainMenu(scheduler);
    if (CHANGES) {
        scheduler.writeFileInfo(CLASSES_FILE_PATH, CLASSES_PER_UC_FILE_PATH, STUDENTS_CLASSES_PATH);
    }
    return 0;
}

