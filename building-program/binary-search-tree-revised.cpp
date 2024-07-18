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
        if (title < root->title) {
            root->left = insertBook(root->left, bookId, title, author, genre, sinopsis);
        } else if (title > root->title) {
            root->right = insertBook(root->right, bookId, title, author, genre, sinopsis);
        }
        return root;
    }

    void inorderTraversal(Book* root) {
        if (root != nullptr) {
            inorderTraversal(root->left);
            cout << "ID: " << root->bookId << "\nTitle: " << root->title << "\nAuthor: " << root->author << "\nSinopsis: " << root->sinopsis << "\n\n";
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

    void searchBookByAuthor(Book* root, const string& author) {
        if (root == nullptr) {
            return;
        }
        searchBookByAuthor(root->left, author);
        if (root->author == author) {
            cout << "Book Found! \nID: " << root->bookId << "\nTitle: " << root->title << "\nAuthor: " << root->author << "\nSinopsis: " << root->sinopsis << "\n\n";
        }
        searchBookByAuthor(root->right, author);
    }

    void searchBookByGenre(Book* root, const string& genre) {
        if (root == nullptr) {
            return;
        }
        searchBookByGenre(root->left, genre);
        for (int i = 0; i < 10; i++) {
            if (root->genre[i] == genre) {
                cout << "Book Found! \nID: " << root->bookId << "\nTitle: " << root->title << "\nAuthor: " << root->author << "\nSinopsis: " << root->sinopsis << "\n\n";
                break;
            }
        }
        searchBookByGenre(root->right, genre);
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
            cout << "Book Found! \nID: " << book->bookId << "\nTitle: " << book->title << "\nAuthor: " << book->author << "\nSinopsis: " << book->sinopsis << "\n\n";
        } else {
            cout << "Book Not Found" << endl;
        }
    }

    void findBookByAuthor(const string& author) {
        cout << "Searching books by author: " << author << endl;
        searchBookByAuthor(root, author);
    }

    void findBookByGenre(const string& genre) {
        cout << "Searching books by genre: " << genre << endl;
        searchBookByGenre(root, genre);
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

    cout << endl << "Searching for Books with Genre 'Classic':" << endl;
    library.findBookByGenre("Classic");

    return 0;
}
