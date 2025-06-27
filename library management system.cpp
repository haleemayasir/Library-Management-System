#include <iostream>
#include <string>
using namespace std;

const int TABLE_SIZE = 100; // Hash table size
class Book  // Class for a Book
{
    public:
    string title, author, isbn;                
    bool isAvailable; // Availability status
    Book(string t, string a, string i, bool available = true) 
	: title(t), author(a), isbn(i), isAvailable(available) {}
};

class TreeNode {  // Node for Binary Search Tree (BST)
public:
    Book* book;  // Pointer to a Book object
    TreeNode* left = nullptr;  // Pointer to the left child node
    TreeNode* right = nullptr; // Pointer to the right child node
    TreeNode(Book* b) : book(b) {}
};

//Hash Table Implementation
class HashNode {
    public:
    string key; //Key used in the hash table (ISBN of the book)
    Book* book;                               
    HashNode* next = nullptr; //Pointer to the next node in the linked list for collision handling
    HashNode(string k, Book* b) : key(k), book(b) {}
};

class HashTable {                           
    private:
    HashNode* table[TABLE_SIZE] = {nullptr}; //Array of hash nodes
    int hashFunction(const string& key) {   // Hash function to calculate index from ISBN
        int hash = 0;
        for (char c : key) hash = (hash * 31 + c) % TABLE_SIZE; // Compute hash value using a prime number
        return hash;
    }

    public:
    void insert(const string& key, Book* book) { // Insert a book into the hash table using its ISBN 
        int index = hashFunction(key);                
        HashNode* newNode = new HashNode(key, book); // Create a new hash node with the key and book
        newNode->next = table[index];   //Point the next pointer of the new node to the current head
        table[index] = newNode;         //Insert the new node at the head of the list
    }
    Book* search(const string& key) {  // Search for a book by ISBN in the hash table
        int index = hashFunction(key);               
        for (HashNode* current = table[index]; current; current = current->next) // Traverse the linked list at index
            if (current->key == key) return current->book;   // Return book if key matches
        return nullptr;   // Return nullptr if book is not found
    }

    void remove(const string& key) {
    int index = hashFunction(key); // Compute the hash index for the given key
    HashNode* current = table[index];
    HashNode* prev = nullptr;

    while (current) {
        if (current->key == key) { // Found the node with the matching key
            if (prev) {
                prev->next = current->next; // Update the previous node's 'next' pointer
            } 
			else {
                table[index] = current->next; // Update the head of the linked list
            }
            delete current; // Free memory for the removed node
            return;
        }
        prev = current;       // Move the 'prev' pointer to the current node
        current = current->next; // Move to the next node in the list
       }
    }
};

// Custom Queue Implementation
class QueueNode {
    public:
    Book* book; // Pointer to a Book object stored in this queue node
    QueueNode* next = nullptr; // Pointer to the next node in the queue, initialized to nullptr

    // Constructor to initialize the node with a Book pointer
    QueueNode(Book* b) : book(b) {}
};

class CustomQueue {
    private:
    QueueNode* front = nullptr; // Pointer to the front node
    QueueNode* rear = nullptr;  // Pointer to the rear node

    public:
    // Adds a new book to the rear of the queue
    void enqueue(Book* book) {
        QueueNode* newNode = new QueueNode(book); // Create a new node with the book
        if (!rear) // If the queue is empty, both front and rear point to the new node
            front = rear = newNode;
        else 
            rear = rear->next = newNode; // Otherwise, add the new node to the rear
    }

    // Removes and returns the book from the front of the queue
    Book* dequeue() {
        if (!front) return nullptr; // Return nullptr if the queue is empty
        QueueNode* temp = front;    // Temporary pointer to the front node
        front = front->next;        // Move the front pointer to the next node
        if (!front) rear = nullptr; // If the queue is now empty, set rear to nullptr
        Book* book = temp->book;    // Get the book from the removed node
        delete temp;                // Delete the removed node
        return book;                // Return the book
    }

    // Checks if the queue is empty
    bool isEmpty() const { return !front; }
};


// User Class
class User {
public:
    string username; //string to store username
    string password; //string to store password

    User(string u, string p) : username(u), password(p) {}
};

// Hash Table for Users
class UserHashTable {
private:
    User* table[TABLE_SIZE] = {nullptr}; // Hash table to store User objects

    // Hash function to compute an index based on username
    int hashFunction(const string& key) {
        int hash = 0;
        for (char c : key) 
            hash = (hash * 31 + c) % TABLE_SIZE; // Compute hash value
        return hash; // Return the computed index 
    }

public:
    // Inserts a new User object into the hash table
    void insert(const string& username, const string& password) {
        int index = hashFunction(username); // Compute the index using the hash function
        table[index] = new User(username, password); // Store a new User object at the computed index
    }

    // Searches for a User object in the hash table based on the username
    User* search(const string& username) {
        int index = hashFunction(username); // Compute the index using the hash function
        // Check if a User exists at the index and if the username matches
        return table[index] && table[index]->username == username ? table[index] : nullptr;
    }
};


// Undo Stack Implementation
class UndoNode {
public:
    string action;    // The action to be undone
    string isbn;      // The ISBN associated with the action
    UndoNode* next = nullptr; // Pointer to the next node in the stack

    // Constructor to initialize an UndoNode with an action and ISBN
    UndoNode(const string& a, const string& i = "") : action(a), isbn(i) {}
};

class UndoStack {
private:
    UndoNode* top = nullptr; // Pointer to the top node of the stack

public:
    // Pushes a new undo action onto the stack
    void push(const string& action, const string& isbn = "") {
        UndoNode* newNode = new UndoNode(action, isbn); // Create a new node with action and ISBN
        newNode->next = top; // Link the new node to the current top node
        top = newNode;       // Update the top pointer to the new node
    }

    // Removes and returns the top undo action from the stack
    UndoNode* pop() {
        if (!top) return nullptr; // If the stack is empty, return nullptr
        UndoNode* temp = top;     // Temporarily store the current top node
        top = top->next;          // Update the top pointer to the next node
        return temp;              // Return the removed node
    }

    // Checks if the stack is empty
    bool isEmpty() const {
        return top == nullptr; // Returns true if the stack has no nodes
    }
};

// Library Management System Class
class Library {
    private:
    TreeNode* root = nullptr; // Root node of BST 
    HashTable isbnTable;      // Hash table for ISBN-based book lookup    
    CustomQueue issueQueue;   // Queue for managing issued books    
    UserHashTable userTable;  // Hash table for storing user information  
    string currentUser;       // Currently logged-in user  
    UndoStack undoStack;      // Stack for undoing actions
    
    class AddedBookNode {    // Linked list to track added books for undo
        public:
		Book* book;
        AddedBookNode* next;
        AddedBookNode(Book* b) : book(b), next(nullptr) {}
    };
    AddedBookNode* addedBooksHead = nullptr;

    //function to insert a book into the BST
    TreeNode* insertIntoBST(TreeNode* node, Book* book) {
        if (!node) return new TreeNode(book);  // Create new node if the current is null
        if (book->title < node->book->title)  // Recursively insert into the left or right subtree
            node->left = insertIntoBST(node->left, book);
        else
            node->right = insertIntoBST(node->right, book);
        return node;
    }

    //function to remove a book from the BST
    TreeNode* removeFromBST(TreeNode* root, const string& title) {
        if (!root) return root;  // return if empty tree
        if (title < root->book->title) {  
            root->left = removeFromBST(root->left, title);
        } else if (title > root->book->title) {
            root->right = removeFromBST(root->right, title);
        } else {
            // Node with only one child or no child
            if (!root->left) {
                TreeNode* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                TreeNode* temp = root->left;
                delete root;
                return temp;
            }
            // Node with two children: find in-order successor
            TreeNode* temp = minValueNode(root->right);
            root->book = temp->book; 
            root->right = removeFromBST(root->right, temp->book->title);  // Delete successor
        }
        return root;
    }

    //function to find the node with the smallest value in a subtree
    TreeNode* minValueNode(TreeNode* node) {
        TreeNode* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }

    //function to search for a book in the BST by title
    TreeNode* searchInBST(TreeNode* node, const string& title) {
        if (!node || node->book->title == title)
            return node;  // return if book found or tree is empty
        if (title < node->book->title)
            return searchInBST(node->left, title);
        return searchInBST(node->right, title);
    }

    //function to perform in-order traversal of the BST
    void inOrder(TreeNode* node) {
        if (!node) return;
        inOrder(node->left);
        cout << "Title: " << node->book->title << ", Author: " << node->book->author
             << ", ISBN: " << node->book->isbn << ", Available: "
             << (node->book->isAvailable ? "Yes" : "No") << endl;
        inOrder(node->right);
    }

    //function to remove a book from the hash table
    void removeFromHashTable(const string& isbn) {
        isbnTable.remove(isbn);
    }

    //function to track added books for undo operations
    void addToAddedBooks(Book* book) {
        AddedBookNode* newNode = new AddedBookNode(book);
        newNode->next = addedBooksHead;
        addedBooksHead = newNode;
    }

public:
    // Constructor
    Library() {}

    // Register a new user
    void registerUser(const string& username, const string& password) {
        if (userTable.search(username)) {  // Check if username already exists
            printHeading();
            cout << "Username already exists. Please choose a different username.\n";
            return;
        }
        userTable.insert(username, password);  // Add new user
        printHeading();
        cout << "User registered successfully!\n";
    }

    // User login functionality
    bool loginUser(const string& username, const string& password) {
        User* user = userTable.search(username);
        if (!user || user->password != password) {  // Validate username and password
            printHeading();
            cout << "Invalid username or password.\n";
            return false;
        }
        currentUser = username;  // Set the current user
        printHeading();
        cout << "Login successful! Welcome, " << username << ".\n";
        return true;
    }

    // User logout functionality
    void logoutUser() {
        if (currentUser.empty()) {  // Check if any user is logged in
            printHeading();
            cout << "No user is currently logged in.\n";
            return;
        }
        printHeading();
        cout << "User " << currentUser << " logged out successfully.\n";
        currentUser.clear();  // Clear the current user
    }

    // Add a new book to the library
    void addBook(const string& title, const string& author, const string& isbn, bool isAvailable = true) {
        Book* newBook = new Book(title, author, isbn, isAvailable);  // Create a new book object
        root = insertIntoBST(root, newBook);  // Insert the book into the BST
        isbnTable.insert(isbn, newBook);  // Add the book to the hash table
        addToAddedBooks(newBook);  // Track the book for undo operations
        undoStack.push("addBook", isbn);  // Push the action to the undo stack
        printHeading();
        cout << "Book added successfully!\n";
    }

    // Display all books in the library
    void displayAllBooks() {
        if (!root) {  // Check if the library is empty
            printHeading();
            cout << "No books available in the library.\n";
            return;
        }
        printHeading();
        cout << "Books in the library:\n";
        inOrder(root);  // Perform in-order traversal of the BST
    }

    // Search for a book by its title
    void searchBookByTitle(const string& title) {
        TreeNode* result = searchInBST(root, title);  // Search the BST
        if (result) {
            printHeading();
            cout << "Book Found:\n"
                 << "Title: " << result->book->title << ", Author: " << result->book->author
                 << ", ISBN: " << result->book->isbn << ", Available: "
                 << (result->book->isAvailable ? "Yes" : "No") << endl;
        } else {
            printHeading();
            cout << "No book found with title: " << title << endl;
        }
    }

    // Issue a book to a user
    void issueBook(const string& isbn) {
        Book* book = isbnTable.search(isbn);  // Search for the book by ISBN
        if (!book) {
            printHeading();
            cout << "Book with ISBN " << isbn << " not found.\n";
        } else if (book->isAvailable) {
            book->isAvailable = false;  // Mark the book as unavailable
            issueQueue.enqueue(book);  // Add the book to the issue queue
            undoStack.push("issueBook", isbn);  // Push the action to the undo stack
            printHeading();
            cout << "Book issued successfully!\n";
        } else {
            printHeading();
            cout << "Book is not available.\n";
        }
    }

    // Return a book to the library
    void returnBook(const string& isbn) {
        Book* book = isbnTable.search(isbn);  // Search for the book by ISBN
        if (!book) {
            printHeading();
            cout << "Book with ISBN " << isbn << " not found.\n";
        } else if (book->isAvailable) {
            printHeading();
            cout << "This book was not issued, so it cannot be returned.\n";
        } else {
            book->isAvailable = true;  // Mark the book as available
            undoStack.push("returnBook", isbn);  // Push the action to the undo stack
            printHeading();
            cout << "Book returned successfully!\n";
        }
    }

    // Undo the last action
    void undo() {
        if (undoStack.isEmpty()) {  // Check if the undo stack is empty
            printHeading();
            cout << "No actions to undo.\n";
            return;
        }

        UndoNode* lastAction = undoStack.pop();  // Get the last action from the stack
        if (lastAction->action == "addBook") {
            // Undo adding a book
            Book* lastBook = isbnTable.search(lastAction->isbn);
            if (lastBook) {
                root = removeFromBST(root, lastBook->title);  // Remove the book from the BST
                removeFromHashTable(lastAction->isbn);  // Remove the book from the hash table
                printHeading();
                cout << "Undo: Last book addition undone.\n";
            }
        } else if (lastAction->action == "issueBook") {
            // Undo issuing a book
            Book* book = isbnTable.search(lastAction->isbn);
            if (book) {
                book->isAvailable = true;  // Make the book available again
                printHeading();
                cout << "Undo: Last book issue undone.\n";
            }
        } else if (lastAction->action == "returnBook") {
            // Undo returning a book
            Book* book = isbnTable.search(lastAction->isbn);
            if (book) {
                book->isAvailable = false;  // Make the book unavailable again
                printHeading();
                cout << "Undo: Last book return undone.\n";
            }
        } else {
            printHeading();
            cout << "Unknown action to undo.\n";
        }
    }
    
    // Print Heading
    void printHeading() {
    	system("cls");
        cout << "\t\t\t\t\t\t\t\t\tLibrary Management System\n";
    }
    // function to print library options
    void showMenu() {
        int choice;
        printHeading();
        do {
            cout << "\nLibrary Menu:\n"
                 << "1. Add Book\n"
                 << "2. Display All Books\n"
                 << "3. Search Book by Title\n"
                 << "4. Issue Book\n"
                 << "5. Return Book\n"
                 << "6. Undo\n"
                 << "7. Logout\n"
                 << "Enter choice: ";
            cin >> choice;
            cin.ignore();  // To consume the newline character

            string title, author, isbn;
            switch (choice) {
                case 1:
                    cout << "Enter title: ";
                    getline(cin, title);
                    cout << "Enter author: ";
                    getline(cin, author);
                    cout << "Enter ISBN: ";
                    getline(cin, isbn);
                    addBook(title, author, isbn);
                    break;
                case 2:
                    displayAllBooks();
                    break;
                case 3:
                    cout << "Enter title to search: ";
                    getline(cin, title);
                    searchBookByTitle(title);
                    break;
                case 4:
                    cout << "Enter ISBN to issue: ";
                    getline(cin, isbn);
                    issueBook(isbn);
                    break;
                case 5:
                    cout << "Enter ISBN to return: ";
                    getline(cin, isbn);
                    returnBook(isbn);
                    break;
                case 6:
                    undo();
                    break;
                case 7:
                    logoutUser();
                    break;
                default:
                    cout << "Invalid choice, please try again.\n";
            }
        } while (choice != 7);  //condition for logout
    }
};


// Main Function
int main() {
    Library library;
    int choice;
    cout << "\t\t\t\t\t\t\t\t\tLibrary Management System\n";
    do {
        cout << "1. Sign up\n"
             << "2. Login\n"
             << "3. Exit\n"
             << "what operation would you like to perform?: ";
        cin >> choice;
        cin.ignore();

        string username, password;
        switch (choice) {
            case 1:
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                library.registerUser(username, password);
                break;
            case 2:
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                if (library.loginUser(username, password)) {
                    system("cls");
                    library.showMenu();
                }
                break;
            case 3:
                cout << "Exiting.......!\n";
                break;
            default:
                cout << "Invalid option!\n";
        }
    } while (choice != 3);

    return 0;
}
