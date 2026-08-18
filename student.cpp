/*
    ==========================================================
    STUDENT MANAGEMENT SYSTEM
    ==========================================================
    Language     : C++
    Data Structure : Singly Linked List
    Features     : Add, Search, Update, Delete, Display,
                   Sort by Roll No, File Persistence (save/load)
    Author       : BCA Final Year Mini Project
    ==========================================================
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <limits>

using namespace std;

const string DATA_FILE = "students.dat";

// ---------------------------------------------------------
// Node structure: one node = one student record
// ---------------------------------------------------------
struct Student {
    int rollNo;
    string name;
    string course;
    int semester;
    float cgpa;
    string contact;
    Student* next;
};

// ---------------------------------------------------------
// Linked List based Student Database
// ---------------------------------------------------------
class StudentDatabase {
private:
    Student* head;
    int count;

public:
    StudentDatabase() : head(nullptr), count(0) {}

    ~StudentDatabase() {
        clearAll();
    }

    bool isEmpty() const { return head == nullptr; }
    int getCount() const { return count; }

    // ---------------- CREATE ----------------
    bool addStudent(int rollNo, const string& name, const string& course,
                     int semester, float cgpa, const string& contact) {
        if (findStudent(rollNo) != nullptr) {
            return false; // roll number already exists
        }

        Student* newNode = new Student{rollNo, name, course, semester, cgpa, contact, nullptr};

        // Insert in ascending order of roll number (keeps list sorted)
        if (head == nullptr || head->rollNo > rollNo) {
            newNode->next = head;
            head = newNode;
        } else {
            Student* temp = head;
            while (temp->next != nullptr && temp->next->rollNo < rollNo) {
                temp = temp->next;
            }
            newNode->next = temp->next;
            temp->next = newNode;
        }
        count++;
        return true;
    }

    // ---------------- READ (search) ----------------
    Student* findStudent(int rollNo) const {
        Student* temp = head;
        while (temp != nullptr) {
            if (temp->rollNo == rollNo) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    // ---------------- UPDATE ----------------
    bool updateStudent(int rollNo, const string& name, const string& course,
                        int semester, float cgpa, const string& contact) {
        Student* s = findStudent(rollNo);
        if (s == nullptr) return false;
        s->name = name;
        s->course = course;
        s->semester = semester;
        s->cgpa = cgpa;
        s->contact = contact;
        return true;
    }

    // ---------------- DELETE ----------------
    bool deleteStudent(int rollNo) {
        Student* temp = head;
        Student* prev = nullptr;

        while (temp != nullptr && temp->rollNo != rollNo) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) return false; // not found

        if (prev == nullptr) {
            head = temp->next; // deleting head node
        } else {
            prev->next = temp->next;
        }

        delete temp;
        count--;
        return true;
    }

    // ---------------- DISPLAY ALL ----------------
    void displayAll() const {
        if (isEmpty()) {
            cout << "\nNo student records found.\n";
            return;
        }

        cout << "\n" << string(90, '=') << "\n";
        cout << left << setw(8) << "Roll"
             << setw(20) << "Name"
             << setw(15) << "Course"
             << setw(6)  << "Sem"
             << setw(8)  << "CGPA"
             << setw(15) << "Contact" << "\n";
        cout << string(90, '-') << "\n";

        Student* temp = head;
        while (temp != nullptr) {
            cout << left << setw(8) << temp->rollNo
                 << setw(20) << temp->name
                 << setw(15) << temp->course
                 << setw(6)  << temp->semester
                 << setw(8)  << fixed << setprecision(2) << temp->cgpa
                 << setw(15) << temp->contact << "\n";
            temp = temp->next;
        }
        cout << string(90, '=') << "\n";
        cout << "Total Students: " << count << "\n";
    }

    void clearAll() {
        Student* temp = head;
        while (temp != nullptr) {
            Student* next = temp->next;
            delete temp;
            temp = next;
        }
        head = nullptr;
        count = 0;
    }

    // ---------------- FILE PERSISTENCE ----------------
    void saveToFile() const {
        ofstream fout(DATA_FILE);
        if (!fout) {
            cout << "Error: could not open file for saving.\n";
            return;
        }
        Student* temp = head;
        while (temp != nullptr) {
            fout << temp->rollNo << "|" << temp->name << "|" << temp->course << "|"
                 << temp->semester << "|" << temp->cgpa << "|" << temp->contact << "\n";
            temp = temp->next;
        }
        fout.close();
    }

    void loadFromFile() {
        ifstream fin(DATA_FILE);
        if (!fin) return; // no existing file yet, that's fine

        clearAll();
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string token;
            int rollNo, semester;
            string name, course, contact;
            float cgpa;

            getline(ss, token, '|'); rollNo = stoi(token);
            getline(ss, name, '|');
            getline(ss, course, '|');
            getline(ss, token, '|'); semester = stoi(token);
            getline(ss, token, '|'); cgpa = stof(token);
            getline(ss, contact, '|');

            addStudent(rollNo, name, course, semester, cgpa, contact);
        }
        fin.close();
    }
};

// ---------------------------------------------------------
// Helper input functions (basic validation)
// ---------------------------------------------------------
int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

float readFloat(const string& prompt) {
    float value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Please enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

// ---------------------------------------------------------
// Menu-driven operations
// ---------------------------------------------------------
void addStudentMenu(StudentDatabase& db) {
    cout << "\n--- Add New Student ---\n";
    int rollNo = readInt("Enter Roll No: ");
    string name = readLine("Enter Name: ");
    string course = readLine("Enter Course: ");
    int semester = readInt("Enter Semester: ");
    float cgpa = readFloat("Enter CGPA: ");
    string contact = readLine("Enter Contact No: ");

    if (db.addStudent(rollNo, name, course, semester, cgpa, contact)) {
        db.saveToFile();
        cout << "\nStudent added successfully!\n";
    } else {
        cout << "\nError: Roll No " << rollNo << " already exists.\n";
    }
}

void searchStudentMenu(StudentDatabase& db) {
    cout << "\n--- Search Student ---\n";
    int rollNo = readInt("Enter Roll No to search: ");
    Student* s = db.findStudent(rollNo);

    if (s == nullptr) {
        cout << "\nStudent with Roll No " << rollNo << " not found.\n";
        return;
    }

    cout << "\n--- Student Found ---\n";
    cout << "Roll No  : " << s->rollNo << "\n";
    cout << "Name     : " << s->name << "\n";
    cout << "Course   : " << s->course << "\n";
    cout << "Semester : " << s->semester << "\n";
    cout << "CGPA     : " << fixed << setprecision(2) << s->cgpa << "\n";
    cout << "Contact  : " << s->contact << "\n";
}

void updateStudentMenu(StudentDatabase& db) {
    cout << "\n--- Update Student ---\n";
    int rollNo = readInt("Enter Roll No to update: ");

    Student* s = db.findStudent(rollNo);
    if (s == nullptr) {
        cout << "\nStudent with Roll No " << rollNo << " not found.\n";
        return;
    }

    cout << "Current details -> Name: " << s->name << ", Course: " << s->course
         << ", Semester: " << s->semester << ", CGPA: " << s->cgpa
         << ", Contact: " << s->contact << "\n";
    cout << "Enter new details:\n";

    string name = readLine("Enter Name: ");
    string course = readLine("Enter Course: ");
    int semester = readInt("Enter Semester: ");
    float cgpa = readFloat("Enter CGPA: ");
    string contact = readLine("Enter Contact No: ");

    db.updateStudent(rollNo, name, course, semester, cgpa, contact);
    db.saveToFile();
    cout << "\nStudent record updated successfully!\n";
}

void deleteStudentMenu(StudentDatabase& db) {
    cout << "\n--- Delete Student ---\n";
    int rollNo = readInt("Enter Roll No to delete: ");

    char confirm;
    cout << "Are you sure you want to delete Roll No " << rollNo << "? (y/n): ";
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (confirm != 'y' && confirm != 'Y') {
        cout << "Delete cancelled.\n";
        return;
    }

    if (db.deleteStudent(rollNo)) {
        db.saveToFile();
        cout << "\nStudent deleted successfully!\n";
    } else {
        cout << "\nStudent with Roll No " << rollNo << " not found.\n";
    }
}

void printMenu() {
    cout << "\n============================================\n";
    cout << "     STUDENT MANAGEMENT SYSTEM\n";
    cout << "============================================\n";
    cout << "1. Add Student\n";
    cout << "2. Display All Students\n";
    cout << "3. Search Student (by Roll No)\n";
    cout << "4. Update Student\n";
    cout << "5. Delete Student\n";
    cout << "6. Exit\n";
    cout << "============================================\n";
}

int main() {
    StudentDatabase db;
    db.loadFromFile();

    int choice;
    do {
        printMenu();
        choice = readInt("Enter your choice (1-6): ");

        switch (choice) {
            case 1: addStudentMenu(db); break;
            case 2: db.displayAll(); break;
            case 3: searchStudentMenu(db); break;
            case 4: updateStudentMenu(db); break;
            case 5: deleteStudentMenu(db); break;
            case 6:
                db.saveToFile();
                cout << "\nData saved. Exiting program. Goodbye!\n";
                break;
            default:
                cout << "\nInvalid choice! Please enter a number between 1 and 6.\n";
        }
    } while (choice != 6);

    return 0;
}
