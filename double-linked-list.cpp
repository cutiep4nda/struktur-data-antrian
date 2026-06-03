#include <chrono>
#include <iostream>

using namespace std;

auto start = chrono::steady_clock::now();

typedef struct Node {
  // ID -> data[0]
  // nama -> data[1]
  // layanan -> data[2]
  // prioritas -> data[3]
  // no_antrian -> data[4]
  // waktu_kedatangan -> data[5]
  // status -> data[6]
  string data[7];
  Node *next;
  Node *prev;
} Node;

Node *newNode(string data[]) {
  Node *tmp = new Node;
  tmp->next = tmp->prev = nullptr;
  for (int i = 0; i < 7; i++)
    tmp->data[i] = data[i];
  return tmp;
}

Node *insert(Node *head, string data[]) {
  Node *now = newNode(data);
  now->prev = head;
  now->next = head->next;
  if (head->next != nullptr)
    head->next->prev = now;
  head->next = now;
  return now;
}

Node *getAntrian(Node *front) {
  while (front != nullptr && front->data[0] == "") {
    front = front->next;
  }
  return front;
}

void printNode(Node *pointer) {
  for (int i = 0; i < 7; i++)
    cout << pointer->data[i] << " ";
  cout << endl;
}

void cariData(Node *front, string id) {
  Node *cur = front;
  while (cur != nullptr) {
    if (cur->data[0] == id)
      break;
    cur = cur->next;
  }
  if (cur == nullptr)
    cout << "Data dengan id : " << id << " tidak ditemukan." << endl;
  else {
    cout << "Data dengan id " << id << " ditemukan : \n";
    printNode(cur);
  }
}

bool cekDuplikat(Node *front, string data[]) {
    bool dupId = false, dupNama = false, dupNoAntrian = false;
    Node *cur = front;
    while (cur != nullptr) {
        if (cur->data[0] != "" && cur->data[0] == data[0]) dupId = true;
        if (cur->data[1] != "" && cur->data[1] == data[1]) dupNama = true;
        if (cur->data[4] != "" && cur->data[4] == data[4]) dupNoAntrian = true;
        cur = cur->next;
    }
    if (!dupId && !dupNama && !dupNoAntrian) return false;

    string fields[3];
    int count = 0;
    if (dupId) fields[count++] = "ID";
    if (dupNama) fields[count++] = "Nama";
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
  input[3] = '1';
  head_darurat = newNode(input);
  input[3] = '2';
  head_lansia = newNode(input);
  input[3] = '3';
  head_umum = newNode(input);
  // rear = newNode(tmp);
  head_darurat->next = head_lansia;
  head_darurat->prev = nullptr;
  head_lansia->prev = head_darurat;
  head_lansia->next = head_umum;
  head_umum->prev = head_lansia;
  head_umum->next = nullptr;
  front = head_darurat;
  Node *current = nullptr;
  while (1 < 2) {
    int tipe;
    cout
        << "RECEPTIONIST RUMAH SAKIT\n1. Tambah antrian\n2. Panggil antrian "
           "berikutnya\n3. Cari data\n4. Update data pasien saat ini\nPilih : ";
    cin >> tipe;
    if (tipe == 9)
      break;
    switch (tipe) {
    case 1:
      cout << "Tambah Antrian Berdasarkan Prioritas\n1. Tambah Pasien "
              "Darurat\n2. Tambah Pasien Lansia\n3. Tambah Pasien Umum\nPilih "
              ": ";
      cin >> tipe;
      cout << "Format data : {ID} {Nama} {Jenis Layanan} {Nomor Antrian} "
              "{Waktu Kedatangan}\nMasukkan data : ";
      input[6] = "Dalam Antrian";
      switch (tipe) {
      case 1:
        cin >> input[0] >> input[1] >> input[2] >> input[4] >> input[5];
        if (cekDuplikat(front, input)) break;
        input[3] = '1';
        head_darurat = insert(head_darurat, input);
        break;
      case 2:
        cin >> input[0] >> input[1] >> input[2] >> input[4] >> input[5];
        if (cekDuplikat(front, input)) break;
        input[3] = '2';
        head_lansia = insert(head_lansia, input);
        break;
      case 3:
        cin >> input[0] >> input[1] >> input[2] >> input[4] >> input[5];
        if (cekDuplikat(front, input)) break;
        input[3] = '3';
        head_umum = insert(head_umum, input);
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
      cout << "Data pasien berikutnya : ";
      printNode(current);
      break;

    case 3:
      cout << "Masukkan ID dari pasien : ";
      cin >> input[0];
      cariData(front, input[0]);
      break;
    case 4:
      if (current == nullptr) {
        cout << "Panggil dahulu antrian berikutnya!" << endl;
        break;
      }
      cout << "Data ID :" << current->data[0] << " Nama :" << current->data[1]
           << " Status Layanan : " << current->data[6] << endl;
      cout << "Status layanana akan diubah menjadi :\n1. Dalam Antrian\n2. "
              "Ditangani\n3. Selesai (otomatis akan terhapus)\nPilih : ";
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
        current->prev->next = current->next;
        if (current->next != nullptr)
          current->next->prev = current->prev;
        current = nullptr;
        break;
      default:
        break;
      }
      break;
    }
    cout << endl;
  }
  cout << endl << endl << "Waktu : ";
  auto end = chrono::steady_clock::now();
  auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << duration.count() << " ms" << endl;
}