#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

struct Book {
    int bookId;
    string title;
    string author;
    string genre[10];
    string sinopsis;
    bool pinjam;
    Book* left;
    Book* right;
};

struct History {
    Book data;
    History* next;
};

class Stack {
private:
    History* top;

public:
    Stack() {
        top = nullptr;
    }

    void push(Book newBook) {
        History* newNode = new History;
        newNode->data = newBook;
        newNode->next = top;
        top = newNode;
    }

    void pop() {
        if (top == nullptr) {
            cout << "History Buku Kosong. Silahkan meminjam buku :)" << endl;
            return;
        }
        History* temp = top;
        top = top->next;
        delete temp;
    }

    void clear() {
        while (top != nullptr) {
            pop();
        }
        cout << "Seluruh History Buku berhasil dihapus" << endl;
    }

    Book topElement() {
        if (top == nullptr) {
            cout << "History Buku kosong." << endl;
            return Book();
        }
        return top->data;
    }

    void displayStack() {
        History* current = top;
        if (top == nullptr) {
            cout << "History Buku kosong..." << endl;
            return;
        }
        while (current != nullptr) {
            cout << "ID: " << current->data.bookId << endl;
            cout << "Judul buku: " << current->data.title << endl;
            cout << "Penulis: " << current->data.author << endl;
            cout << "Genre: ";
            for (int i = 0; i < 10; i++) {
                if (!current->data.genre[i].empty()) {
                    cout << current->data.genre[i] << (i < 9 && !current->data.genre[i + 1].empty() ? ", " : "");
                }
            }
            cout << endl;
            cout << "Sinopsis: " << current->data.sinopsis << endl;
            cout << "Status Peminjaman: " << (current->data.pinjam ? "Dipinjam" : "Tersedia") << "\n\n";
            current = current->next;
        }
        cout << "1. Selesai" << endl;
        char ch = getch();
        system("cls");
    }

    bool isEmpty() {
        return (top == nullptr);
    }

    string displayGenre(Book& book) {
        string genreList = "";
        for (int i = 0; i < 10; i++) {
            if (!book.genre[i].empty()) {
                genreList += book.genre[i] + (i < 9 && !book.genre[i + 1].empty() ? ", " : "");
            }
        }
        return genreList;
    }
};

int main() {
    Stack history;
    Book book1 = {1, "The Great Gatsby", "F. Scott Fitzgerald", {"Fiction", "Classic"}, "A story about the mysterious Jay Gatsby...", false, nullptr, nullptr};
    Book book2 = {2, "1984", "George Orwell", {"Dystopian", "Science Fiction"}, "A dystopian novel set in a totalitarian society...", false, nullptr, nullptr};
    Book book3 = {3, "To Kill a Mockingbird", "Harper Lee", {"Classic", "Drama"}, "A novel about the serious issues of rape and racial inequality...", false, nullptr, nullptr};

    history.push(book1);
    history.push(book2);
    history.push(book3);

    cout << "Display Stack:" << endl;
    history.displayStack();

    cout << "Clearing Stack:" << endl;
    history.clear();

    cout << "Display Stack after clearing:" << endl;
    history.displayStack();

    return 0;
}
