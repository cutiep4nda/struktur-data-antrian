#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
using namespace std;

typedef struct Node
{
    // ID -> data[0]
    // nama -> data[1]
    // layanan -> data[2]
    // prioritas -> data[3]
    // no_antrian -> data[4]
    // waktu_kedatangan -> data[5]
    // status -> data[6]
    string data[7];
    // Node *next;
    // Node *prev;
} Node;

// void insert(Node *po, string *data)
// {
//     for (int i = 0; i < 7; i++)
//     {
//         // cout << data[i] << " ";
//         po->data[i] = data[i];
//         // strcpy(po->data[i], data[i]);
//     }
// }

int getAntrian(vector<Node> &v)
{
    for (int i = 0; i < v.size(); i++)
        if (v[i].data[3] == "1")
            return i;

    for (int i = 0; i < v.size(); i++)
        if (v[i].data[3] == "2")
            return i;

    for (int i = 0; i < v.size(); i++)
        if (v[i].data[3] == "3")
            return i;
    return -1;
}

void printNode(Node &nd)
{
    for (int i = 0; i < 7; i++)
        cout << nd.data[i] << " ";
    cout << endl;
}

void cariData(vector<Node> &v, string id)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i].data[0] == id)
        {
            cout << "Data dengan id " << id << " ditemukan : \n";
            printNode(v[i]);
            return;
        }
    }
    cout << "Data dengan id : " << id << " tidak ditemukan." << endl;
}

bool cekDuplikat(vector<Node> &v, string data[]) {
    bool dupId = false, dupNama = false, dupNoAntrian = false;
    for (int i = 0; i < (int)v.size(); i++) {
        if (v[i].data[0] == data[0]) dupId = true;
        if (v[i].data[1] == data[1]) dupNama = true;
        if (v[i].data[4] == data[4]) dupNoAntrian = true;
    }
    if (!dupId && !dupNama && !dupNoAntrian) return false;

    vector<string> fields;
    if (dupId) fields.push_back("ID");
    if (dupNama) fields.push_back("Nama");
    if (dupNoAntrian) fields.push_back("No Antrian");

    string msg = "Data ditolak! ";
    for (int i = 0; i < (int)fields.size(); i++) {
        if (i > 0 && i == (int)fields.size() - 1) msg += " dan ";
        else if (i > 0) msg += ", ";
        msg += fields[i];
    }
    msg += " sudah ada di antrian";
    cout << msg << endl;
    return true;
}

int main()
{
    vector<Node> queue;
    Node input;

    int current = -1;

    while (1 < 2)
    {
        int tipe;
        cout << "RECEPTIONIST RUMAH SAKIT\n1. Tambah antrian\n2. Panggil antrian berikutnya\n3. Cari data\n4. Update data pasien saat ini\n5. Load data dummy dari file\nPilih : ";
        cin >> tipe;

        if (tipe == 9)
        {
            break;
        }

        switch (tipe)
        {
        case 1:
            cout << "Tambah Antrian Berdasarkan Prioritas\n1. Tambah Pasien Darurat\n2. Tambah Pasien Lansia\n3. Tambah Pasien Umum\nPilih : ";
            cin >> tipe;
            cout << "Format data : {ID} {Nama} {Jenis Layanan} {Nomor Antrian} {Waktu Kedatangan}\nMasukkan data : ";
            input.data[6] = "Dalam Antrian";

            cin >> input.data[0] >> input.data[1] >> input.data[2] >> input.data[4] >> input.data[5];
            if (cekDuplikat(queue, input.data)) break;
            switch (tipe)
            {
            case 1:
                input.data[3] = '1';
                // head_darurat = insert(head_darurat, input);
                break;
            case 2:
                // cin >> input[0] >> input[1] >> input[2] >> input[4] >> input[5];
                input.data[3] = '2';
                // head_lansia = insert(head_lansia, input);
                break;
            case 3:
                // cin >> input[0] >> input[1] >> input[2] >> input[4] >> input[5];
                input.data[3] = '3';
                // head_umum = insert(head_umum, input);
                break;
            default:
                break;
            }

            // for (int i = 0; i < 7; i++)
            //     cout << input.data[i] << " ";
            // cout << endl;

            queue.push_back(input);
            break;
        case 2:
            current = getAntrian(queue);
            if (current == -1)
            {
                cout << "Antrian kosong!" << endl;
                break;
            }
            cout << "Data pasien berikutnya : ";
            printNode(queue[current]);
            break;
        case 3:
        {
            string id;
            cout << "Masukkan ID dari pasien : ";
            cin >> id;
            cariData(queue, id);
            break;
        }
        case 4:
            if (current == -1)
            {
                cout << "Panggil dahulu antrian berikutnya!" << endl;
                break;
            }
            cout << "Data ID :" << queue[current].data[0] << " Nama :" << queue[current].data[1] << " Status Layanan : " << queue[current].data[6] << endl;
            cout << "Status layanana akan diubah menjadi :\n1. Dalam Antrian\n2. Ditangani\n3. Selesai (otomatis akan terhapus)\nPilih : ";
            cin >> tipe;
            switch (tipe)
            {
            case 1:
                queue[current].data[6] = "Dalam Antrian";
                break;
            case 2:
                queue[current].data[6] = "Ditangani";
                break;
            case 3:
                queue[current].data[6] = "Selesai";
                queue.erase(queue.begin() + current);
                current = -1;
                break;
            default:
                break;
            }
            break;
        case 5:
        {
            string filename;
            cout << "Masukkan nama file dummy: ";
            cin >> filename;
            ifstream infile(filename);
            if (!infile.is_open())
            {
                cout << "Gagal membuka file!" << endl;
                break;
            }
            string id, nama, no_antrian;
            int count = 0;
            while (infile >> id >> nama >> no_antrian)
            {
                Node input;
                input.data[0] = id;
                input.data[1] = nama;
                char cat = id[0];
                input.data[3] = cat;
                if (cat == '1')
                {
                    input.data[2] = "IGD";
                }
                else if (cat == '2')
                {
                    input.data[2] = "Bedah";
                }
                else
                {
                    input.data[2] = "Poliklinik";
                }
                input.data[4] = no_antrian;
                input.data[5] = "08.00";
                input.data[6] = "Dalam Antrian";
                
                queue.push_back(input);
                count++;
            }
            infile.close();
            cout << "Berhasil memuat " << count << " data dummy dari file." << endl;
            break;
        }
        default:
            break;
        }
    }

    // for (int i = 0; i < queue.size(); i++)
    // {
    //     printNode(queue[i]);
    // }
}