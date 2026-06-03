#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;

auto start_time = chrono::steady_clock::now();

typedef struct Node {
  // ID -> data[0]
  // nama -> data[1]
  // layanan -> data[2]
  // prioritas -> data[3]
  // no_antrian -> data[4]
  // waktu_kedatangan -> data[5]
  // status -> data[6]
    string data[7];
    Node* next;
    Node* prev;
} Node;

//struct struct buat hash table
typedef struct HashEntry {
    string key;        
    Node*  value;      
    HashEntry* next;   
} HashEntry;

typedef struct HashTable {
    HashEntry** slot;  
    int kapasitas;     
    int jumlahIsi;     
} HashTable;


// fungsi - fungsi hash table
int hashFunction(string key, int kapasitas) {
    int total = 0;
    for (int i = 0; i < (int)key.length(); i++) {
        total = (total * 31 + (int)key[i]) % kapasitas;
    }
    return total;
}

HashTable* buatHashTable(int kapasitasAwal) {
    HashTable* ht = new HashTable;
    ht->kapasitas  = kapasitasAwal;
    ht->jumlahIsi  = 0;
    ht->slot       = new HashEntry*[kapasitasAwal];
    for (int i = 0; i < kapasitasAwal; i++) {
        ht->slot[i] = nullptr;
    }
    return ht;
}

void insertSaja(HashTable* ht, string key, Node* nodePasien) {
    int index = hashFunction(key, ht->kapasitas); // hitung slot

    HashEntry* entryBaru = new HashEntry;
    entryBaru->key   = key;
    entryBaru->value = nodePasien;
    entryBaru->next  = nullptr;

    if (ht->slot[index] == nullptr) {
        // slot kosong, langsung tempel
        ht->slot[index] = entryBaru;
    } else {
        entryBaru->next = ht->slot[index];
        ht->slot[index] = entryBaru;
    }
    ht->jumlahIsi++;
}

void rehash(HashTable* ht) {
    int kapasitasLama = ht->kapasitas;
    int kapasitasBaru = kapasitasLama * 2;

    cout << "[Info] Rehashing: kapasitas " << kapasitasLama
         << " -> " << kapasitasBaru << endl;

    HashEntry** slotBaru = new HashEntry*[kapasitasBaru];
    for (int i = 0; i < kapasitasBaru; i++) {
        slotBaru[i] = nullptr;
    }

    for (int i = 0; i < kapasitasLama; i++) {
        HashEntry* cur = ht->slot[i];
        while (cur != nullptr) {
            HashEntry* selanjutnya = cur->next;

            int indexBaru = hashFunction(cur->key, kapasitasBaru);

            cur->next = slotBaru[indexBaru];
            slotBaru[indexBaru] = cur;

            cur = selanjutnya; 
        }
    }

    delete[] ht->slot;
    ht->slot      = slotBaru; 
    ht->kapasitas = kapasitasBaru;
}

void htInsert(HashTable* ht, string key, Node* nodePasien) {
    // cek apakah isi sudah 75% untuk rehashing
    if (ht->jumlahIsi * 4 >= ht->kapasitas * 3) {
        rehash(ht);
    }
    insertSaja(ht, key, nodePasien);
}

Node* htCari(HashTable* ht, string key) {
    int index = hashFunction(key, ht->kapasitas);

    HashEntry* cur = ht->slot[index];
    while (cur != nullptr) {
        if (cur->key == key) {
            return cur->value; 
        }
        cur = cur->next; 
    }

    return nullptr; 
}

void htHapus(HashTable* ht, string key) {
    int index = hashFunction(key, ht->kapasitas);

    HashEntry* cur  = ht->slot[index];
    HashEntry* prev = nullptr;

    while (cur != nullptr) {
        if (cur->key == key) {
            if (prev == nullptr) {
                ht->slot[index] = cur->next; 
            } else {
                prev->next = cur->next;
            }
            delete cur;
            ht->jumlahIsi--;
            return;
        }
        prev = cur;
        cur  = cur->next;
    }
}

Node* newNode(string data[]) {
    Node* tmp = new Node;
    tmp->next = tmp->prev = nullptr;
    for (int i = 0; i < 7; i++)
        tmp->data[i] = data[i];
    return tmp;
}

Node* insert(Node* head, string data[]) {
    Node* now = newNode(data);
    now->prev = head;
    now->next = head->next;
    if (head->next != nullptr)
        head->next->prev = now;
    head->next = now;
    return now;
}

Node* getAntrian(Node* front) {
    while (front != nullptr && front->data[0] == "") {
        front = front->next;
    }
    return front;
}

void printNode(Node* pointer) {
    cout << "ID           : " << pointer->data[0] << endl;
    cout << "Nama         : " << pointer->data[1] << endl;
    cout << "Layanan      : " << pointer->data[2] << endl;
    cout << "Prioritas    : " << pointer->data[3] << endl;
    cout << "No. Antrian  : " << pointer->data[4] << endl;
    cout << "Waktu Datang : " << pointer->data[5] << endl;
    cout << "Status       : " << pointer->data[6] << endl;
}

void cariData(HashTable* ht, string id) {
    Node* hasil = htCari(ht, id);
    if (hasil == nullptr) {
        cout << "Data dengan id : " << id << " tidak ditemukan." << endl;
    } else {
        cout << "Data dengan id " << id << " ditemukan :\n";
        printNode(hasil);
    }
}

bool cekDuplikat(Node* front, string data[]) {
    bool dupId = false, dupNama = false, dupNoAntrian = false;
    Node* cur = front;
    while (cur != nullptr) {
        if (cur->data[0] != "" && cur->data[0] == data[0]) dupId = true;
        if (cur->data[1] != "" && cur->data[1] == data[1]) dupNama = true;
        if (cur->data[4] != "" && cur->data[4] == data[4]) dupNoAntrian = true;
        cur = cur->next;
    }
    if (!dupId && !dupNama && !dupNoAntrian) return false;

    string fields[3];
    int count = 0;
    if (dupId)        fields[count++] = "ID";
    if (dupNama)      fields[count++] = "Nama";
    if (dupNoAntrian) fields[count++] = "No Antrian";

    string msg = "Data ditolak! ";
    for (int i = 0; i < count; i++) {
        if (i > 0 && i == count - 1) msg += " dan ";
        else if (i > 0) msg += ", ";
        msg += fields[i];
    }
    msg += " sudah ada di antrian";
    cout << msg << endl;
    return true;
}


int main() {
    Node *head_darurat, *head_lansia, *head_umum, *front;
    string input[] = {"", "", "", "", "", "", ""};
    input[3] = "1";
    head_darurat = newNode(input);
    input[3] = "2";
    head_lansia  = newNode(input);
    input[3] = "3";
    head_umum    = newNode(input);
    // rear = newNode(tmp);
    head_darurat->next = head_lansia;
    head_darurat->prev = nullptr;
    head_lansia->prev  = head_darurat;
    head_lansia->next  = head_umum;
    head_umum->prev    = head_lansia;
    head_umum->next    = nullptr;

    front = head_darurat;

    //buat ukuran hash table awal dengan bilangan prima
    HashTable* tabelPasien = buatHashTable(11);

    Node* current = nullptr;

    while (true) {
        int tipe;
        cout << "\nRECEPTIONIST RUMAH SAKIT\n"
             << "1. Tambah antrian\n"
             << "2. Panggil antrian berikutnya\n"
             << "3. Cari data\n"
             << "4. Update data pasien saat ini\n"
             << "5. Load data dummy dari file\n"
             << "9. Keluar\n"
             << "Pilih : ";
        cin >> tipe;

        if (tipe == 9) break;

        switch (tipe) {

        case 1:
            cout << "Tambah Antrian Berdasarkan Prioritas\n"
                 << "1. Tambah Pasien Darurat\n"
                 << "2. Tambah Pasien Lansia\n"
                 << "3. Tambah Pasien Umum\n"
                 << "Pilih : ";
            cin >> tipe;
            cout << "Format data : {ID} {Nama} {Jenis Layanan} {Nomor Antrian} {Waktu Kedatangan}\n"
                 << "Masukkan data : ";
            input[6] = "Dalam Antrian";

            switch (tipe) {
            case 1:
                cin >> input[0] >> input[1] >> input[2] >> input[4] >> input[5];
                if (cekDuplikat(front, input)) break;
                input[3] = "1";
                insert(head_darurat, input);
                htInsert(tabelPasien, input[0], head_darurat->next);
                break;

            case 2:
                cin >> input[0] >> input[1] >> input[2] >> input[4] >> input[5];
                if (cekDuplikat(front, input)) break;
                input[3] = "2";
                insert(head_lansia, input);
                htInsert(tabelPasien, input[0], head_lansia->next);
                break;

            case 3:
                cin >> input[0] >> input[1] >> input[2] >> input[4] >> input[5];
                if (cekDuplikat(front, input)) break;
                input[3] = "3";
                insert(head_umum, input);
                htInsert(tabelPasien, input[0], head_umum->next);
                break;

            default:
                break;
            }
            break;

        case 2:
            current = getAntrian(front);
            if (current == nullptr) {
                cout << "Antrian kosong!" << endl;
                break;
            }
            cout << "Data pasien berikutnya :\n";
            printNode(current);
            break;

        case 3:
            cout << "Masukkan ID dari pasien : ";
            cin >> input[0];
            cariData(tabelPasien, input[0]); // pakai hash table
            break;

        case 4:
            if (current == nullptr) {
                cout << "Panggil dahulu antrian berikutnya!" << endl;
                break;
            }
            cout << "Data ID    : " << current->data[0]
                 << "\nNama       : " << current->data[1]
                 << "\nStatus     : " << current->data[6] << endl;
            cout << "Status akan diubah menjadi :\n"
                 << "1. Dalam Antrian\n"
                 << "2. Ditangani\n"
                 << "3. Selesai (otomatis terhapus)\n"
                 << "Pilih : ";
            cin >> tipe;

            switch (tipe) {
            case 1:
                current->data[6] = "Dalam Antrian";
                break;
            case 2:
                current->data[6] = "Ditangani";
                break;
            case 3:
                current->data[6] = "Selesai";
                // hapus dari hash table dulu
                htHapus(tabelPasien, current->data[0]);
                // hapus dari linked list
                current->prev->next = current->next;
                if (current->next != nullptr)
                    current->next->prev = current->prev;
                delete current;  // bebaskan memori
                current = nullptr;
                break;
            default:
                break;
            }
            break;

        // load data dummy dari file 
        case 5: {
            string filename;
            cout << "Masukkan nama file dummy: ";
            cin >> filename;
            ifstream infile(filename);
            if (!infile.is_open()) {
                cout << "Gagal membuka file!" << endl;
                break;
            }
            string id, nama, no_antrian;
            int count = 0;
            while (infile >> id >> nama >> no_antrian) {
                string dataFile[] = {id, nama, "", "", no_antrian, "08.00", "Dalam Antrian"};
                char cat = id[0];
                dataFile[3] = cat;  // prioritas dari karakter pertama ID

                if (cat == '1') {
                    dataFile[2] = "IGD";
                    insert(head_darurat, dataFile);
                    htInsert(tabelPasien, dataFile[0], head_darurat->next);
                } else if (cat == '2') {
                    dataFile[2] = "Bedah";
                    insert(head_lansia, dataFile);
                    htInsert(tabelPasien, dataFile[0], head_lansia->next);
                } else {
                    dataFile[2] = "Poliklinik";
                    insert(head_umum, dataFile);
                    htInsert(tabelPasien, dataFile[0], head_umum->next);
                }
                count++;
            }
            infile.close();
            cout << "Berhasil memuat " << count << " data dari file." << endl;
            cout << "[Info] Hash table sekarang berisi " << tabelPasien->jumlahIsi
                 << " entry, kapasitas " << tabelPasien->kapasitas << endl;
            break;
        }

        } // end switch
        cout << endl;
    } // end while


    cout << "\nWaktu total : ";
    auto end_time = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << duration.count() << " ms" << endl;

    return 0;
}