#include "contactlist.h"
#include <iostream>
using namespace std;

int main() {
    ContactList contactList;
    int choice;

    do {
        cout << "\n--- Contact List Menu ---\n";
        cout << "1. Add a Contact\n";
        cout << "2. Display All Contacts\n";
        cout << "3. Remove a Contact\n";
        cout << "4. Edit a Contact\n";
        cout << "5. Search for a Contact\n";
        cout << "6. Save Contacts to File\n";
        cout << "7. Load Contacts from File\n";
        cout << "8. Clear All Contacts\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        cin.ignore(); // To handle the newline character left in the buffer
        switch (choice) {
        case 1:
            contactList.addContactInput();
            break;

        case 2:
            contactList.displayContacts();
            break;

        case 3: {
            char firstName[20], lastName[20];
            cout << "Enter the first name of the contact to remove: ";
            cin.getline(firstName, sizeof(firstName));
            cout << "Enter the last name of the contact to remove: ";
            cin.getline(lastName, sizeof(lastName));
            contactList.removeContact(firstName, lastName);
            break;
        }

        case 4: {
            char firstName[20], lastName[20];
            cout << "Enter the first name of the contact to edit: ";
            cin.getline(firstName, sizeof(firstName));
            cout << "Enter the last name of the contact to edit: ";
            cin.getline(lastName, sizeof(lastName));
            contactList.editContact(firstName, lastName);
            break;
        }

        case 5:
            contactList.search();
            break;

      /*  case 6:
            contactList.saveToFile();
            cout << "Contacts saved to file successfully." << endl;
            break;*/

      /*  case 7:
            contactList.loadFromFile();
            break;

        case 8:
            contactList.clearContacts();
            break;*/

        case 9:
            cout << "Exiting the program. Goodbye!" << endl;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}
