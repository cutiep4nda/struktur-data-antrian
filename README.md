<div align="center">

<pre>
  █████╗ ███╗   ██╗████████╗██████╗ ███████╗ █████╗ ███╗   ██╗
 ██╔══██╗████╗  ██║╚══██╔══╝██╔══██╗██╔════╝██╔══██╗████╗  ██║
 ███████║██╔██╗ ██║   ██║   ██████╔╝█████╗  ███████║██╔██╗ ██║
 ██╔══██║██║╚██╗██║   ██║   ██╔══██╗██╔══╝  ██╔══██║██║╚██╗██║
 ██║  ██║██║ ╚████║   ██║   ██║  ██║███████╗██║  ██║██║ ╚████║
 ╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝
</pre>

### Hospital Priority Queue Simulation Engine
### Built on Double Linked List & Vector Structures

[![C++ Version](https://img.shields.io/badge/C%2B%2B-17%2B-%2300599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Compiler](https://img.shields.io/badge/Compiler-GCC_MinGW-df0078?style=for-the-badge&logo=gnu&logoColor=white)](#)
[![Dataset](https://img.shields.io/badge/Dataset-Up_to_100k_Rows-%237b2cbf?style=for-the-badge)](#)
[![Build](https://img.shields.io/badge/Build-✅_Passing-27ae60?style=for-the-badge)](#)
[![License: MIT](https://img.shields.io/badge/License-MIT-f39c12?style=for-the-badge)](https://opensource.org/licenses/MIT)

**Priority-Based Patient Queues, Optimized.**  
Simulasi antrean rumah sakit berbasis prioritas (Darurat, Lansia, Umum) yang membandingkan performa antara struktur data **Double Linked List** dan **Vector** menggunakan data dummy hingga **100.000** data unik.

[Quick Start](#-quick-start) · [Architecture](#-architecture) · [Data Structures](#-data-structures) · [Menu & API](#-menu--api-reference) · [Comparison](#%EF%B8%8F-comparison)

</div>

---

## 📖 Overview

**Antrean Pasien** adalah sistem simulasi manajemen antrean resepsionis rumah sakit. Sistem ini dirancang untuk menangani tiga kategori prioritas pelayanan secara otomatis:

| Prioritas | Jenis Layanan | Kode Kategori |
|-----------|---------------|:-------------:|
| 🚨 **Darurat (Emergency)** | Pelayanan IGD (Instalasi Gawat Darurat) | `1` |
| 🧓 **Lansia (Elderly)** | Pelayanan Bedah / Spesialistik Lansia | `2` |
| 🧑 **Umum (General)** | Pelayanan Poliklinik Umum | `3` |

Setiap pasien memiliki ID unik sepanjang 10 digit yang mengikuti pola: `[Kategori (1 digit)] [Tanggal DDMMYY (6 digit)] [Urutan (3 digit)]`. Sistem ini membandingkan dua pendekatan pengelolaan antrean, yaitu linked list tersegmentasi prioritas dan array dinamis (vector).

---

## 🚀 Quick Start

Anda dapat dengan cepat menghasilkan data dummy dan menjalankan pengujian menggunakan perintah berikut:

### 1. Buat Data Dummy
Kompilasi dan jalankan generator untuk membuat file `dummy_1000.txt`, `dummy_10000.txt`, dan `dummy_100000.txt`:
```bash
g++ -std=c++17 generator.cpp -o generator
./generator
```

### 2. Jalankan Implementasi Double Linked List
```bash
g++ -std=c++17 double-linked-list.cpp -o double-linked-list
./double-linked-list
```

### 3. Jalankan Implementasi Vector
```bash
g++ -std=c++17 kode-antrean-vector.cpp -o kode-antrean-vector
./kode-antrean-vector
```

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                          RECEPTIONIST SYSTEM                        │
│                                                                     │
│    ┌───────────────┐              ┌──────────────────────────┐      │
│    │  File Dummy   │─────────────▶│  Pilihan Menu 5: Load    │      │
│    │  (.txt)       │              │  (Melewati Cek Duplikat) │      │
│    └───────────────┘              └────────────┬─────────────┘      │
│                                                │                    │
│                        ┌───────────────────────┴───────────────┐    │
│                        ▼                                       ▼    │
│          ┌──────────────────────────┐            ┌──────────────────────────┐
│          │  Double Linked List      │            │  STD Vector              │
│          │  (Priority Segments)     │            │  (Flat Dynamic Array)    │
│          │ ──────────────────────── │            │ ──────────────────────── │
│          │  IGD Dummy Head (Prio 1) │            │                          │
│          │            │             │            │  [Pasien 1, Pasien 2,    │
│          │            ▼             │            │   Pasien 3, ...]         │
│          │  Bedah Dummy Head(Prio 2)│            │                          │
│          │            │             │            │  Pencarian sekuensial    │
│          │            ▼             │            │  3 kali untuk melayani   │
│          │  Poli Dummy Head (Prio 3)│            │  prio 1 -> 2 -> 3        │
│          └──────────────────────────┘            └──────────────────────────┘
└─────────────────────────────────────────────────────────────────────┘
```

---

## 💾 Data Structures

Kedua implementasi menyimpan detail pasien di dalam array string berukuran 7 kolom (`data[7]`):

| Indeks | Atribut | Deskripsi | Contoh |
|:---:|---|---|---|
| **`[0]`** | **ID** | 10 digit kode pengenal unik | `1030626001` |
| **`[1]`** | **Nama** | Nama pasien (spasi diganti `_`) | `Abi_Saputra` |
| **`[2]`** | **Layanan** | Instalasi tujuan (`IGD`, `Bedah`, `Poliklinik`) | `IGD` |
| **`[3]`** | **Prioritas** | Karakter tingkat kedaruratan (`1`, `2`, `3`) | `1` |
| **`[4]`** | **No Antrian** | Nomor urutan pendaftaran | `001` |
| **`[5]`** | **Waktu** | Jam kedatangan pasien | `08.00` |
| **`[6]`** | **Status** | Keadaan saat ini (`Dalam Antrian`, `Ditangani`) | `Dalam Antrian` |

---

## 📋 Menu & API Reference

Setiap program antrean menyediakan menu interaktif berikut:

### `1. Tambah antrian`
Menambahkan pasien secara manual. Meminta input: ID, Nama, Jenis Layanan, Nomor Antrian, dan Waktu Kedatangan. Pengecekan duplikat otomatis diaktifkan untuk pendaftaran manual.

### `2. Panggil antrian berikutnya`
Memanggil pasien dengan prioritas tertinggi untuk ditangani. Pasien prioritas `1` dilayani terlebih dahulu, diikuti `2`, lalu `3`.

### `3. Cari data`
Mencari informasi pasien di dalam antrean menggunakan ID Pasien secara efisien.

### `4. Update data pasien saat ini`
Mengubah status penanganan pasien yang sedang dipanggil (`Dalam Antrian` $\rightarrow$ `Ditangani` $\rightarrow$ `Selesai`). Mengubah status menjadi selesai akan menghapusnya secara permanen dari struktur data.

### `5. Load data dummy dari file`
Membaca data dummy dari file eksternal (seperti `dummy_1000.txt`) secara otomatis. Fitur ini melewati pemindaian duplikat (`cekDuplikat`) agar pemuatan volume besar (100.000 data) berjalan instan dalam hitungan milidetik ($O(N)$).

---

## ⚡ Comparison

| Fitur / Karakteristik | Double Linked List (`double-linked-list.cpp`) | Vector (`kode-antrean-vector.cpp`) |
|-----------------------|-----------------------------------------------|------------------------------------|
| **Struktur Penyimpanan** | Node yang saling terhubung dengan pointer `next` dan `prev` | Elemen berurutan secara kontigu di memori (`std::vector`) |
| **Penyisipan Data (Insert)** | Sangat cepat ($O(1)$) karena disisipkan langsung setelah dummy head kategorinya. | Sangat cepat ($O(1)$) karena langsung dimasukkan ke belakang vector dengan `push_back`. |
| **Pemanggilan Antrean** | Instan ($O(1)$) karena tinggal menunjuk ke elemen terdepan list yang valid. | Lebih lambat ($O(N)$) karena harus melakukan scan linear 3 kali untuk mencari prioritas `1`, `2`, lalu `3`. |
| **Penghapusan Node** | Cepat ($O(1)$) karena hanya perlu memutus/mengubah pointer tetangganya. | Lambat ($O(N)$) karena menghapus di tengah/depan memerlukan pergeseran memori elemen di belakangnya. |

---

## 📁 Repository Structure

```
struktur-data-antrian/
├── .vscode/
│   └── c_cpp_properties.json   # Konfigurasi compiler VS Code
├── README.md                   # File dokumentasi ini
├── double-linked-list.cpp      # Implementasi Queue berbasis Double Linked List
├── kode-antrean-vector.cpp     # Implementasi Queue berbasis STD Vector
├── generator.cpp               # Generator dataset dummy pasien
├── input.txt                   # Contoh simulasi input manual
├── dummy_1000.txt              # Dataset dummy 1.000 data
├── dummy_10000.txt             # Dataset dummy 10.000 data
└── dummy_100000.txt            # Dataset dummy 100.000 data
```
