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
const int HASH_SIZE = 10;

// Mendapatkan waktu saat ini
time_t now = time(0);
tm *ltm = localtime(&now);

struct book {
    int bookId;
    string title;
    string author;
    string genre[10];
    string sinopsis;
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
//-- Hash_Table & Collision Handling
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
    return sum / 10;
}

int generateId(string username, string password) {    
    int id;
    int usernameNum = konversiJumlahString(username);
    int firstNamaNum = konversiHuruf(username[0]);
    int lastNamaNum = konversiHuruf(username[username.size() - 1]);

    if (firstNamaNum < 11) {
        return (id % 10) * 10000 + (usernameNum % 10) * 1000 + lastNamaNum * 100 + firstNamaNum;
    }
    return (id % 10) * 10000 + (usernameNum % 10) * 1000 + lastNamaNum * 100 + firstNamaNum;
}

void insertMemberRecursive(Admin*& head, Admin* admin) {
    if (head == nullptr) {
        head = admin;
    } else {
        insertMemberRecursive(head->next, admin);
    }
}

void insertMember(Admin* admin) {
    int hashIndex = hashFunction(admin->id);
    insertMemberRecursive(hashTable[hashIndex], admin);
}

void addAdmin(string username, string password) {
    Admin* newAdmin = new Admin;
    newAdmin->id = generateId(username, password);
    newAdmin->username = "";
    newAdmin->password = "";
    newAdmin->next = nullptr;
    insertMember(newAdmin);
}

void displayHashTableRecursive(Admin* current) {
    if (current == nullptr) {
        cout << "nullptr" << endl;
        return;
    }
    if (current->id > 9) {
        cout << current->id << " (" << current->username << ") -> ";
    } else if (current->id < 10) {
        cout << setfill('0') << current->id << " (" << current->username << ") -> ";
    }
    displayHashTableRecursive(current->next);
}

void displayHashTable() {
    int maxNamaLength;

    for (int i = 0; i < 10; i++) {
        cout << "Index " << i << ": ";
        displayHashTableRecursive(hashTable[i]);
    }
}

int main() {
    string username, password;
    char konfirmasi = 'y';

    while (konfirmasi != 'n') {
        cout << "Masukkan username : ";
        cin >> username;
        cout << "Masukkan password : ";
        password = passwordMask();

        addAdmin(username, password);
        displayHashTable();
        Sleep(2000);
        system("cls");
        cout << "Apakah ingin menambahkan lagi ?\n";
        cout << "> ";
        cin >> konfirmasi;
        system("cls");
    }
    return 0;
}
