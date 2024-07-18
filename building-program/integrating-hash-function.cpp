#include <iostream>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <string>

using namespace std;
const int HASH_SIZE = 10;
const string PI = "31415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549308196442881097566593344612847564823378678316527120190914564856692346034861045432664821339360726024914127372458700663155881748815209209628292540919171536436789259036001133053054882046652138414695194151160943305727036575959195309218611738193261179310511854807446237996274956735188575272489122793818301194912983367336244065664308602139494639522473719070217986094370277053921717629317678765195982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989";

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
    return sum;
}

int generateId(string username, string password) {    
    static int id_counter = 0; // Penambah ID
    int usernameNum = konversiJumlahString(username);
    int firstNamaNum = konversiHuruf(username[0]);
    int lastNamaNum = konversiHuruf(username[username.size() - 1]);

    int id = id_counter++; // Increment ID counter
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

int main() {
    string username, password, usname, passwd;
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

    cout << "Login\n\n";
    cout << "Masukkan Username : ";
    cin >> usname;
    cout << "Masukkan Password : ";
    passwd = passwordMask();
    
    if (searchAdmin(username, password) == true) cout << "true";
    else cout << "false";

    return 0;
}
