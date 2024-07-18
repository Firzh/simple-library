#include <iostream>
#include <string>

using namespace std;
const string PI = "31415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549308196442881097566593344612847564823378678316527120190914564856692346034861045432664821339360726024914127372458700663155881748815209209628292540919171536436789259036001133053054882046652138414695194151160943305727036575959195309218611738193261179310511854807446237996274956735188575272489122793818301194912983367336244065664308602139494639522473719070217986094370277053921717629317678765195982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989";

unsigned long long hashFunction(const string& input) {
    unsigned long long hash = 0;
    int pi_length = PI.length();

    for (char c : input) {
        int pos = static_cast<int>(c) % pi_length; // Temukan posisi dalam deret Pi
        hash = (hash * 10 + (PI[pos] - '0')) % 1000000007; // Gabungkan nilai hash, modulo dengan bilangan prima besar untuk menghindari overflow
    }

    return hash;
}

bool cekHash(const string& name, const string& pass, const unsigned long long hash_name, const unsigned long long hash_pass) {
    return hashFunction(name) == hash_name && hashFunction(pass) == hash_pass;
}

int main() {
    string name, pass;
    cout << "Masukkan string untuk di-hash (name): ";
    cin >> name;
    cout << "Masukkan string untuk di-hash (pass): ";
    cin >> pass;

    unsigned long long hashNameValue = hashFunction(name);
    cout << "Nilai hash name: " << hashNameValue << endl;
    unsigned long long hashPassValue = hashFunction(pass);
    cout << "Nilai hash pass: " << hashPassValue << endl;

    // Cek hash
    if (cekHash(name, pass, hashNameValue, hashPassValue)) {
        cout << "Hash valid!" << endl;
    } else {
        cout << "Hash tidak valid!" << endl;
    }

    return 0;
}
