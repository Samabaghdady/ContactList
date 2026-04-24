#include "contactlist.h"
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

const string default_filename = "contact.dat";



ContactList::Contact::Contact() : prev(nullptr), next(nullptr) {
    // Initialize members as needed
    firstName[0] = '\0';
    lastName[0] = '\0';
    Email[0] = '\0';
    address[0] = '\0';
    phone_number[0] = '\0';
}

ContactList::Contact::Contact(const char* fName, const char* lName, const char* email, const char* add, const char* phoneNo)
    : prev(nullptr), next(nullptr) {

    if (fName) {
        strncpy_s(firstName, fName, sizeof(firstName) - 1);
        firstName[sizeof(firstName) - 1] = '\0';
    }
    else {
        throw runtime_error("First name cannot be null");
    }

    if (lName) {
        strncpy_s(lastName, lName, sizeof(lastName) - 1);
        lastName[sizeof(lastName) - 1] = '\0';
    }
    else {
        throw runtime_error("Last name cannot be null");
    }

    if (add) {
        strncpy_s(address, add, sizeof(address) - 1);
        address[sizeof(address) - 1] = '\0';
    }
    else {
        throw runtime_error("Address cannot be null");
    }

    if (email) {
        strncpy_s(Email, email, sizeof(Email) - 1);
        Email[sizeof(Email) - 1] = '\0';
    }
    else {
        throw runtime_error("Email cannot be null");
    }

    if (phoneNo) {
        strncpy_s(phone_number, phoneNo, sizeof(phone_number) - 1);
        phone_number[sizeof(phone_number) - 1] = '\0';
    }
    else {
        throw runtime_error("Phone number cannot be null");
    }
}

ContactList::ContactList() : head(nullptr), tail(nullptr) {}

ContactList::~ContactList() {
    Contact* current = head;
    while (current) {
        Contact* toDelete = current;
        current = current->next;
        delete toDelete;
    }
}

void ContactList::addContact(const char* firstName, const char* lastName, const char* address, const char* email, const char* phoneNo) {
    Contact* newContact = new Contact(firstName, lastName, address, email, phoneNo);

    if (!head) {
        head = newContact;
        tail = newContact;
    }
    else {
        Contact* current = head;
        while (current) {
            int nameComparison = strcmp(newContact->firstName, current->firstName);

            if (nameComparison < 0 || (nameComparison == 0 && strcmp(newContact->lastName, current->lastName) < 0)) {
                newContact->next = current;
                newContact->prev = current->prev;

                if (current->prev) {
                    current->prev->next = newContact;
                }
                else {
                    head = newContact;
                }

                current->prev = newContact;
                return;
            }

            current = current->next;
        }

        tail->next = newContact;
        newContact->prev = tail;
        tail = newContact;
    }
}



void ContactList::addContactInput() {
    int choice = 0;

    do {
        char firstName[20], lastName[20], email[50], address[100], phoneNo[12];

        cout << "Enter the first name (maximum 10 characters): ";
        cin.getline(firstName, sizeof(firstName));

        cout << "Enter the last name (maximum 10 characters): ";
        cin.getline(lastName, sizeof(lastName));

        cout << "Enter the email (maximum 50 characters): ";
        cin.getline(email, sizeof(email));

        cout << "Enter the address (maximum 100 characters): ";
        cin.getline(address, sizeof(address));

        while (true) {
            cout << "Enter the phone number ( 11 digit ): ";
            cin.getline(phoneNo, sizeof(phoneNo));


            if (strlen(phoneNo) == 11) {
                break;
            }
            else {
                cout << "\n Invalid phone number. Please try again." << endl;
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        Contact* current = head;
        bool contactExists = false;

        while (current != nullptr) {
            //strcmp function is used for comparing
            if (strcmp(current->firstName, firstName) == 0 &&
                strcmp(current->lastName, lastName) == 0 &&
                strcmp(current->address, address) == 0 &&
                strcmp(current->Email, email) == 0 &&
                strcmp(current->phone_number, phoneNo) == 0) {
                contactExists = true;
                break;

            }
            current = current->next;
        }

        if (contactExists) {
            cout << "Contact already exists. Are you sure you want to add it again?\n1. Yes\n2. No\nEnter your choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice != 1) {


                continue;
            }
        }

        addContact(firstName, lastName, email, address, phoneNo);

        cout << "Contact added successfully!" << endl;

        cout << "Do you want to add another contact?\n1. Yes\n2. No\nEnter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (choice == 1);
}
bool ContactList::removeContact(const char* firstName, const char* lastName) {
    Contact* current = head;
    Contact* previous = nullptr;

    while (current != nullptr) {
        if (strcmp(current->firstName, firstName) == 0 && strcmp(current->lastName, lastName) == 0) {
            if (previous == nullptr) {
                head = current->next;
            }
            else {
                previous->next = current->next;
            }

            delete current;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false; // Contact not found
}

//void ContactList::removeContact(const char* fName, const char* lName) {
//    Contact* current = head;
//    while (current) {
//        if (strcmp(current->firstName, fName) == 0 && strcmp(current->lastName, lName) == 0) {
//            if (current->prev) current->prev->next = current->next;
//            if (current->next) current->next->prev = current->prev;
//            if (current == head) head = current->next;
//            if (current == tail) tail = current->prev;
//
//            delete current;
//            cout << "Contact removed." << endl;
//            return;
//        }
//        current = current->next;
//    }
//    cout << "Contact not found." << endl;
//}
void ContactList::editContact(const char* targetFName, const char* targetLName) {
    NodePointer current = head;
    NodePointer ptr = nullptr;
    //strcmp function is used for comparing 
    while (current != nullptr) {
        if (strcmp(current->firstName, targetFName) == 0 && strcmp(current->lastName, targetLName) == 0) {
            ptr = current;
            break;
        }

        current = current->next;
    }

    if (ptr != nullptr) {
        cout << "What do you want to edit?" << endl;
        int choice;
        cout << "1. Edit contact name" << endl;
        cout << "2. Edit phone number" << endl;
        cout << "3. Edit both" << endl;
        cout << "4.Edit address " << endl;
        cout << "5.Edit email " << endl;
        cout << "Your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Do you want to edit first name or last name?" << endl;
            int choice2;
            cout << "1. Edit first name" << endl;
            cout << "2. Edit last name" << endl;
            cout << "3. Edit both" << endl;
            cout << "Your choice: ";
            cin >> choice2;

            switch (choice2) {

            case 1: {
                char newFirstName[20];
                cout << "Enter the new first name: ";
                cin.ignore(); // Ignore the newline character in the buffer
                cin.getline(newFirstName, sizeof(newFirstName));
                strncpy_s(ptr->firstName, newFirstName, sizeof(ptr->firstName) - 1);
                ptr->firstName[sizeof(ptr->firstName) - 1] = '\0';
                break;
            }
            case 2: {
                char newLastName[20];
                cout << "Enter the new last name: ";
                cin.ignore();
                cin.getline(newLastName, sizeof(newLastName));
                strncpy_s(ptr->lastName, newLastName, sizeof(ptr->lastName) - 1);
                ptr->lastName[sizeof(ptr->lastName) - 1] = '\0';
                break;
            }
            case 3: {
                char newFirstName[20], newLastName[20];
                cout << "Enter the new first name: ";
                cin.ignore();
                cin.getline(newFirstName, sizeof(newFirstName));
                strncpy_s(ptr->firstName, newFirstName, sizeof(ptr->firstName) - 1);
                ptr->firstName[sizeof(ptr->firstName) - 1] = '\0';

                cout << "Enter the new last name: ";
                cin.getline(newLastName, sizeof(newLastName));
                strncpy_s(ptr->lastName, newLastName, sizeof(ptr->lastName) - 1);
                ptr->lastName[sizeof(ptr->lastName) - 1] = '\0';
                break;
            }
            default:
                cout << "Invalid choice. Please enter 1, 2, or 3." << endl;
                break;
            }
            break;
        }
        case 2: {
            char newPhoneNumber[11];
            while (true) {
                cout << "Enter the new phone number ( +123 01112242750)\nPlease enter a space after entering the country code: ";
                cin.getline(newPhoneNumber, sizeof(newPhoneNumber));

                if (strlen(newPhoneNumber) == 11) {
                    break;
                }
                else {
                    cout << "Invalid phone number or country code. Please try again." << endl;
                }
                cin.clear();
                // Clear the input buffer
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            strncpy_s(ptr->phone_number, newPhoneNumber, sizeof(ptr->phone_number) - 1);
            ptr->phone_number[sizeof(ptr->phone_number) - 1] = '\0';

            break;
        }
        case 3: {
            cout << "Do you want to edit first name or last name?" << endl;
            int choice2;
            cout << "1. Edit first name" << endl;
            cout << "2. Edit last name" << endl;
            cout << "3. Edit both" << endl;
            cout << "Your choice: ";
            cin >> choice2;

            switch (choice2) {
            case 1: {
                char newFirstName[20];
                cout << "Enter the new first name: ";
                cin.ignore();
                cin.getline(newFirstName, sizeof(newFirstName));
                strncpy_s(ptr->firstName, newFirstName, sizeof(ptr->firstName) - 1);
                ptr->firstName[sizeof(ptr->firstName) - 1] = '\0';
                break;
            }
            case 2: {
                char newLastName[20];
                cout << "Enter the new last name: ";
                cin.ignore();
                cin.getline(newLastName, sizeof(newLastName));
                strncpy_s(ptr->lastName, newLastName, sizeof(ptr->lastName) - 1);
                ptr->lastName[sizeof(ptr->lastName) - 1] = '\0';
                break;
            }
            case 3: {
                char newFirstName[20], newLastName[20];
                cout << "Enter the new first name: ";
                cin.ignore();
                cin.getline(newFirstName, sizeof(newFirstName));
                strncpy_s(ptr->firstName, newFirstName, sizeof(ptr->firstName) - 1);
                ptr->firstName[sizeof(ptr->firstName) - 1] = '\0';

                cout << "Enter the new last name: ";
                cin.getline(newLastName, sizeof(newLastName));
                strncpy_s(ptr->lastName, newLastName, sizeof(ptr->lastName) - 1);
                ptr->lastName[sizeof(ptr->lastName) - 1] = '\0';
                break;
            }

            default:
                cout << "Invalid choice. Please enter 1, 2,3,4 or 5." << endl;
                break;
            }
            char newPhoneNumber[11], newcountryCode[5];
            while (true) {
                cout << "Enter the country code and the new phone number (e.g., +123 4567890123)\nPlease enter a space after entering the country code: ";
                cin.getline(newcountryCode, sizeof(newcountryCode), ' ');
                cin.getline(newPhoneNumber, sizeof(newPhoneNumber));

                if (strlen(newPhoneNumber) == 11 && strlen(newcountryCode) <= 4 && newcountryCode[0] == '+') {
                    break;
                }
                else {
                    cout << "Invalid phone number or country code. Please try again." << endl;
                }
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            strncpy_s(ptr->phone_number, newPhoneNumber, sizeof(ptr->phone_number) - 1);
            ptr->phone_number[sizeof(ptr->phone_number) - 1] = '\0';

            break;
        }
        case 4: {
            cout << "Enter new address: ";
            cin.getline(ptr->address, sizeof(ptr->address));
            break;
        }
        case 5: {
            cout << "Enter new email: ";
            cin.getline(ptr->Email, sizeof(ptr->Email));
            break;


        }
        default:
            cout << "Invalid choice. Please enter 1, 2, or 3." << endl;
            break;
        }
    }
    else {
        cerr << "Contact not found." << endl;
    }
}

void ContactList::displayContacts() {
    Contact* current = head;
    while (current) {
        cout << "Name: " << current->firstName << " " << current->lastName << endl;
        cout << "Address: " << current->address << endl;
        cout << "Email: " << current->Email << endl;
        cout << "Phone: " << current->phone_number << endl;
        cout << "---------------------" << endl;
        current = current->next;
    }
}
void ContactList::saveToFile(const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        Contact* current = head;
        while (current) {
            outFile << current->firstName << ","
                << current->lastName << ","
                << current->Email << ","
                << current->address << ","
                << current->phone_number << endl;
            current = current->next;
        }
        outFile.close();
    }
    else {
       cerr << "Unable to open file for saving." << std::endl;
    }
}

void ContactList::loadFromFile(const std::string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string firstName, lastName, email, address, phone;

            getline(ss, firstName, ',');
            getline(ss, lastName, ',');
            getline(ss, email, ',');
            getline(ss, address, ',');
            getline(ss, phone, ',');

            addContact(firstName.c_str(), lastName.c_str(), email.c_str(), address.c_str(), phone.c_str());
        }
        inFile.close();
    }
    else {
        std::cerr << "Unable to open file for loading." << std::endl;
    }
}


void ContactList::searchByFullName(const char* firstName, const char* lastName) {
    Contact* current = head;
    bool found = false;
    while (current) {
        //strcmp function is used for comparing
        if (strcmp(current->firstName, firstName) == 0 && strcmp(current->lastName, lastName) == 0) {
            cout << "Contact found: " << current->firstName << " " << current->lastName << endl;
            found = true;
            break;
        }
        current = current->next;
    }

    if (!found) {
        cout << "Contact with full name " << firstName << " " << lastName << " not found." << endl;
    }
}

void ContactList::searchByFirstName(const char* firstName) {
    Contact* current = head;
    bool found = false;
    while (current) {
        if (strcmp(current->firstName, firstName) == 0) {
            std::cout << "Contact found: " << current->firstName << " " << current->lastName << std::endl;
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        cout << "No contacts found with first name: " << firstName << std::endl;
    }
}

void ContactList::searchByLastName(const char* lastName) {
    Contact* current = head;
    bool found = false;

    while (current) {
        if (strcmp(current->lastName, lastName) == 0) {
            cout << "Contact found: " << current->firstName << " " << current->lastName << endl;
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        cout << "No contacts found with last name: " << lastName << endl;
    }
}


void ContactList::search() {
    int option, searchChoice;
    char firstName[10];
    char lastName[10];

    do {
        cout << "Search by:\n1. First Name and Last Name\n2. First Name Only\n3. Last Name Only\n4. Exit\nEnter your choice: ";
        cin >> option;

        switch (option) {
        case 1:
            cout << "Enter the first name: ";
            cin >> firstName;
            cout << "Enter the last name: ";
            cin >> lastName;
            searchByFullName(firstName, lastName);
            break;
        case 2:
            cout << "Enter the first name: ";
            cin >> firstName;
            searchByFirstName(firstName);
            break;
        case 3:
            cout << "Enter the last name: ";
            cin >> lastName;
            searchByLastName(lastName);
            break;
        case 4:
            return;
        default:
            cout << "Invalid choice." << endl;
        }

        cout << "Do you want to search again?\n1. Yes\n2. No\nEnter your choice: ";
        cin >> searchChoice;

    } while (searchChoice == 1);
}



bool ContactList::isDuplicate(const char* firstName, const char* lastName, const char* email, const char* add, const char* phoneNumber) const {
    NodePointer current = head;
    while (current != nullptr) {
        //strcmp function is used for comparing
        if (strcmp(current->firstName, firstName) == 0 && strcmp(current->lastName, lastName) == 0 &&
            strcmp(current->Email, email) == 0 && strcmp(current->phone_number, phoneNumber) == 0 && strcmp(current->address, add) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void ContactList::clearContacts() {
    ofstream outFile(default_filename, ios::binary | ios::trunc);

    if (outFile.is_open()) {
        outFile.close();
        cout << "File cleared successfully." << endl;
    }
    else {
        cerr << "Unable to open file: " << default_filename << endl;
    }
}
