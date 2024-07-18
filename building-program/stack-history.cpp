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

    string displayGenre() {
        // History* current = top;
        // for(int i = 0; i < 10 ; i++) {

        // }
    }

    void push(Book newBook) {
        History* newNode = new History;
        newNode->data = newBook;
        newNode->next = top;
        top = newNode;
    }

    void pop() {
        if(top == nullptr) {
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
        if(top == nullptr) {
            cout << "History Buku, kosong" << endl;
            return ;
        }
        return top->data;
    }

    void displayStack() {
        History* current = top;
        if(top == nullptr) {
            cout << "History Buku, kosong..." <<endl;
            return;
        }
        while (current != nullptr) {
            cout << "ID: " << current->data.bookId << endl;
            cout << "Judul buku: " << current->data.title << endl;
            cout << "Penulis: " << current->data.author << endl;
            cout << "Genre: " << current->data.genre << endl;
            cout << "Sinopsis: " << current->data.sinopsis << "\n\n";
            current = current->next; 
        }
        char ch;
        cout << "1. Selesai" << endl;
        ch = getch();
        system("cls");
    }

    bool isEmpty() {
        return (top == nullptr);
    }
};
