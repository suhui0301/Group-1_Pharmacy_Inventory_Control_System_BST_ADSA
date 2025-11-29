#include <iostream>
#include <iomanip>
#include <limits> 
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono; 

struct Medicine {
    string batchID;     
    string medicineName;
    int quantity;        
    string expiryDate;   
    string location;   
};

struct Node {
    Medicine data;
    Node *left;   
    Node *right;  

    Node(Medicine med) {
        data = med;
        left = nullptr;
        right = nullptr;
    }
};

class MedicineManager {  
private:
    Node *root;

    Node *insertInternal(Node *current, Medicine med) {  
        if (current == nullptr) {
            return new Node(med);
        }
        if (med.batchID < current->data.batchID) {
            current->left = insertInternal(current->left, med);
        } else if (med.batchID > current->data.batchID) {
            current->right = insertInternal(current->right, med);
        } else {
            cout << "Error: Medicine with Batch ID " << med.batchID
                 << " already exists.🚫" << endl;
        }
        return current;
    }

    Node *searchInternal(Node *current, string batchID) {
        if (current == nullptr || current->data.batchID == batchID) {
            return current;
        }
        if (batchID < current->data.batchID) {
            return searchInternal(current->left, batchID);
        } else {
            return searchInternal(current->right, batchID);
        }
    }

    void displayInOrderInternal(Node *current) {
        if (current != nullptr) {
            displayInOrderInternal(current->left);
            cout << left << setw(15) << current->data.batchID << setw(25)
                 << current->data.medicineName << setw(10) << current->data.quantity
                 << setw(15) << current->data.expiryDate << setw(15)
                 << current->data.location << endl;
            displayInOrderInternal(current->right);
        }
    }

    Node *findMinNode(Node *node) {
        Node *current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    Node *removeInternal(Node *current, string batchID) {
        if (current == nullptr)
            return current;
        if (batchID < current->data.batchID) {
            current->left = removeInternal(current->left, batchID);
        } else if (batchID > current->data.batchID) {
            current->right = removeInternal(current->right, batchID);
        } else {
            if (current->left == nullptr) {
                Node *temp = current->right;
                delete current;
                return temp;
            } else if (current->right == nullptr) {
                Node *temp = current->left;
                delete current;
                return temp;
            }
            Node *temp = findMinNode(current->right);
            current->data = temp->data;
            current->right = removeInternal(current->right, temp->data.batchID);
        }
        return current;
    }

public:
    MedicineManager() { root = nullptr; }
    
    void addMedicine(Medicine med) { 
        root = insertInternal(root, med); 
    }
    
    Medicine *findMedicine(string batchID) {
        Node *result = searchInternal(root, batchID);
        if (result != nullptr) {
            return &(result->data);
        }
        return nullptr;
    }

    void showAllMedicines() {
        if (root == nullptr) {
            cout << "No medicines in the system.⚠️" << endl;
            return;
        }
        cout << left << setw(15) << "Batch ID" << setw(25) << "Name" << setw(10)
             << "Quantity" << setw(15) << "Expiry" << setw(15) << "Location"
             << endl;
        cout << string(80, '-') << endl;
        displayInOrderInternal(root);
    }

    void removeMedicine(string batchID) {
        root = removeInternal(root, batchID);
    }
};

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidInt() {
    int value;
    while (!(cin >> value)) {
        cout << "Invalid input. Please enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(); 
    return value;
}

int main() {
    MedicineManager system;
    int choice;    
    system.addMedicine({"B303", "Ascorbic Acid 1000mg", 100, "03-03-2027", "Shelf A01"});  
    system.addMedicine({"B101", "Insulin Pen U-100",    20,  "01-01-2026", "Fridge FR01"});   
    system.addMedicine({"B505", "Paracetamol 500mg",    500, "05-05-2026", "Shelf P02"});

    do {
        cout << "\nWelcome to the Medicine Management System!💊" << endl;
        cout << "\n=== Medicine Management System (BST) ===" << endl;
        cout << "1. Add New Medicine Batch➕" << endl;
        cout << "2. Search by Batch ID🔎" << endl;
        cout << "3. Display All Batches💻" << endl;
        cout << "4. Delete Batch➖" << endl;
        cout << "5. Exit👋" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid choice. Please enter a number.⚠️" << endl;
            clearInputBuffer();
            continue;
        }

        cin.ignore(); 

        if (choice == 1) {
            Medicine med;
            cout << "\n--- ➕Add Medicine Batch➕ ---" << endl;

            cout << "Enter Batch ID (e.g., B202): ";
            getline(cin, med.batchID);
            cout << "Enter Medicine Name: ";
            getline(cin, med.medicineName);
            cout << "Enter Quantity: ";
            med.quantity = getValidInt();
            cout << "Enter Expiry Date (DD-MM-YYYY): ";
            getline(cin, med.expiryDate);
            cout << "Enter Location (e.g., Shelf A01): ";
            getline(cin, med.location);

            auto start = high_resolution_clock::now();
            system.addMedicine(med);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);

            cout << "Batch added successfully!✅" << endl;
            cout << "Execution Time: " << duration.count() << " nanoseconds" << endl;

        } else if (choice == 2) {
            string id;
            cout << "\n--- 🔎Search Medicine🔎 ---" << endl;
            cout << "Enter Batch ID to search: ";
            getline(cin, id);

            auto start = high_resolution_clock::now();
            Medicine *foundMed = system.findMedicine(id);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);

            if (foundMed != nullptr) {
                cout << "\n[FOUND] Batch Details:" << endl;
                cout << "Batch ID: " << foundMed->batchID << endl;
                cout << "Name:     " << foundMed->medicineName << endl;
                cout << "Quantity: " << foundMed->quantity << endl;
                cout << "Expiry:   " << foundMed->expiryDate << endl;
                cout << "Location: " << foundMed->location << endl;
            } else {
                cout << "\n[NOT FOUND] Batch " << id << " does not exist.❌" << endl;
            }
            cout << "Execution Time: " << duration.count() << " nanoseconds" << endl;

        } else if (choice == 3) {
            cout << "\n--- 💊All Medicine Batches💊 ---" << endl;
            
            auto start = high_resolution_clock::now();
            system.showAllMedicines();
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);
            
            cout << "Execution Time: " << duration.count() << " nanoseconds" << endl;

        } else if (choice == 4) {
            string id;
            cout << "\n--- ➖Delete Batch➖ ---" << endl;
            cout << "Enter Batch ID to delete: ";
            getline(cin, id);
            
            auto start = high_resolution_clock::now();
            system.removeMedicine(id);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);
            
            cout << "Delete operation completed.✅" << endl;
            cout << "Execution Time: " << duration.count() << " nanoseconds" << endl;

        } else if (choice == 5) {
            cout << "Exiting... Goodbye!👋" << endl;
        } else {
            cout << "Invalid choice.❌" << endl;
        }

    } while (choice != 5);

    return 0;
}