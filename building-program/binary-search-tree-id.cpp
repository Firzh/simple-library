#include <iostream>
#include <string>
using namespace std;

struct Book {
    int id;
    string title;
    string author;
    Book* left;
    Book* right;
};

class Library {
private:
    Book* root;

    Book* createNode(int id, string title, string author) {
        Book* newBook = new Book;
        newBook->id = id;
        newBook->title = title;
        newBook->author = author;
        newBook->left = newBook->right = nullptr;
        return newBook;
    }

    Book* insertBook(Book* root, int id, string title, string author) {
        if (root == nullptr) {
            return createNode(id, title, author);
        }
        if (id < root->id) {
            root->left = insertBook(root->left, id, title, author);
        } else {
            root->right = insertBook(root->right, id, title, author);
        }
        return root;
    }

    void inorderTraversal(Book* root) {
        if (root != nullptr) {
            inorderTraversal(root->left);
            cout << "ID: " << root->id << ", Title: " << root->title << ", Author: " << root->author << endl;
            inorderTraversal(root->right);
        }
    }

    Book* searchBook(Book* root, int id) {
        if (root == nullptr || root->id == id) {
            return root;
        }
        if (id < root->id) {
            return searchBook(root->left, id);
        }
        return searchBook(root->right, id);
    }

public:
    Library() {
        root = nullptr;
    }

    void addBook(int id, string title, string author) {
        root = insertBook(root, id, title, author);
    }

    void displayBooks() {
        inorderTraversal(root);
    }

    void findBook(int id) {
        Book* book = searchBook(root, id);
        if (book != nullptr) {
            cout << "Book Found! ID: " << book->id << ", Title: " << book->title << ", Author: " << book->author << endl;
        } else {
            cout << "Book Not Found" << endl;
        }
    }
};

int main() {
    Library library;
    library.addBook(1, "The Great Gatsby", "F. Scott Fitzgerald");
    library.addBook(2, "1984", "George Orwell");
    library.addBook(3, "To Kill a Mockingbird", "Harper Lee");

    cout << "All Books in Library:" << endl;
    library.displayBooks();

    cout << endl << "Searching for Book with ID 2:" << endl;
    library.findBook(2);

    cout << endl << "Searching for Book with ID 4:" << endl;
    library.findBook(4);

    return 0;
}
