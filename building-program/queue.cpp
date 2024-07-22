#include <windows.h>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <cstdlib>
#include <string>
#include <cctype>
#include <ctime>

using namespace std;

struct Admin {
    string username = "Admin";
    string password = "ApaAja";
};

struct Supir {
    int id;
    string nama;
    string lahir;
    string alamat;
    char gender;
};

struct Node {
    Supir data;
    Node* next;
    Node* prev;
};

struct Order {
    double id;
    string nama;
    string supir;
    string platNomor;
    string tujuan;

    Order* next;
};

//==========================================================================================================
//                                              Awal Class Queue & Linkedlist
//==========================================================================================================

class Queue {
private:
    Order* front;
    Order* rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(Order newData) {
        Order* newNode = new Order;
        newNode->id = newData.id;
        newNode->nama = newData.nama;
        newNode->supir = newData.supir;
        newNode->platNomor = newData.platNomor;        
        newNode->tujuan = newData.tujuan; 
        newNode->next = nullptr;

        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "Queue kosong." << endl;
            return;
        }

        Order* temp = front;
        front = front->next;
        delete temp;

        if (front == nullptr) {
            rear = nullptr;
        }
    }

    bool isEmpty() {
        return (front == nullptr);
    }

    void displayQueue() {
        if (isEmpty()) {
            cout << "Queue kosong." << endl;
            return;
        }

        Order* current = front;
        while (current != nullptr) {
            cout << "ID         :      " << fixed << setprecision(0) << current->id  << endl 
            << "Nama       :      " << current->nama << endl
            << "Supir      :      " << current->supir << endl
            << "Plat Nomor :      " << current->platNomor << endl 
            << "Tujuan     :      " << current->tujuan ;

            current = current->next;
            char ch;
            cout << "\n\n1. Selesai ";
            ch = getch();
            system("cls");
        }
    }

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

class DoubleLinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    DoubleLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    void insertNode(Supir newData) {
        if (size >= 100) {
            cout << "Stack sudah penuh" << endl;
            return;
        }
        
        int baseID = generateUniqueID(newData);
        int newID = baseID;

        Node* current = head;
        while (current != nullptr) {
            if (current->data.id == newID) {
                newID++;  
                current = head; 
            } else {
                current = current->next;
            }
            if (current == tail) {
                break;
            }
        }
        newData.id = newID;
        
        Node* newNode = new Node;
        newNode->data = newData;

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
            newNode->next = newNode;
            newNode->prev = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
            tail = newNode;
        }

        size++;
    }

//==========================================================================================================
//                                              Awal Fungsi Generate ID
//==========================================================================================================
    int konversiHurufKeAngka(char huruf) {
        if (isalpha(huruf)) {
            return toupper(huruf) - 'A' + 1;
        } else {
            return -1;
        }
    }

    int deteksiGender(char gender) {
        int hasilDeteksi = -1;  
        if (isalpha(gender)) {
            gender = toupper(gender);

            switch (gender) {
                case 'L':
                    hasilDeteksi = 1;
                    break;
                case 'P':
                    hasilDeteksi = 0;
                    break;
                default:
                    cout << "Karakter gender tidak dikenali." << endl;
            }
        } else {
            cout << "Input tidak valid untuk karakter gender." << endl;
        }

        return hasilDeteksi;
    }

    int calculateOnesPlaceSum(const string& date) {
        auto isNumeric = [](const string& str) {
            for (char c : str) {
                if (!isdigit(c)) {
                    return false;
                }
            }
            return true;
        };

        if (date.length() == 10 && date[2] == '-' && date[5] == '-') {
            string day = date.substr(0, 2);
            string month = date.substr(3, 2);
            string year = date.substr(6, 4);

            auto getOnesPlace = [isNumeric](const string& str) {
                if (isNumeric(str) && str.length() > 0) {
                    return str[str.length() - 1] - '0';
                }
                return -1;  
            };

            if (isNumeric(day) && isNumeric(month) && isNumeric(year)) {
                int dayInt = stoi(day);
                int monthInt = stoi(month);
                int yearInt = stoi(year);

                if (dayInt >= 1 && dayInt <= 31 && monthInt >= 1 && monthInt <= 12 && yearInt >= 1000) {
                    int onesPlaceDay = getOnesPlace(day);
                    int onesPlaceMonth = getOnesPlace(month);
                    int onesPlaceYear = getOnesPlace(year);

                    if (onesPlaceDay != -1 && onesPlaceMonth != -1 && onesPlaceYear != -1) {
                        return ((onesPlaceDay + onesPlaceMonth + onesPlaceYear)%9);
                    }
                }
            }
        }
        return -1;
    }

    int generateUniqueID(const Supir& supir) {
        int angkaHurufPertama = konversiHurufKeAngka(supir.nama[0]);
        int angkaHurufTerakhir = konversiHurufKeAngka(supir.nama[supir.nama.length() - 1]);
        int hasil = abs(angkaHurufPertama - angkaHurufTerakhir);
        int digitKetiga = deteksiGender(supir.gender);
        int digitKeempat = calculateOnesPlaceSum(supir.lahir);

        return (hasil * 1000) + (digitKetiga * 100) + (digitKeempat * 10);
    }

    void refreshAllIDs() {
        Node* current = head;
        Node* tail = this->tail;  // Inisialisasi tail
        while (current != nullptr) {
            current->data.id = generateUniqueID(current->data);
            current = current->next;
            if (current == tail) {
                break;
            }
        }
    }

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    string generatePlatNomor() {
        const string hurufAcak = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        srand(static_cast<unsigned int>(time(nullptr)));

        string empatAngka;
        for (int i = 0; i < 4; ++i) {
            empatAngka += to_string(rand() % 10);
        }

        string duaHurufAcak;
        for (int i = 0; i < 2; ++i) {
            duaHurufAcak += hurufAcak[rand() % hurufAcak.length()];
        }
        string platNomor = "L " + empatAngka + " " + duaHurufAcak;
        return platNomor;
    }

//=========================================================================================================
//                                              Awal Fungsi Admin
//=========================================================================================================

    void prosesPesanan(Queue orderQueue) {
        Node* current = head;

        while (true) {
            orderQueue.displayQueue();

            cout << "\n\n1. Accept\n2. Reject\n3. Exit\n\n>  ";
            int pilihan;
            cin >> pilihan;

            if (pilihan = 1) {
                orderQueue.dequeue();
                current = current->next;
            } else if (pilihan = 2) {
                orderQueue.dequeue();
                current = current->prev;
            } else {
                system("cls");
                break;
            }
        }
    }

    void displayList() {
        Node* current = head;
        Node* prev = head;  

        while (true) {
            displaySupirInfo(current);

            cout << "1. Next\n2. Previous\n3. Exit\n\n>  ";
            int pilihan;
            cin >> pilihan;

            if (pilihan == 1) {
                moveNext(current);
            } else if (pilihan == 2) {
                movePrevious(prev, current);
            } else {
                break;
            }
        }
    }

    // Fungsi tambahan untuk menampilkan informasi supir
    void displaySupirInfo(Node* current) {
        cout << "|------------------------------------------|" << endl;
        cout << "|Nama      : " << current->data.nama << endl;
        cout << "|Id        : " << setw(5) << setfill('0') << current->data.id << endl;
        cout << "|Tgl Lahir : " << current->data.lahir << endl;
        cout << "|Gender    : " << current->data.gender << endl;
        cout << "|Alamat    : " << current->data.alamat << endl;
        cout << "|------------------------------------------|\n" << endl;
    }

    // Fungsi tambahan untuk memindahkan ke node berikutnya
    void moveNext(Node*& current) {
        system("cls");
        current = current->next;
        if (current == nullptr) {
            current = head;
        }
    }

    // Fungsi tambahan untuk memindahkan ke node sebelumnya
    void movePrevious(Node*& prev, Node*& current) {
        while (prev->next != current) {
            system("cls");
            prev = prev->next;
        }
        current = prev;
    }

    void updateData(int id) {
        Node* current = head;
        while (current != nullptr && current->data.id != id) {
            current = current->next;
        }

        if (current != nullptr) {

            cout << "Pilih data yang ingin diupdate untuk supir dengan ID " << id << ":\n";
            cout << "1. Nama\n";
            cout << "2. Tanggal Lahir\n";
            cout << "3. Alamat\n";
            cout << "4. Gender\n";
            cout << "5. Batal\n\n>  ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Masukkan nama baru: ";
                    cin.ignore(); 
                    getline(cin, current->data.nama);
                    cout << "Nama berhasil diupdate!\n";
                    system("cls");
                    break;
                case 2:
                    cout << "Masukkan tanggal lahir baru: ";
                    cin.ignore();
                    getline(cin, current->data.lahir);
                    cout << "Tanggal Lahir berhasil diupdate!\n";
                    system("cls");
                    break;
                case 3:
                    cout << "Masukkan alamat baru: ";
                    cin >> current->data.alamat;
                    cout << "Alamat berhasil diupdate!\n";
                    system("cls");
                    break;
                case 4:
                    cout << "Masukkan gender baru (L/P): ";
                    cin >> current->data.gender;
                    while (current->data.gender != 'L' && current->data.gender != 'P') {
                        cout << "Pilihan tidak valid. Masukkan kembali (L/P): ";
                        cin >> current->data.gender;
                    }
                    cout << "Gender berhasil diupdate!\n";
                    system("cls");
                    break;
                case 5:
                    cout << "Batal mengupdate data.\n";
                    system("cls");
                    break;
                default:
                    cout << "Pilihan tidak valid.\n";
                    break;
            }
            current->data.id = generateUniqueID(current->data);
            cout << "Data berhasil diupdate!\n";        
            } else {
            cout << "ID " << id << " tidak ditemukan.\n";
        }
    }

    void deleteData(int id) {
        Node* current = head;
        while (current != nullptr && current->data.id != id) {
            current = current->next;
        }

        if (current != nullptr) {
            if (current == head) {
                head = head->next;
                tail->next = head;
                head->prev = tail;
            } else if (current == tail) {
                tail = tail->prev;
                tail->next = head;
                head->prev = tail;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }

            delete current;
            size--;
            cout << "Data supir berhasil dihapus!\n";
            clock_t start_time = clock();
            while ((clock() - start_time) / CLOCKS_PER_SEC < 1) {}
            system("cls");
        } else {
            cout << "ID " << id << " tidak ditemukan.\n";
        }
    }
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
//                                              Awal Fungsi User Menu
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

    // SINGKATAN UNTUK displayUserMenu
    void displayUserMenu(Queue orderQueue) {
        Node* current = head;
        Node* prev = head;  

        while (true) {
            system("cls");
            cout << "Selamat datang, User!" << endl;
            displaySupirInfo(current);

            cout << "1. Next\n2. Previous\n3. Order\n0. Exit\n\n>  ";
            int pilihan;
            cin >> pilihan;

            if (pilihan == 1) {
                moveNext(current);
            } else if (pilihan == 2) {
                movePrevious(prev, current);
            } else if (pilihan == 3) {
                system("cls");
                string nama;
                string tujuan;
                char reOrder;

                do {
                    cout << "Masukkan Nama Anda : ";
                    cin >> nama;
                    cout << "Masukkan Tujuan Anda : ";
                    cin >> tujuan;
                    system("cls");

                    int idSupir;
                    cout << "Daftar Supir: " << endl;

                    Node *currentSupir = head;
                    while (currentSupir != NULL) {
                        cout << "Nama: " << currentSupir->data.nama 
                        << " <-> Id : " << setw(5) << setfill('0') << currentSupir->data.id << endl;
                        currentSupir = currentSupir->next;
                        
                        // Periksa apakah sudah mencapai tail
                        if (currentSupir == tail) {
                            cout << "Id yang dipilih: ";
                            cin >> idSupir;
                            break;  // Hentikan perulangan jika sudah mencapai tail
                        }
                    }
                    // Apabila perulangan selesai tanpa mencapai tail, minta input setelah loop
                    if (currentSupir != tail) {
                        cout << "Id yang dipilih: ";
                        cin >> idSupir;
                    } system("cls");

                    Node* current = head;
                    while(current != NULL && current->data.id != idSupir) {
                    current = current->next;
                    }

                    if(current != NULL) {
                        Supir choosenDriver = current->data;
                                                
                        string plat;
                        double idPemesanan;
                        plat = generatePlatNomor();
                        double angkaHurufPertama = konversiHurufKeAngka(plat[0]);
                        double angkaKe37 = idSupir;
                        int angkaHurufKe8 = konversiHurufKeAngka(tujuan[tujuan.length() - 1]);
                        int angkaHurufKe9 = konversiHurufKeAngka(tujuan[tujuan.length() - 2]);
                        int angkaHurufKe89 = angkaHurufKe8 + angkaHurufKe9;
                        int jumlahNama = 0;
                        for (char c : nama) {
                        jumlahNama += toupper(c) - 'A' + 1;
                        }
                        idPemesanan = (angkaHurufPertama * 100000000) + (angkaKe37 * 1000) + (angkaHurufKe89 * 10) + (jumlahNama % 10);

                        orderQueue.enqueue({
                            idPemesanan,
                            nama,
                            currentSupir->data.nama,
                            plat,
                            tujuan
                        });
                        cout << "Pesanan Berhasil Dilakukan!" << endl;
                        orderQueue.displayQueue();
                    } else {
                        cout << "Supir dengan ID yang dimasukkan tidak ditemukan. Pesanan dibatalkan." << endl;
                    }
                    cout << "\nApakah ingin melakukan order lagi? (y/n): ";
                    cin >> reOrder;
                } while (reOrder == 'y' || reOrder == 'Y');
            } else if (pilihan == 0) {
                system("cls");
                break;
            } else {
                cout << "Pilihan tidak valid. Silakan masukkan pilihan yang benar." << endl;
            }
        }
    }
};
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//                                              Awal Main Fun
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

int main() {
    DoubleLinkedList stack;
    Admin admin;
    Queue orderQueue;

    // Berikut adalah data default, jangan dihapus
    Supir supir1 = {0, "Sugeng Pangestu", "21-02-1999", "Jl. A", 'L'};
    stack.insertNode(supir1);
    Supir supir2 = {0, "Andi Pangestu", "01-02-1999", "Jl. A", 'L'};
    stack.insertNode(supir2);

    system("cls");
    int choice,A,c;
    A = 0;
    while(A != 1) {
    cout << "=============================" << endl << "      Selamat Datang!!    " << endl << "=============================" << endl 
    << "   1. Masuk Sebagai Admin" << endl 
    << "   2. Masuk Sebagai User" << endl 
    << "   3. Exit" << endl << endl 
    << ">  ";
    cin >> choice;

    switch (choice) {
        case 1: {
            system("cls");
            cout << "Masukkan username dan password sebagai Admin" << endl;
            string username, password;
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;

            if (username == admin.username && password == admin.password) {
                system("cls");
                int adminChoice,adEx;
                adEx = 0;
                while(adEx != 1){
                    cout << "Selamat datang, Admin!\n" << endl;

                    // Menu admin
                    cout << "Pilih operasi: "<< endl 
                    << "1. Tambah Data Supir" << endl 
                    << "2. Tampilkan Data Supir" << endl
                    << "3. Update Data Supir" << endl
                    << "4. Hapus Data Supir" << endl
                    << "5. Proses Pesanan" << endl
                    << "0. Exit Menu Admin\n\n>  ";
                    cin >> adminChoice;

                    switch (adminChoice) {
                        case 1: {
                            system("cls");
                            // Menambah data supir
                            Supir newSupir;
                            cout << "Masukkan data supir:\n";
                            cin.ignore();
                            cout << "Nama: ";
                            getline(cin, newSupir.nama);
                            cout << "Tanggal Lahir: ";
                            cin >> newSupir.lahir;
                            cout << "Alamat: ";
                            cin >> newSupir.alamat;
                            cout << "Gender (L/P): ";
                            cin >> newSupir.gender;
                            while (newSupir.gender != 'L' && newSupir.gender != 'P') {
                                cout << "Pilihan tidak valid. Masukkan kembali (L/P): ";
                                cin >> newSupir.gender;
                            }
                            
                            stack.insertNode(newSupir);
                            cout << "Data supir berhasil ditambahkan!\n";
                            system("cls");
                            break;
                        }
                        case 2: {
                            system("cls");
                            int a;
                            stack.displayList();
                            cout << "1. Keluar\n\n>  ";
                            cin >> a;
                            if(a == 1){
                                system("cls");
                                break;
                            }
                        default:
                            cout << "Pilihan tidak valid.\n";
                            break;
                        }
                        case 3: {
                            system("cls");
                            int idToUpdate;
                            cout << "Masukkan ID supir yang ingin diupdate: ";
                            cin >> idToUpdate;
                            stack.updateData(idToUpdate);
                            system("cls");                        
                            break;
                        }
                        case 4: {
                            system("cls");
                            int idToDelete;
                            cout << "Masukkan ID supir yang ingin dihapus: ";
                            cin >> idToDelete;
                            stack.deleteData(idToDelete);
                            system("cls");                      
                            break;
                        }
                        case 5: {
                            system("cls");
                            stack.prosesPesanan(orderQueue);
                        }
                        case 0: {
                            system("cls");
                            adEx = 1;                        
                        }
                    }
                }
            } else {
                system("cls");
                cout << "Username atau password salah. Silahkan coba lagi." << endl;
                system("cls");
            }
            break;
        }
        case 2: {
            system("cls");
            Sleep(1000);

            stack.displayUserMenu(orderQueue);            
            break;
        }
        case 3:{
            cout << "Apakah anda yakin ?" << endl;
            cout << "1. Ya" << endl << "2. Tidak" << endl << endl << ">  ";
            cin >> c;
            system("cls");

            if (c == 1) A = 1;
            break;
        }
    }
    }

    return 0;
}
