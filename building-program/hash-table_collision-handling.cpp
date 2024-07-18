#include <iostream>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <string>

using namespace std;
const int HASH_SIZE = 10;

struct Admin {
    int id;
    string username;
    string password;
    Admin* next;
};

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
    static int id_counter = 0; // Penambah ID
    int usernameNum = konversiJumlahString(username);
    int firstNamaNum = konversiHuruf(username[0]);
    int lastNamaNum = konversiHuruf(username[username.size() - 1]);

    int id = id_counter++; // Increment ID counter
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
    newAdmin->username = username;
    newAdmin->password = password;
    newAdmin->next = nullptr;
    insertMember(newAdmin);
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
    int maxNamaLength;

    for (int i = 0; i < HASH_SIZE; i++) {
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
