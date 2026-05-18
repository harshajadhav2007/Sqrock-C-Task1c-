#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

class Student {
public:
    int id;
    char name[50];
    float gpa;

    void getDetails() {
        cout << "Enter ID: "; cin >> id;
        cin.ignore(); // Clear buffer
        cout << "Enter Name: "; cin.getline(name, 50);
        cout << "Enter GPA: "; cin >> gpa;
    }

    void displayHeader() {
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "GPA" << endl;
        cout << "------------------------------------------" << endl;
    }

    void displayRecord() {
        cout << left << setw(10) << id << setw(20) << name << setw(10) << gpa << endl;
    }
};
void addStudent() {
    Student s;
    s.getDetails();
    
    // ios::app ensures we append to the file rather than overwriting
    ofstream outFile("students.dat", ios::binary | ios::app);
    if (outFile) {
        outFile.write(reinterpret_cast<char*>(&s), sizeof(s));
        outFile.close();
        cout << "\nRecord saved successfully!" << endl;
    }
}

void viewAll() {
    Student s;
    ifstream inFile("students.dat", ios::binary);
    
    if (!inFile) {
        cout << "No records found (file does not exist)." << endl;
        return;
    }

    s.displayHeader();
    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        s.displayRecord();
    }
    inFile.close();
}

void searchStudent() {
    int searchId;
    bool found = false;
    Student s;
    
    cout << "Enter ID to search: "; cin >> searchId;
    ifstream inFile("students.dat", ios::binary);

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.id == searchId) {
            s.displayHeader();
            s.displayRecord();
            found = true;
            break;
        }
    }
    if (!found) cout << "Record not found." << endl;
    inFile.close();
}

void updateStudent() {
    int updateId;
    bool found = false;
    Student s;
    
    fstream file("students.dat", ios::binary | ios::in | ios::out);
    cout << "Enter ID to update: "; cin >> updateId;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.id == updateId) {
            cout << "Enter New Details:\n";
            s.getDetails();
            
            // Move the put pointer back to the start of this specific record
            int pos = -1 * static_cast<int>(sizeof(s));
            file.seekp(pos, ios::cur);
            
            file.write(reinterpret_cast<char*>(&s), sizeof(s));
            found = true;
            break;
        }
    }
    if (found) cout << "Record updated!" << endl;
    else cout << "Record not found." << endl;
    file.close();
}

void deleteStudent() {
    int deleteId;
    Student s;
    bool found = false;
    
    cout << "Enter ID to delete: "; cin >> deleteId;
    
    ifstream inFile("students.dat", ios::binary);
    ofstream tempFile("temp.dat", ios::binary); // Temporary file to store kept records

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.id != deleteId) {
            tempFile.write(reinterpret_cast<char*>(&s), sizeof(s));
        } else {
            found = true;
        }
    }
    
    inFile.close();
    tempFile.close();
    
    remove("students.dat");        // Delete old file
    rename("temp.dat", "students.dat"); // Rename temp to original
    
    if (found) cout << "Record deleted successfully." << endl;
    else cout << "Record not found." << endl;
}
int main() {
    int choice;
    
    while (true) {
        cout << "\n--- Student Management System ---" << endl;
        cout << "1. Add Student\n2. View All\n3. Search\n4. Update\n5. Delete\n6. Exit" << endl;
        cout << "Select option: "; cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewAll(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: exit(0);
            default: cout << "Invalid choice!" << endl;
        }
    }
    return 0;
}
