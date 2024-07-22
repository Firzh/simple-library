#include "nlohmann\json.hpp"
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;
using json = nlohmann::json;

// Mendapatkan waktu saat ini
time_t now = time(0);
tm *ltm = localtime(&now);

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

struct DateTime {
    string jam;
    string hari;
    string tanggal;
};

struct Admin {
    int id;
    string username;
    string password;
    Admin* next;
};

struct History {
    Book data;
    History* next;
};



void clearScreen() {
    system("cls");
}

string passwordMask() {
    string password;
    char ch;

    ch = _getch();  

    while (ch != 13) { 
        if (ch != 8) { 
            password.push_back(ch);
            cout << '*';
        } else if (!password.empty()) { 
            password.pop_back();
            cout << "\b \b"; 
        }
        ch = _getch();
    }
    cout << endl;
    return password;
}

DateTime getDateTime() {
    DateTime result;

    // Menampilkan jam
    int jam = ltm->tm_hour;
    int min = ltm->tm_min;
    int sec = ltm->tm_sec;
    if (jam > 9) {
        if (sec >9)
            result.jam = to_string(jam) + ":" + to_string(min) + ":" + to_string(sec);
        if (sec < 10)
            result.jam = to_string(jam) + ":" + to_string(min) + ":0" + to_string(sec);
    }
    if (jam < 10) {
        if (sec >9)
            result.jam = to_string(jam) + ":" + to_string(min) + ":" + to_string(sec);
        if (sec < 10)
            result.jam = to_string(jam) + ":" + to_string(min) + ":0" + to_string(sec);
    }

    // Menampilkan hari
    switch (ltm->tm_wday) {
        case 0:
            result.hari = "Minggu, ";
            break;
        case 1:
            result.hari = "Senin, ";
            break;
        case 2:
            result.hari = "Selasa, ";
            break;
        case 3:
            result.hari = "Rabu, ";
            break;
        case 4:
            result.hari = "Kamis, ";
            break;
        case 5:
            result.hari = "Jumat, ";
            break;
        case 6:
            result.hari = "Sabtu, ";
            break;
    }

    // Menampilkan tanggal
    int tanggal = ltm->tm_mday;
    int bulan = 1 + ltm->tm_mon;
    int tahun = 1900 + ltm->tm_year;
    
    if (bulan > 9) {
        result.tanggal = to_string(tanggal) + "-" + to_string(bulan) + "-" + to_string(tahun);
    } else if (bulan < 10) {
        result.tanggal = to_string(tanggal) + "-0" + to_string(bulan) + "-" + to_string(tahun);
    }
    return result;
}

string headerBottomBorder  = "|_____________________________________________________________________________________________|\n";
string headerBorder        = "|=============================================================================================|\n";
string headerAdmin         = "|                                        Menu Admin                                           |\n";
string headerLogin         = "|                                          Login                                              |\n";
string headerMember        = "|                                        Menu Member                                          |\n";
string headerDenda         = "|                                           Denda                                             |\n";
string headerNewMember     = "|                                        Member Baru                                          |\n";
string headerListMember    = "|                                        List Member                                          |\n";
string headerKembalikanBuku= "|                                      Kembalikan Buku                                        |\n";
string headerPeminjamanBuku= "|                                      Peminjaman Buku                                        |\n";
string headerCariMember    = "|                                        Cari Member                                          |\n";
string headerPengaturanAdm = "|                                     Pengaturan Admin                                        |\n";
string headerNewBook       = "|                                         Buku Baru                                           |\n";
string headerDisplayBook   = "|                                        Daftar Buku                                          |\n";
string headerDisplayPinjam = "|                                 Daftar Member Pinjam Buku                                   |\n";
string headerDisplayDenda  = "|                                 Daftar Member Kena Denda                                    |\n";
string headerDisplayLog    = "|                                        Display Log                                          |\n";
string headerDisplay       = "|                                          Display                                            |\n";
string headerMembership    = "|                                        Membership                                           |\n";

// Fungsi untuk membaca file JSON
json bacaFileJson(const string& namaFile) {
    ifstream file(namaFile);
    json data;
    if (file.is_open()) {
        file >> data;
        file.close();
    }
    return data;
}

// Fungsi untuk menyimpan data ke file JSON
void simpanKeJson(const json& data, const string& namaFile) {
    ofstream file(namaFile);
    if (file.is_open()) {
        file << data.dump(4);
        file.close();
    }
}

//================================================== awal fungsi universal ===============================================
//                                                  -- Hashing Function
const string PI = "31415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549308196442881097566593344612847564823378678316527120190914564856692346034861045432664821339360726024914127372458700663155881748815209209628292540919171536436789259036001133053054882046652138414695194151160943305727036575959195309218611738193261179310511854807446237996274956735188575272489122793818301194912983367336244065664308602139494639522473719070217986094370277053921717629317678765195982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989";

unsigned long long hashingFunction(const string& input) {
    unsigned long long hash = 0;
    int pi_length = PI.length();

    for (char c : input) {
        int pos = static_cast<int>(c) % pi_length;                  // Temukan posisi dalam deret Pi
        hash = (hash * 10 + (PI[pos] - '0')) % 1000000007;          // Gabungkan nilai hash, modulo dengan bilangan prima besar untuk menghindari overflow
    }

    return hash;
}

bool cekHash(const string& name, const string& pass, const string& uname, const unsigned long long hash_pass) {
    return name == uname && hashingFunction(pass) == hash_pass;
}

//                                           -- Hash Table & Collision Handling
const int HASH_SIZE = 10;
Admin* hashTable[HASH_SIZE];

int hashFunction(int id) {
    return id % 10;
}

int konversiHuruf(char nama) {
    if (isalpha(nama)) {
        return toupper(nama) - 'A' + 1;
    } else {
        return -1;
    }
}

int konversiJumlahString(string tempat) {
    int sum = 0;
    for (char c : tempat) {
        if (isalpha(c)) {
            sum += toupper(c) - 'A' + 1;
        }
    }
    return sum;
}

int generateId(string username, string password) {    
    static int id_counter = 0; 
    int usernameNum = konversiJumlahString(username);
    int firstNamaNum = konversiHuruf(username[0]);
    int lastNamaNum = konversiHuruf(username[username.size() - 1]);

    int id = id_counter++; 
    return (id % 10) * 10000 + (usernameNum % 10) * 1000 + lastNamaNum * 100 + firstNamaNum;
}

void insertAdminRecursive(Admin*& head, Admin* admin) {
    if (head == nullptr) {
        head = admin;
    } else {
        insertAdminRecursive(head->next, admin);
    }
}

void insertAdmin(Admin* admin) {
    int hashIndex = hashFunction(admin->id);
    insertAdminRecursive(hashTable[hashIndex], admin);
}

void addAdmin(string username, string password) {
    Admin* newAdmin = new Admin;
    newAdmin->id = generateId(username, password);
    newAdmin->username = username;
    newAdmin->password = hashingFunction(password);
    newAdmin->next = nullptr;
    insertAdmin(newAdmin);
}

void displayHashTableRecursive(Admin* current) {
    if (current == nullptr) {
        cout << "nullptr" << endl;
        return;
    }
    cout << setfill('0') << setw(5) << current->id << " (" << current->username << ") -> ";
    displayHashTableRecursive(current->next);
}

void displayHashTable() {
    for (int i = 0; i < HASH_SIZE; i++) {
        cout << "Index " << i << ": ";
        displayHashTableRecursive(hashTable[i]);
    }
}

bool searchAdmin(string username, string password) {
    unsigned long long hashUsername = hashingFunction(username);
    unsigned long long hashPassword = hashingFunction(password);
    
    for (int i = 0; i < HASH_SIZE; i++) {
        Admin* current = hashTable[i];
        while (current != nullptr) {
            if (cekHash(username, password, current->username, hashPassword)) {
                return true;
            }
            current = current->next;
        }
    }
    return false;
}

//                                               -- Class Stack
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

//                                               -- Class Libray
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
            cout << "ID: " << root->bookId << "\nTitle: " << root->title << "\nPenulis: " << root->author << "\nSinopsis: " << root->sinopsis << "\n\n";
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
            cout << "Book Found! \nID: " << root->bookId << "\nTitle: " << root->title << "\nPenulis: " << root->author << "\nSinopsis: " << root->sinopsis << "\n\n";
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
                cout << "Book Found! \nID: " << root->bookId << "\nTitle: " << root->title << "\nPenulis: " << root->author << "\nSinopsis: " << root->sinopsis << "\n\n";
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
            cout << "Book Found! \nID: " << book->bookId << "\nTitle: " << book->title << "\nPenulis: " << book->author << "\nSinopsis: " << book->sinopsis << "\n\n";
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
    Stack history;

    string username, password, usname, passwd;
    char konfirmasi = 'y';

    // while (konfirmasi != 'n') {
    //     cout << "Masukkan username : ";
    //     cin >> username;
    //     cout << "Masukkan password : ";
    //     password = passwordMask();

    //     addAdmin(username, password);
    //     displayHashTable();
    //     Sleep(2000);
    //     system("cls");
    //     cout << "Apakah ingin menambahkan lagi ?\n";
    //     cout << "> ";
    //     cin >> konfirmasi;
    //     system("cls");
    // }

    // cout << "Login\n\n";
    // cout << "Masukkan Username : ";
    // cin >> usname;
    // cout << "Masukkan Password : ";
    // passwd = passwordMask();
    
    // if (searchAdmin(username, password) == true) cout << "true";
    // else cout << "false";

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
