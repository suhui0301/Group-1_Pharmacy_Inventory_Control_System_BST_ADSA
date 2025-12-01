#include <iostream>
#include <string>
#include <limits>

using namespace std;

struct Medicine {
    string id;
    string name;
    int quantity{};
    double price{};
};

struct Node {
    Medicine data;
    Node* left;
    Node* right;

    Node(const Medicine& m) : data(m), left(nullptr), right(nullptr) {}
};

class PharmacyBST {
private:
    Node* root;

    //Insert
    Node* insert(Node* node, const Medicine& med) {
        if (node == nullptr) {
            return new Node(med);
        }
        if (med.id < node->data.id) {
            node->left = insert(node->left, med);
        } else if (med.id > node->data.id) {
            node->right = insert(node->right, med);
        } else {
            //Update existing record
            cout << "  [Info] ID " << med.id << " exists. Updating record...\n";
            node->data.name = med.name;
            node->data.quantity = med.quantity;
            node->data.price = med.price;
        }
        return node;
    }

    //Search 
    Node* search(Node* node, const string& id) const {
        if (node == nullptr) return nullptr;
        if (id == node->data.id) return node;
        if (id < node->data.id) return search(node->left, id);
        return search(node->right, id);
    }

    //Find Minimum Node (for deletion)
    Node* findMin(Node* node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    //Delete 
    Node* remove(Node* node, const string& id) {
        if (node == nullptr) return nullptr;

        if (id < node->data.id) {
            node->left = remove(node->left, id);
        } else if (id > node->data.id) {
            node->right = remove(node->right, id);
        } else {
            // Case 1: Leaf Node
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            // Case 2: One Child
            else if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            // Case 3: Two Children
            else {
                Node* succ = findMin(node->right);
                node->data = succ->data;
                node->right = remove(node->right, succ->data.id);
            }
        }
        return node;
    }

    //In-order Traversal for Display
    void inorder(Node* node) const {
        if (!node) return;
        inorder(node->left);
        cout << "ID: " << node->data.id 
             << " | Name: " << node->data.name 
             << " | Qty: " << node->data.quantity 
             << " | Price: RM" << node->data.price << "\n";
        inorder(node->right);
    }

    //clear memory
    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    PharmacyBST() : root(nullptr) {}
    ~PharmacyBST() { destroy(root); }

    // --- Public Interface ---

    void addMedicine(const Medicine& med) {
        root = insert(root, med);
    }

    bool searchMedicine(const string& id, Medicine& outMed) const {
        Node* res = search(root, id);
        if (res) {
            outMed = res->data;
            return true;
        }
        return false;
    }

    bool updateMedicine(const string& id, int newQty, double newPrice) {
        Node* res = search(root, id);
        if (!res) return false;
        res->data.quantity = newQty;
        res->data.price = newPrice;
        return true;
    }

    bool deleteMedicine(const string& id) {
        if (!search(root, id)) return false; 
        root = remove(root, id);
        return true;
    }

    void displayAll() const {
        if (root == nullptr) {
            cout << "Stock is empty.\n";
        } else {
            cout << "\n--- Current Inventory (Sorted by ID) ---\n";
            inorder(root);
            cout << "----------------------------------------\n";
        }
    }
};
void clearBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

Medicine getMedicineInput() {
    Medicine med;
    cout << "Enter ID: ";
    getline(cin, med.id);
    cout << "Enter Name: ";
    getline(cin, med.name);
    
    cout << "Enter Quantity: ";
    while (!(cin >> med.quantity) || med.quantity < 0) {
        cout << "Invalid. Retry Quantity: ";
        clearBuffer();
    }
    
    cout << "Enter Price: ";
    while (!(cin >> med.price) || med.price < 0) {
        cout << "Invalid. Retry Price: ";
        clearBuffer();
    }
    clearBuffer(); 
    return med;
}

// --- Main Function ---

int main() {
    PharmacyBST pharmacy;
    int choice;

    // ==========================================
    //   PRE-LOADING DATA 
    // ==========================================
    cout << "Initializing System with Default Data...\n";
    pharmacy.addMedicine({"M011", "Ascorbic Acid 1000mg", 100, 15.00});
    pharmacy.addMedicine({"M102", "Amoxicillin 500mg",    50,  20.00});
    pharmacy.addMedicine({"M230", "Cough Syrup 100ml",    30,  12.50});
    pharmacy.addMedicine({"M014", "Insulin Pen U-100",    20,  55.00});
    cout << "Data Loaded Successfully!\n";
    // ==========================================

    do {
        cout << "\n=== Pharmacy System (Core Functions) ===\n";
        cout << "1. Add Medicine\n";
        cout << "2. Search Medicine\n";
        cout << "3. Update Medicine\n";
        cout << "4. Delete Medicine\n";
        cout << "5. Display All Stock\n";
        cout << "0. Exit\n";
        cout << "Select: ";

        while (!(cin >> choice)) {
            cout << "Invalid input. Number only: ";
            clearBuffer();
        }
        clearBuffer();

        switch (choice) {
            case 1: 
                pharmacy.addMedicine(getMedicineInput());
                break;
            case 2: {
                string id;
                cout << "Search ID: "; getline(cin, id);
                Medicine m;
                if (pharmacy.searchMedicine(id, m)) 
                    cout << "Found: " << m.name << " | Qty: " << m.quantity << " | RM" << m.price << "\n";
                else cout << "Not found.\n";
                break;
            }
            case 3: {
                string id;
                cout << "Update ID: "; getline(cin, id);
                Medicine m;
                if (pharmacy.searchMedicine(id, m)) {
                    cout << "Current: " << m.name << " (" << m.quantity << ", RM" << m.price << ")\n";
                    int q; double p;
                    cout << "New Qty: "; cin >> q;
                    cout << "New Price: "; cin >> p;
                    clearBuffer();
                    pharmacy.updateMedicine(id, q, p);
                    cout << "Updated.\n";
                } else cout << "ID not found.\n";
                break;
            }
            case 4: {
                string id;
                cout << "Delete ID: "; getline(cin, id);
                if (pharmacy.deleteMedicine(id)) cout << "Deleted.\n";
                else cout << "ID not found.\n";
                break;
            }
            case 5:
                pharmacy.displayAll();
                break;
            case 0:
                cout << "Goodbye.\n";
                break;
            default:
                cout << "Invalid option.\n";
        }
    } while (choice != 0);

    return 0;
}
