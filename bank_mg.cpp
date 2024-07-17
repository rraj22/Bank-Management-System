#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
using namespace std;

// Forward declaration of AVLTree class
class AVLTree;

// Account class
class account {
    int acno;
    char name[50];
    int deposit;
    char type;

public:
    void create_account(AVLTree&);
    void show_account() const;
    void modify();
    void dep(int);
    void draw(int);
    void report() const;
    int retacno() const;
    int retdeposit() const;
    char rettype() const;
};

// AVLTree class
class AVLTree {
private:
    struct Node {
        account acc;
        Node* left;
        Node* right;
        int height;
    };

    Node* root;

    // Private methods for AVL tree operations
    Node* insert(Node*, account);
    Node* deleteNode(Node*, int);
    Node* minValueNode(Node*);
    int height(Node*);
    int getBalance(Node*);
    Node* rightRotate(Node*);
    Node* leftRotate(Node*);
    void preOrder(Node*);
    Node* search(Node*, int);
    void saveToFileHelper(ofstream&, Node*);

public:
    AVLTree();
    void addAccount(account);
    void displayAll();
    account* searchAccount(int);
    void deleteAccount(int);
    void loadFromFile();
    void saveToFile();
};

// AVLTree methods implementation

// Constructor
AVLTree::AVLTree() {
    root = nullptr;
}

// Insert account into AVL tree
AVLTree::Node* AVLTree::insert(Node* node, account acc) {
    if (node == nullptr) {
        Node* newNode = new Node;
        newNode->acc = acc;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }

    if (acc.retacno() < node->acc.retacno()) {
        node->left = insert(node->left, acc);
    } else if (acc.retacno() > node->acc.retacno()) {
        node->right = insert(node->right, acc);
    } else {
        // Duplicate account number handling (not allowed)
        cout << "Account with this number already exists." << endl;
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && acc.retacno() < node->left->acc.retacno())
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && acc.retacno() > node->right->acc.retacno())
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && acc.retacno() > node->left->acc.retacno()) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && acc.retacno() < node->right->acc.retacno()) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Delete account from AVL tree
AVLTree::Node* AVLTree::deleteNode(Node* root, int key) {
    if (root == nullptr)
        return root;

    if (key < root->acc.retacno())
        root->left = deleteNode(root->left, key);
    else if (key > root->acc.retacno())
        root->right = deleteNode(root->right, key);
    else {
        if ((root->left == nullptr) || (root->right == nullptr)) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else
                *root = *temp;
            delete temp;
        } else {
            Node* temp = minValueNode(root->right);

            root->acc = temp->acc;

            root->right = deleteNode(root->right, temp->acc.retacno());
        }
    }

    if (root == nullptr)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Get minimum value node in AVL tree
AVLTree::Node* AVLTree::minValueNode(Node* node) {
    Node* current = node;

    while (current->left != nullptr)
        current = current->left;

    return current;
}

// Get height of AVL tree
int AVLTree::height(Node* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// Get balance factor of AVL tree
int AVLTree::getBalance(Node* node) {
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

// Right rotate AVL tree
AVLTree::Node* AVLTree::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Left rotate AVL tree
AVLTree::Node* AVLTree::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Preorder traversal of AVL tree
void AVLTree::preOrder(Node* root) {
    if (root != nullptr) {
        root->acc.show_account();
        preOrder(root->left);
        preOrder(root->right);
    }
}

// Search for account in AVL tree
AVLTree::Node* AVLTree::search(Node* root, int key) {
    if (root == nullptr || root->acc.retacno() == key)
        return root;

    if (root->acc.retacno() < key)
        return search(root->right, key);

    return search(root->left, key);
}

// Helper method to save AVL tree to file
void AVLTree::saveToFileHelper(ofstream& outFile, Node* node) {
    if (node != nullptr) {
        saveToFileHelper(outFile, node->left);
        outFile.write(reinterpret_cast<char*>(&node->acc), sizeof(account));
        saveToFileHelper(outFile, node->right);
    }
}

// Add account to AVL tree
void AVLTree::addAccount(account acc) {
    root = insert(root, acc);
}

// Display all accounts in AVL tree
void AVLTree::displayAll() {
    preOrder(root);
}

// Search for account in AVL tree
account* AVLTree::searchAccount(int key) {
    Node* resultNode = search(root, key);
    if (resultNode != nullptr)
        return &(resultNode->acc);
    else
        return nullptr;
}

// Delete account from AVL tree
void AVLTree::deleteAccount(int key) {
    root = deleteNode(root, key);
}

// Load accounts from file to AVL tree
void AVLTree::loadFromFile() {
    ifstream inFile("account.dat", ios::binary);
    if (!inFile) {
        cout << "Error opening file. No accounts loaded." << endl;
        return;
    }

    account acc;
    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(account))) {
        addAccount(acc);
    }

    inFile.close();
}

// Save accounts from AVL tree to file
void AVLTree::saveToFile() {
    ofstream outFile("account.dat", ios::binary);
    if (!outFile) {
        cout << "Error opening file. Accounts not saved." << endl;
        return;
    }

    saveToFileHelper(outFile, root);

    outFile.close();
}

// Account methods implementation

// Create account
void account::create_account(AVLTree& accountTree) {
    system("clear");
    cout << "\n\t\t\tEnter the Account No. : ";
    cin >> acno;
    
    // Check if account number already exists
    if (accountTree.searchAccount(acno) != nullptr) {
        cout << "\n\t\t\tAccount number already exists. Please enter a different account number.\n";
        return;
    }

    cout << "\n\n\t\t\tPlease Enter the Name of the Account holder Here : ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\n\t\t\tEnter Type of the Account (C/S) : ";
    cin >> type;
    type = toupper(type);
    cout << "\n\t\t\tEnter The Initial amount : ";
    cin >> deposit;
    cout << "\n\n\t\t\tAccount Created..";
}

// Show account details
void account::show_account() const {
    cout << "\n\t\t\tAccount No. : " << acno;
    cout << "\n\t\t\tAccount Holder Name : ";
    cout << name;
    cout << "\n\t\t\tType of Account : " << type;
    cout << "\n\t\t\tBalance amount : " << deposit;
}

// Modify account details
void account::modify() {
    cout << "\n\t\t\tAccount No. : " << acno;
    cout << "\n\t\t\tModify Account Holder Name : ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\n\t\t\tModify Type of Account : ";
    cin >> type;
    type = toupper(type);
    cout << "\n\t\t\tModify Balance amount : ";
    cin >> deposit;
}

// Deposit money into account
void account::dep(int x) {
    deposit += x;
}

// Withdraw money from account
void account::draw(int x) {
    deposit -= x;
}

// Display account report
void account::report() const {
    cout << acno << setw(10) << " " << name << setw(10) << " " << type << setw(6) << deposit << endl;
}

// Return account number
int account::retacno() const {
    return acno;
}

// Return account balance
int account::retdeposit() const {
    return deposit;
}

// Return account type
char account::rettype() const {
    return type;
}

// Main program
int main() {
    AVLTree accountTree;
    account ac;
    int num;
    char ch;

    // Load accounts from file
    accountTree.loadFromFile();

    do {
        system("CLS");
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
                ac.create_account(accountTree);
                accountTree.addAccount(ac);
                break;
            case '2':
                system("CLS");
                cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
                accountTree.searchAccount(num)->dep(num);
                break;
            case '3':
                system("CLS");
                cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
                accountTree.searchAccount(num)->draw(num);
                break;
            case '4':
                system("CLS");
                cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
                accountTree.searchAccount(num)->show_account();
                break;
            case '5':
                system("CLS");
                accountTree.displayAll();
                break;
            case '6':
                system("CLS");
                cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
                accountTree.deleteAccount(num);
                break;
            case '7':
                system("CLS");
                cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
                accountTree.searchAccount(num)->modify();
                break;
            case '8':
                system("CLS");
                cout << "\n\n\t\t\t Thank You For Using The Banking Services";
                break;
            default: cout << "\a";
        }
        cin.ignore();
        cin.get();
    } while (ch != '8');

    // Save accounts to file before exiting
    accountTree.saveToFile();

    return 0;
}

