#include <iostream>
#include <string>
using namespace std;

struct Book {
    int bookId;
    string title;
    string author;
    string genre[10];
    string sinopsis;
    Book* left;
    Book* right;
};

class Library {
private:
    Book* root;

    Book* createNode(int bookId, string title, string author, string genre[], string sinopsis) {
        Book* newBook = new Book;
        newBook->bookId = bookId;
        newBook->title = title;
        newBook->author = author;
        for (int i = 0; i < 10; i++) {
            newBook->genre[i] = genre[i];
        }
        newBook->sinopsis = sinopsis;
        newBook->left = newBook->right = nullptr;
        return newBook;
    }

    Book* insertBook(Book* root, int bookId, string title, string author, string genre[], string sinopsis) {
        if (root == nullptr) {
            return createNode(bookId, title, author, genre, sinopsis);
        }
        if (bookId < root->bookId) {
            root->left = insertBook(root->left, bookId, title, author, genre, sinopsis);
        } else {
            root->right = insertBook(root->right, bookId, title, author, genre, sinopsis);
        }
        return root;
    }

    void inorderTraversal(Book* root) {
        if (root != nullptr) {
            inorderTraversal(root->left);
            cout << "ID: " << root->bookId << ", Title: " << root->title << ", Author: " << root->author << ", Sinopsis: " << root->sinopsis << endl;
            inorderTraversal(root->right);
        }
    }

    Book* searchBookByTitle(Book* root, const string& title) {
        if (root == nullptr || root->title == title) {
            return root;
        }
        if (title < root->title) {
            return searchBookByTitle(root->left, title);
        }
        return searchBookByTitle(root->right, title);
    }

    Book* searchBookByAuthor(Book* root, const string& author) {
        if (root == nullptr || root->author == author) {
            return root;
        }
        Book* leftSearch = searchBookByAuthor(root->left, author);
        if (leftSearch != nullptr) {
            return leftSearch;
        }
        return searchBookByAuthor(root->right, author);
    }

public:
    Library() {
        root = nullptr;
    }

    void addBook(int bookId, string title, string author, string genre[], string sinopsis) {
        root = insertBook(root, bookId, title, author, genre, sinopsis);
    }

    void displayBooks() {
        inorderTraversal(root);
    }

    void findBookByTitle(const string& title) {
        Book* book = searchBookByTitle(root, title);
        if (book != nullptr) {
            cout << "Book Found! ID: " << book->bookId << ", Title: " << book->title << ", Author: " << book->author << ", Sinopsis: " << book->sinopsis << endl;
        } else {
            cout << "Book Not Found" << endl;
        }
    }

    void findBookByAuthor(const string& author) {
        Book* book = searchBookByAuthor(root, author);
        if (book != nullptr) {
            cout << "Book Found! ID: " << book->bookId << ", Title: " << book->title << ", Author: " << book->author << ", Sinopsis: " << book->sinopsis << endl;
        } else {
            cout << "Book Not Found" << endl;
        }
    }
};

int main() {
    Library library;
    string genres1[10] = {"Fiction", "Classic"};
    string genres2[10] = {"Dystopian", "Science Fiction"};
    string genres3[10] = {"Classic", "Drama"};

    library.addBook(1, "The Great Gatsby", "F. Scott Fitzgerald", genres1, "A story about the mysterious Jay Gatsby...");
    library.addBook(2, "1984", "George Orwell", genres2, "A dystopian novel set in a totalitarian society...");
    library.addBook(3, "To Kill a Mockingbird", "Harper Lee", genres3, "A novel about the serious issues of rape and racial inequality...");

    cout << "All Books in Library:" << endl;
    library.displayBooks();

    cout << endl << "Searching for Book with Title '1984':" << endl;
    library.findBookByTitle("1984");

    cout << endl << "Searching for Book with Author 'Harper Lee':" << endl;
    library.findBookByAuthor("Harper Lee");

    return 0;
}
