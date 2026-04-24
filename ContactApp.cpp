#include <wx/wx.h>
#include <wx/listctrl.h>
#include "contactlist.h"
#include <wx/tokenzr.h>
#include <algorithm>
#include <cctype>

// Helper function to convert a string to lowercase
    string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

class ContactApp : public wxApp {
private:
    ContactList contactList; // Make contactList a member of ContactApp

public:
    virtual bool OnInit();
    virtual int OnExit();
};

class MainFrame : public wxFrame {
private:
    ContactList& contactList; // Reference to the contactList in ContactApp
    wxListCtrl* contactListCtrl;
    wxPanel* mainPanel; // Store a reference to the main panel for theme updates
    bool isDarkTheme = false; // Track the current theme state

    void OnAddContact(wxCommandEvent& event);
    void OnDeleteContact(wxCommandEvent& event);
    void OnEditContact(wxCommandEvent& event);
    void OnDisplayContacts(wxCommandEvent& event);
    void OnSearchContact(wxCommandEvent& event);
    void OnClearContacts(wxCommandEvent& event);
    void OnToggleTheme(wxCommandEvent& event); // New handler for toggling theme

public:
    MainFrame(const wxString& title, ContactList& list);
};


wxIMPLEMENT_APP(ContactApp);

bool ContactApp::OnInit() {
    contactList.loadFromFile("contacts.txt"); // Load contacts on startup
    MainFrame* frame = new MainFrame("Contact Management System", contactList);
    frame->Show(true);
    return true;
}

int ContactApp::OnExit() {
    contactList.saveToFile("contacts.txt"); // Save contacts on exit
    return 0;
}

class AddContactDialog : public wxDialog {
public:
    wxTextCtrl* firstNameCtrl;
    wxTextCtrl* lastNameCtrl;
    wxTextCtrl* emailCtrl;
    wxTextCtrl* addressCtrl;
    wxTextCtrl* phoneCtrl;

    AddContactDialog(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, "Add Contact", wxDefaultPosition, wxSize(600, 450)) {

        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        mainSizer->Add(new wxStaticText(this, wxID_ANY, "First Name:"), 0, wxALL, 5);
        firstNameCtrl = new wxTextCtrl(this, wxID_ANY);
        mainSizer->Add(firstNameCtrl, 0, wxEXPAND | wxALL, 5);

        mainSizer->Add(new wxStaticText(this, wxID_ANY, "Last Name:"), 0, wxALL, 5);
        lastNameCtrl = new wxTextCtrl(this, wxID_ANY);
        mainSizer->Add(lastNameCtrl, 0, wxEXPAND | wxALL, 5);

        mainSizer->Add(new wxStaticText(this, wxID_ANY, "Email:"), 0, wxALL, 5);
        emailCtrl = new wxTextCtrl(this, wxID_ANY);
        mainSizer->Add(emailCtrl, 0, wxEXPAND | wxALL, 5);

        mainSizer->Add(new wxStaticText(this, wxID_ANY, "Address:"), 0, wxALL, 5);
        addressCtrl = new wxTextCtrl(this, wxID_ANY);
        mainSizer->Add(addressCtrl, 0, wxEXPAND | wxALL, 5);

        mainSizer->Add(new wxStaticText(this, wxID_ANY, "Phone Number:"), 0, wxALL, 5);
        phoneCtrl = new wxTextCtrl(this, wxID_ANY);
        mainSizer->Add(phoneCtrl, 0, wxEXPAND | wxALL, 5);

        

        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* btnOK = new wxButton(this, wxID_OK, "OK");
        wxButton* btnCancel = new wxButton(this, wxID_CANCEL, "Cancel");
        buttonSizer->Add(btnOK, 1, wxALL, 5);
        buttonSizer->Add(btnCancel, 1, wxALL, 5);

        mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER);

        SetSizer(mainSizer);
    }
};

MainFrame::MainFrame(const wxString& title, ContactList& list)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900, 650)), contactList(list) {

    mainPanel = new wxPanel(this, wxID_ANY);

    wxButton* btnAdd = new wxButton(mainPanel, 1001, "Add Contact", wxPoint(50, 50), wxSize(200, 40));
    wxButton* btnDelete = new wxButton(mainPanel, 1002, "Delete Contact", wxPoint(50, 100), wxSize(200, 40));
    wxButton* btnEdit = new wxButton(mainPanel, 1003, "Edit Contact", wxPoint(50, 150), wxSize(200, 40));
    wxButton* btnDisplay = new wxButton(mainPanel, 1004, "Display Contacts", wxPoint(50, 200), wxSize(200, 40));
    wxButton* btnSearch = new wxButton(mainPanel, 1005, "Search Contact", wxPoint(50, 250), wxSize(200, 40));
    wxButton* btnClear = new wxButton(mainPanel, 1006, "Clear Contacts", wxPoint(50, 300), wxSize(200, 40));
    wxButton* btnToggleTheme = new wxButton(mainPanel, 1007, "Toggle Theme", wxPoint(50, 350), wxSize(200, 40));

    contactListCtrl = new wxListCtrl(mainPanel, wxID_ANY, wxPoint(300, 50), wxSize(500, 500), wxLC_REPORT | wxLC_SINGLE_SEL);

    contactListCtrl->InsertColumn(0, "First Name", wxLIST_FORMAT_LEFT, 100);
    contactListCtrl->InsertColumn(1, "Last Name", wxLIST_FORMAT_LEFT, 100);
    contactListCtrl->InsertColumn(2, "Email", wxLIST_FORMAT_LEFT, 100);
    contactListCtrl->InsertColumn(3, "Address", wxLIST_FORMAT_LEFT, 100);
    contactListCtrl->InsertColumn(4, "Phone Number", wxLIST_FORMAT_LEFT, 100);

    Bind(wxEVT_BUTTON, &MainFrame::OnAddContact, this, 1001);
    Bind(wxEVT_BUTTON, &MainFrame::OnDeleteContact, this, 1002);
    Bind(wxEVT_BUTTON, &MainFrame::OnEditContact, this, 1003);
    Bind(wxEVT_BUTTON, &MainFrame::OnDisplayContacts, this, 1004);
    Bind(wxEVT_BUTTON, &MainFrame::OnSearchContact, this, 1005);
    Bind(wxEVT_BUTTON, &MainFrame::OnClearContacts, this, 1006);
    Bind(wxEVT_BUTTON, &MainFrame::OnToggleTheme, this, 1007); // Bind the theme toggle button
}



void MainFrame::OnAddContact(wxCommandEvent& event) {
    AddContactDialog dialog(this);

    // Loop until valid input is provided or the dialog is canceled
    while (dialog.ShowModal() == wxID_OK) {
        wxString firstName = dialog.firstNameCtrl->GetValue();
        wxString lastName = dialog.lastNameCtrl->GetValue();
        wxString email = dialog.emailCtrl->GetValue();
        wxString address = dialog.addressCtrl->GetValue();
        wxString phone = dialog.phoneCtrl->GetValue();

        // Validation variables
        wxString errorMessage;

        // Validate fields for empty values
        if (firstName.IsEmpty()) errorMessage += "First name cannot be empty.\n";
        if (lastName.IsEmpty()) errorMessage += "Last name cannot be empty.\n";
        if (email.IsEmpty()) errorMessage += "Email cannot be empty.\n";
        if (address.IsEmpty()) errorMessage += "Address cannot be empty.\n";
        if (phone.IsEmpty()) errorMessage += "Phone number cannot be empty.\n";

        // Check if the phone number is 11 digits long
        if (!phone.IsEmpty() && phone.Length() != 11) {
            errorMessage += "Phone number must be 11 digits long.\n";
        }

        // Check if the email contains '@' and '.com'
        if (!email.IsEmpty() && (!email.Contains("@") || !email.EndsWith(".com"))) {
            errorMessage += "Email must contain '@' and end with '.com'.\n";
        }

        // If there are validation errors, show them and keep the dialog open
        if (!errorMessage.IsEmpty()) {
            wxMessageBox(errorMessage, "Validation Error", wxOK | wxICON_ERROR);
            continue; // Restart the loop to allow corrections
        }

        // Check for duplicate contact
        ContactList::Contact* current = contactList.getHead();
        bool contactExists = false;
        while (current != nullptr) {
            if (current->firstName == firstName.ToStdString() &&
                current->lastName == lastName.ToStdString() &&
                current->Email == email.ToStdString() &&
                current->address == address.ToStdString() &&
                current->phone_number == phone.ToStdString()) {
                contactExists = true;
                break;
            }
            current = current->next;
        }

        if (contactExists) {
            int choice = wxMessageBox(
                "Contact already exists. Are you sure you want to add it again?",
                "Duplicate Contact", wxYES_NO | wxICON_QUESTION
            );
            if (choice != wxYES) {
                continue; // Restart the loop to prevent duplicate addition
            }
        }

        // Add the contact to the contact list
        contactList.addContact(
            firstName.ToStdString().c_str(),
            lastName.ToStdString().c_str(),
            email.ToStdString().c_str(),
            address.ToStdString().c_str(),
            phone.ToStdString().c_str()
        );

        // Add contact to the ListCtrl (UI)
        long index = contactListCtrl->InsertItem(contactListCtrl->GetItemCount(), firstName);
        contactListCtrl->SetItem(index, 1, lastName);
        contactListCtrl->SetItem(index, 2, email);
        contactListCtrl->SetItem(index, 3, address);
        contactListCtrl->SetItem(index, 4, phone);

        wxMessageBox("Contact added successfully!", "Success", wxOK | wxICON_INFORMATION);
        break; // Exit the loop since the contact was added successfully
    }
}

void MainFrame::OnToggleTheme(wxCommandEvent& event) {
    // Toggle theme state
    isDarkTheme = !isDarkTheme;

    // Set colors based on the theme state
    wxColour bgColor = isDarkTheme ? *wxBLACK : *wxWHITE;
    wxColour textColor = isDarkTheme ? *wxWHITE : *wxBLACK;

    // Update main panel background
    mainPanel->SetBackgroundColour(bgColor);
    mainPanel->SetForegroundColour(textColor);

    // Update ListCtrl colors
    contactListCtrl->SetBackgroundColour(bgColor);
    contactListCtrl->SetForegroundColour(textColor);

    // Refresh to apply changes
    mainPanel->Refresh();
    contactListCtrl->Refresh();

    wxMessageBox(isDarkTheme ? "Dark theme applied!" : "Light theme applied!", "Theme Change", wxOK | wxICON_INFORMATION);
}


void MainFrame::OnDeleteContact(wxCommandEvent& event) {
    // Get the selected item in the ListCtrl
    long selectedIndex = contactListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    
    if (selectedIndex == -1) {
        // No item selected
        wxMessageBox("Please select a contact to delete.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Retrieve the contact details from the selected row
    wxString firstName = contactListCtrl->GetItemText(selectedIndex, 0); // First Name
    wxString lastName = contactListCtrl->GetItemText(selectedIndex, 1); // Last Name

    // Confirm deletion
    int response = wxMessageBox(
        wxString::Format("Are you sure you want to delete the contact '%s %s'?", firstName, lastName),
        "Confirm Delete", wxYES_NO | wxICON_QUESTION);

    if (response == wxYES) {
        // Remove the contact from the ContactList
        bool success = contactList.removeContact(firstName.ToStdString().c_str(), lastName.ToStdString().c_str());
        
        if (success) {
            // Remove the item from the ListCtrl
            contactListCtrl->DeleteItem(selectedIndex);
            wxMessageBox("Contact deleted successfully!", "Success", wxOK | wxICON_INFORMATION);
        } else {
            wxMessageBox("Failed to delete the contact.", "Error", wxOK | wxICON_ERROR);
        }
    }
}


   

void MainFrame::OnEditContact(wxCommandEvent& event) {
    wxTextEntryDialog dialog(this, "Enter First Name and Last Name of Contact to Edit (comma-separated):", "Edit Contact");
    if (dialog.ShowModal() == wxID_OK) {
        wxString input = dialog.GetValue();
        wxStringTokenizer tokenizer(input, ",");
        if (tokenizer.CountTokens() == 2) {
            wxString firstName = tokenizer.GetNextToken().Trim();
            wxString lastName = tokenizer.GetNextToken().Trim();

            ContactList::Contact* current = contactList.getHead();
            bool found = false;

            while (current) {
                if (toLower(current->firstName) == toLower(firstName.ToStdString()) &&
                    toLower(current->lastName) == toLower(lastName.ToStdString())) {
                    found = true;
                    break;
                }
                current = current->next;
            }

            if (found) {
                wxString newFirstName = wxGetTextFromUser("Enter new first name (leave blank to keep current):", "Edit Contact", current->firstName);
                wxString newLastName = wxGetTextFromUser("Enter new last name (leave blank to keep current):", "Edit Contact", current->lastName);
                wxString newPhone = wxGetTextFromUser("Enter new phone number (leave blank to keep current):", "Edit Contact", current->phone_number);
                wxString newAddress = wxGetTextFromUser("Enter new address (leave blank to keep current):", "Edit Contact", current->address);
                wxString newEmail = wxGetTextFromUser("Enter new email (leave blank to keep current):", "Edit Contact", current->Email);

                if (!newFirstName.IsEmpty()) strncpy(current->firstName, newFirstName.ToStdString().c_str(), sizeof(current->firstName) - 1);
                if (!newLastName.IsEmpty()) strncpy(current->lastName, newLastName.ToStdString().c_str(), sizeof(current->lastName) - 1);
                if (!newPhone.IsEmpty()) strncpy(current->phone_number, newPhone.ToStdString().c_str(), sizeof(current->phone_number) - 1);
                if (!newAddress.IsEmpty()) strncpy(current->address, newAddress.ToStdString().c_str(), sizeof(current->address) - 1);
                if (!newEmail.IsEmpty()) strncpy(current->Email, newEmail.ToStdString().c_str(), sizeof(current->Email) - 1);

                wxMessageBox("Contact edited successfully!", "Success", wxOK | wxICON_INFORMATION);
            }
            else {
                wxMessageBox("Contact not found.", "Error", wxOK | wxICON_ERROR);
            }
        }
        else {
            wxMessageBox("Invalid input format. Use: FirstName, LastName.", "Error", wxOK | wxICON_ERROR);
        }
    }
}

void MainFrame::OnDisplayContacts(wxCommandEvent& event) {
    contactListCtrl->DeleteAllItems();

    ContactList::Contact* current = contactList.getHead();
    if (!current) {
        wxMessageBox("No contacts available to display.", "Contact List", wxOK | wxICON_INFORMATION);
        return;
    }
    while (current) {
        long index = contactListCtrl->InsertItem(contactListCtrl->GetItemCount(), current->firstName);
        contactListCtrl->SetItem(index, 1, current->lastName);
        contactListCtrl->SetItem(index, 2, current->Email);
        contactListCtrl->SetItem(index, 3, current->address);
        contactListCtrl->SetItem(index, 4, current->phone_number);
        current = current->next;
    }

    wxMessageBox("Contacts displayed successfully!", "Success", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSearchContact(wxCommandEvent& event) {
    wxArrayString options;
    options.Add("Search by First Name and Last Name");
    options.Add("Search by First Name Only");
    options.Add("Search by Last Name Only");
    options.Add("Exit");

    wxSingleChoiceDialog dialog(this, "Choose search option:", "Search Contact", options);
    if (dialog.ShowModal() == wxID_OK) {
        int option = dialog.GetSelection();

        wxString firstName;
        wxString lastName;

        contactListCtrl->DeleteAllItems(); // Clear previous results

        ContactList::Contact* current = contactList.getHead();
        bool found = false;

        switch (option) {
        case 0:
            firstName = wxGetTextFromUser("Enter First Name:");
            lastName = wxGetTextFromUser("Enter Last Name:");
            while (current) {
                if (toLower(current->firstName) == toLower(firstName.ToStdString()) &&
                    toLower(current->lastName) == toLower(lastName.ToStdString())) {
                    long index = contactListCtrl->InsertItem(contactListCtrl->GetItemCount(), current->firstName);
                    contactListCtrl->SetItem(index, 1, current->lastName);
                    contactListCtrl->SetItem(index, 2, current->Email);
                    contactListCtrl->SetItem(index, 3, current->address);
                    contactListCtrl->SetItem(index, 4, current->phone_number);
                    found = true;
                }
                current = current->next;
            }
            break;

        case 1:
            firstName = wxGetTextFromUser("Enter First Name:");
            while (current) {
                if (toLower(current->firstName) == toLower(firstName.ToStdString())) {
                    long index = contactListCtrl->InsertItem(contactListCtrl->GetItemCount(), current->firstName);
                    contactListCtrl->SetItem(index, 1, current->lastName);
                    contactListCtrl->SetItem(index, 2, current->Email);
                    contactListCtrl->SetItem(index, 3, current->address);
                    contactListCtrl->SetItem(index, 4, current->phone_number);
                    found = true;
                }
                current = current->next;
            }
            break;

        case 2:
            lastName = wxGetTextFromUser("Enter Last Name:");
            while (current) {
                if (toLower(current->lastName) == toLower(lastName.ToStdString())) {
                    long index = contactListCtrl->InsertItem(contactListCtrl->GetItemCount(), current->firstName);
                    contactListCtrl->SetItem(index, 1, current->lastName);
                    contactListCtrl->SetItem(index, 2, current->Email);
                    contactListCtrl->SetItem(index, 3, current->address);
                    contactListCtrl->SetItem(index, 4, current->phone_number);
                    found = true;
                }
                current = current->next;
            }
            break;

        case 3:
            return;

        default:
            wxMessageBox("Invalid choice.", "Error", wxOK | wxICON_ERROR);
        }

        if (!found) {
            wxMessageBox("No matching contacts found.", "Search Results", wxOK | wxICON_INFORMATION);
        }
    }
}

void MainFrame::OnClearContacts(wxCommandEvent& event) {
    int response = wxMessageBox("Are you sure you want to clear all contacts?", "Confirm", wxYES_NO | wxICON_QUESTION);
    if (response == wxYES) {
        contactList.clearContacts();
        contactListCtrl->DeleteAllItems();  // Clear the list view as well
        wxMessageBox("All contacts cleared!", "Success", wxOK | wxICON_INFORMATION);
    }
}
