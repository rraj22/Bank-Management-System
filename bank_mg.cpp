#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
using namespace std;

// Forward declaration of AVLTree to be used in account class
class AVLTree;

class account {
    int acno;       // Account number
    char name[50];  // Account holder name
    int deposit;    // Account balance
    char type;      // Account type (C/S)
public:
    void create_account(AVLTree& accountTree); // Function to create a new account
    void show_account() const;  // Function to display account details
    void modify();              // Function to modify account details
    void dep(int);              // Function to deposit money
    void draw(int);             // Function to withdraw money
    void report() const;        // Function to display account report
    int retacno() const;        // Function to return account number
    int retdeposit() const;     // Function to return account balance
    char rettype() const;       // Function to return account type
    // Overloaded operators for file I/O
    friend ofstream& operator<<(ofstream& ofs, const account& ac);
    friend ifstream& operator>>(ifstream& ifs, account& ac);
};

// Function to create a new account
void account::create_account(AVLTree& accountTree) {
    system("clear");
    cout << "\n\t\t\t=============================";
    bool accountExists = false;
    do {
        cout << "\n\t\t\tEnter the Account No. : ";
        cin >> acno;
        if (accountTree.searchAccount(acno) != nullptr) {
            cout << "\n\t\t\tAccount number already exists. Please enter a different Account No.";
            accountExists = true;
        } else {
            accountExists = false;
        }
    } while (accountExists);
    cout << "\n\t\t\tEnter the Name of the Account holder : ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\n\t\t\tEnter Type of the Account (C/S) : ";
    cin >> type;
    type = toupper(type);
    cout << "\n\t\t\tEnter The Initial amount : ";
    cin >> deposit;
    cout << "\n\t\t\tAccount Created Successfully!";
    cout << "\n\t\t\t=============================";
}

// Function to display account details
void account::show_account() const {
    cout << "\n\t\t\t=============================";
    cout << "\n\t\t\tAccount No. : " << acno;
    cout << "\n\t\t\tAccount Holder Name : " << name;
    cout << "\n\t\t\tType of Account : " << type;
    cout << "\n\t\t\tBalance amount : " << deposit;
    cout << "\n\t\t\t=============================";
}

// Function to modify account details
void account::modify() {
    cout << "\n\t\t\t=============================";
    cout << "\n\t\t\tAccount No. : " << acno;
    cout << "\n\t\t\tModify Account Holder Name : ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\n\t\t\tModify Type of Account : ";
    cin >> type;
    type = toupper(type);
    cout << "\n\t\t\tModify Balance amount : ";
    cin >> deposit;
    cout << "\n\t\t\tAccount Modified Successfully!";
    cout << "\n\t\t\t=============================";
}

// Function to deposit money into the account
void account::dep(int x) {
    deposit += x;
}

// Function to withdraw money from the account
void account::draw(int x) {
    deposit -= x;
}

// Function to display account report
void account::report() const {
    cout << acno << setw(10) << " " << name << setw(10) << " " << type << setw(6) << deposit << endl;
}

// Function to return account number
int account::retacno() const {
    return acno;
}

// Function to return account balance
int account::retdeposit() const {
    return deposit;
}

// Function to return account type
char account::rettype() const {
    return type;
}

// Overloaded operator to write account details to a file
ofstream& operator<<(ofstream& ofs, const account& ac) {
    ofs << ac.acno << endl;
    ofs << ac.name << endl;
    ofs << ac.type << endl;
    ofs << ac.deposit << endl;
    return ofs;
}

// Overloaded operator to read account details from a file
ifstream& operator>>(ifstream& ifs, account& ac) {
    ifs >> ac.acno;
    ifs.ignore();
    ifs.getline(ac.name, 50);
    ifs >> ac.type;
    ifs >> ac.deposit;
    return ifs;
}

// AVL Tree Node structure
struct Node {
    account acc;  // Account object
    Node* left;   // Left child
    Node* right;  // Right child
    int height;   // Height of the node
};

// Function to get the height of the tree
int height(Node* N) {
    return (N == nullptr) ? 0 : N->height;
}

// Function to get maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Helper function to create a new AVL tree node
Node* newNode(account key) {
    Node* node = new Node();
    node->acc = key;
    node->left = node->right = nullptr;
    node->height = 1; // new node is initially added at leaf
    return node;
}

// Right rotate subtree rooted with y
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

// Left rotate subtree rooted with x
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}

// Get balance factor of node N
int getBalance(Node* N) {
    return (N == nullptr) ? 0 : height(N->left) - height(N->right);
}

// Recursive function to insert a key in the subtree rooted with node and returns the new root of the subtree
Node* insert(Node* node, account key) {
    // Perform the normal BST insertion
    if (node == nullptr)
        return newNode(key);

    if (key.retacno() < node->acc.retacno())
        node->left = insert(node->left, key);
    else if (key.retacno() > node->acc.retacno())
        node->right = insert(node->right, key);
    else // Equal keys are not allowed in BST
        return node;

    // Update height of this ancestor node
    node->height = 1 + max(height(node->left), height(node->right));

    // Get the balance factor of this ancestor node to check whether this node became unbalanced
    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && key.retacno() < node->left->acc.retacno())
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key.retacno() > node->right->acc.retacno())
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key.retacno() > node->left->acc.retacno()) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key.retacno() < node->right->acc.retacno()) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the (unchanged) node pointer
    return node;
}

// Function to get the node with minimum value found in that tree
Node* minValueNode(Node* node) {
    Node* current = node;

    // Loop down to find the leftmost leaf
    while (current->left != nullptr)
        current = current->left;

    return current;
}

// Recursive function to delete a node with given key from subtree with given root. It returns root of the modified subtree.
Node* deleteNode(Node* root, int key) {
    // Perform standard BST delete
    if (root == nullptr)
        return root;

    // If the key to be deleted is smaller than the root's key, then it lies in left subtree
    if (key < root->acc.retacno())
        root->left = deleteNode(root->left, key);

    // If the key to be deleted is greater than the root's key, then it lies in right subtree
    else if (key > root->acc.retacno())
        root->right = deleteNode(root->right, key);

    // If key is same as root's key, then this is the node to be deleted
    else {
        // Node with only one child or no child
        if ((root->left == nullptr) || (root->right == nullptr)) {
            Node* temp = root->left ? root->left : root->right;

            // No child case
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else // One child case
                *root = *temp; // Copy the contents of the non-empty child
            delete temp;
        } else {
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            Node* temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->acc = temp->acc;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->acc.retacno());
        }
    }

    // If the tree had only one node then return
    if (root == nullptr)
        return root;

    // Update height of the current node
    root->height = max(height(root->left), height(root->right)) + 1;

    // Get the balance factor of this node (to check whether this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// A utility function to print preorder traversal of the tree
void preOrder(Node* root) {
    if (root != nullptr) {
        root->acc.show_account();
        preOrder(root->left);
        preOrder(root->right);
    }
}

// Search function to find a node with given key
Node* search(Node* root, int key) {
    // Base Cases: root is null or key is present at root
    if (root == nullptr || root->acc.retacno() == key)
        return root;

    // Key is greater than root's key
    if (root->acc.retacno() < key)
        return search(root->right, key);

    // Key is smaller than root's key
    return search(root->left, key);
}

// AVL Tree class to encapsulate the AVL tree operations
class AVLTree {
private:
    Node* root;
public:
    AVLTree() : root(nullptr) {}

    // Function to add a new account
    void addAccount(account newAcc) {
        root = insert(root, newAcc);
        saveToFile();
    }

    // Function to display all accounts
    void displayAll() {
        preOrder(root);
    }

    // Function to search for an account
    account* searchAccount(int acno) {
        Node* result = search(root, acno);
        return (result != nullptr) ? &(result->acc) : nullptr;
    }

    // Function to delete an account
    void deleteAccount(int acno) {
        root = deleteNode(root, acno);
        saveToFile();
    }

    // Function to load accounts from file
    void loadFromFile() {
        ifstream inFile("account.dat");
        if (!inFile) {
            cout << "File could not be opened! Press any key...";
            return;
        }

        account ac;
        while (inFile >> ac) {
            root = insert(root, ac);
        }
        inFile.close();
    }

    // Function to save accounts to file
    void saveToFile() {
        ofstream outFile("account.dat", ios::trunc);
        saveToFileHelper(outFile, root);
        outFile.close();
    }

    // Helper function to recursively save accounts to file
    void saveToFileHelper(ofstream& outFile, Node* node) {
        if (node != nullptr) {
            outFile << node->acc;
            saveToFileHelper(outFile, node->left);
            saveToFileHelper(outFile, node->right);
        }
    }
};

// Function to handle deposit and withdrawal operations
void deposit_withdraw(AVLTree& accountTree, int n, int option) {
    int amt;
    account* ac = accountTree.searchAccount(n);
    if (ac == nullptr) {
        cout << "\n\n\t\t\tAccount number does not exist";
        return;
    }

    ac->show_account();
    if (option == 1) {
        cout << "\n\n\t\t\tTO DEPOSIT AMOUNT";
        cout << "\n\n\t\t\tEnter the amount to be deposited: ";
        cin >> amt;
        ac->dep(amt);
    } else if (option == 2) {
        cout << "\n\n\t\t\tTO WITHDRAW AMOUNT";
        cout << "\n\n\t\t\tEnter the amount to withdraw: ";
        cin >> amt;
        int bal = ac->retdeposit() - amt;
        if (bal < 0)
            cout << "Insufficient balance";
        else
            ac->draw(amt);
    }
    cout << "\n\n\t\t\tRecord Updated";
    accountTree.saveToFile();
}

// Main function
int main() {
    AVLTree accountTree;
    accountTree.loadFromFile();  // Load accounts from file on startup
    char ch;
    int num;
    account ac;

    do {
        system("clear");
        cout << "\n\n\t\t\t\t======================\n";
        cout << "\t\t\t\tBANK MANAGEMENT SYSTEM";
        cout << "\n\t\t\t\t======================\n";

        cout << "\t\t\t\t    ::MAIN MENU::\n";
        cout << "\n\t\t\t\t1. NEW ACCOUNT";
        cout << "\n\t\t\t\t2. DEPOSIT AMOUNT";
        cout << "\n\t\t\t\t3. WITHDRAW AMOUNT";
        cout << "\n\t\t\t\t4. BALANCE ENQUIRY";
        cout << "\n\t\t\t\t5. ALL ACCOUNT HOLDER LIST";
        cout << "\n\t\t\t\t6. CLOSE AN ACCOUNT";
        cout << "\n\t\t\t\t7. MODIFY AN ACCOUNT";
        cout << "\n\t\t\t\t8. EXIT";
        cout << "\n\n\t\t\t\tSelect Your Option (1-8): ";
        cin >> ch;

        switch (ch) {
        case '1':
            ac.create_account(accountTree);  // Pass the accountTree to create_account method
            accountTree.addAccount(ac);
            break;
        case '2':
            system("clear");
            cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
            deposit_withdraw(accountTree, num, 1);
            break;
        case '3':
            system("clear");
            cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
            deposit_withdraw(accountTree, num, 2);
            break;
        case '4':
            system("clear");
            cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
            ac = *(accountTree.searchAccount(num));
            if (&ac != nullptr) ac.show_account();
            else cout << "\n\n\t\t\tAccount number does not exist";
            break;
        case '5':
            accountTree.displayAll();
            break;
        case '6':
            system("clear");
            cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
            accountTree.deleteAccount(num);
            cout << "\n\nRecord Deleted ..";
            break;
        case '7':
            system("clear");
            cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
            ac = *(accountTree.searchAccount(num));
            if (&ac != nullptr) {
                cout << "\n\n\t\t\tEnter The New Details of account" << endl;
                ac.modify();
                cout << "\n\n\t\t\tRecord Updated";
                accountTree.saveToFile();
            } else {
                cout << "\n\n\t\t\tRecord Not Found ";
            }
            break;
        case '8':
            system("clear");
            cout << "\n\n\t\t\tThank you for using the Bank Management System!";
            break;
        default:
            cout << "\a";
        }
        cin.ignore();
        cin.get();
    } while (ch != '8');
    return 0;
}
