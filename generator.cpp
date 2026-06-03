#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <cstdlib>

using namespace std;

// list nama depan 
const vector<string> FIRST_NAMES = {
    "Abi", "Abdul", "Arafah", "Budi", "Chika", "Dani", "Eka", "Fadel", 
    "Gita", "Hana", "Indra", "Joko", "Kartika", "Lestari", "Mega", "Naufal", 
    "Olivia", "Putri", "Rian", "Siti", "Taufik", "Utami", "Vera", "Wawan", 
    "Yanto", "Zainal", "Ahmad", "Bagus", "Cahyo", "Dewi", "Elok", "Fitri", 
    "Galih", "Heru", "Ika", "Jatmiko", "Kusuma", "Mulyono", "Nur", "Rudi"
};

// list nama belakang
const vector<string> LAST_NAMES = {
    "Pratama", "Saputra", "Wijaya", "Kusuma", "Lestari", "Hidayat", "Nugroho", 
    "Setiawan", "Wibowo", "Santoso", "Siregar", "Nasution", "Harahap", "Lubis", 
    "Ginting", "Pane", "Tanjung", "Simanjuntak", "Hutapea", "Sinaga", "Siahaan", 
    "Marpaung", "Sitorus", "Pasaribu", "Nababan", "Nainggolan", "Manurung", 
    "Pangaribuan", "Tambunan", "Aritonang", "Bambang", "Supriadi", "Haryanto", 
    "Gunawan", "Susanto", "Hartono", "Kristanto", "Purnama", "Wahyudi", "Budiman"
};

struct CategoryState {
    int day = 3;
    int month = 6;
    int year = 26; // 2026
    int seq = 1;
};

// fungsi untuk menambah hari secara manual dengan memperhatikan jumlah hari per bulan
void nextDate(int &d, int &m, int &y) {
    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
    // tahun kabisat (2026 bukan kabisat, tapi untuk kelengkapan logika)
    if (y % 4 == 0) {
        daysInMonth[2] = 29;
    }
    
    d++;
    if (d > daysInMonth[m]) {
        d = 1;
        m++;
        if (m > 12) {
            m = 1;
            y++;
        }
    }
}

// pebantu untuk format zero-padding
string formatNumber(int val, int width) {
    stringstream ss;
    ss << setw(width) << setfill('0') << val;
    return ss.str();
}

void generateData(const string &filename, int count, unsigned int seed) {
    srand(seed);
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "Gagal membuat file: " << filename << endl;
        return;
    }

    // inisialisasi state tanggal untuk masing-masing kategori
    // indeks 1: Darurat, indeks 2: Lansia, indeks 3: Umum
    CategoryState states[4]; 

    for (int i = 1; i <= count; i++) {
        // pilih kategori acak (1, 2, atau 3)
        int cat = rand() % 3 + 1;
        CategoryState &state = states[cat];

        // jika urutan melebihi 999, reset urutan ke 1 dan tambahkan tanggal 1 hari
        if (state.seq > 999) {
            state.seq = 1;
            nextDate(state.day, state.month, state.year);
        }

        // susun ID pasien: [Kategori (1 digit)] [DDMMYY (6 digit)] [Urutan (3 digit)]
        string id = to_string(cat) + 
                    formatNumber(state.day, 2) + 
                    formatNumber(state.month, 2) + 
                    formatNumber(state.year, 2) + 
                    formatNumber(state.seq, 3);
        
        state.seq++;

        // Pilih nama depan dan belakang secara acak dari bank nama
        string first = FIRST_NAMES[rand() % FIRST_NAMES.size()];
        string last = LAST_NAMES[rand() % LAST_NAMES.size()];
        string name = first + "_" + last;

        // Nomor antrean dibuat unik sequential
        string queueNo = formatNumber(i, 3);

        // Format keluaran: ID NAMA NO_ANTRIAN
        outfile << id << " " << name << " " << queueNo << "\n";
    }

    outfile.close();
    cout << "Berhasil membuat file data dummy (" << count << " data) -> " << filename << endl;
}

int main() {
    cout << "=== PROGRAM GENERATOR DATA DUMMY PASIEN ===" << endl;
    
    // seed angka bebas untuk reproduksibilitas
    unsigned int seed = 42; 
    
    generateData("dummy_1000.txt", 1000, seed);
    generateData("dummy_10000.txt", 10000, seed);
    generateData("dummy_100000.txt", 100000, seed);
    
    cout << "Semua file dummy berhasil dibuat!" << endl;
    return 0;
}
