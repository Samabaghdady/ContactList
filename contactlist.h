#ifndef CONTACT_LIST_H
#define CONTACT_LIST_H

#include <iostream>
#include <fstream>
#include <cstring> 
#include <vector>
#include <algorithm> // For std::transform
#include <cctype>    // For std::tolower

using namespace std;

class ContactList {

public: // Make the nested Contact class accessible
    class Contact {
    public:
        char firstName[20];
        char lastName[20];
        char Email[50];
        char address[100];
        char phone_number[12];
        Contact* prev;
        Contact* next;

        Contact();
        Contact(const char* firstName, const char* lastName, const char* address, const char* email, const char* phone_number);
    };

private: // Keep internal variables private
    typedef Contact* NodePointer;
    NodePointer head;
    NodePointer tail;
    Contact* current = head;

    // Helper function to convert a string to lowercase
    std::string toLower(const char* str) const {
        std::string lowerStr(str);
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }

public: // Public member functions
    ContactList();
    ~ContactList();

    void addContact(const char* firstName, const char* lastName, const char* address, const char* email, const char* phone_number);
    void addContactInput();
    bool removeContact(const char* firstName, const char* lastName);
    void editContact(const char* targetFName, const char* targetLName);
    void displayContacts();
    void saveToFile(const std::string& filename); // Declaration for saving contacts
    void loadFromFile(const std::string& filename); // Declaration for loading contacts
    void searchByFullName(const char* firstName, const char* lastName);
    void searchByFirstName(const char* firstName);
    void searchByLastName(const char* lastName);
    void search();
    Contact* getHead() const { return head; }
    bool isDuplicate(const char* firstName, const char* lastName, const char* address, const char* email, const char* phone_number) const;
    void clearContacts();
};

#endif
