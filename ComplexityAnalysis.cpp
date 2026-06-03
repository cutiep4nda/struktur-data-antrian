#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

struct Patient {
    // 0: ID, 1: Nama, 2: Jenis Layanan, 3: Prioritas,
    // 4: Nomor Antrian, 5: Waktu Kedatangan, 6: Status
    string data[7];
};

struct LinkedNode {
    Patient patient;
    LinkedNode *next;
    LinkedNode *prev;
};

Patient makePatient(const string &id, const string &nama, const string &noAntrian) {
    Patient patient;
    patient.data[0] = id;
    patient.data[1] = nama;
    patient.data[3] = id.empty() ? "3" : string(1, id[0]);
    patient.data[4] = noAntrian;
    patient.data[5] = "08.00";
    patient.data[6] = "Dalam Antrian";

    if (patient.data[3] == "1") {
        patient.data[2] = "IGD";
    } else if (patient.data[3] == "2") {
        patient.data[2] = "Bedah";
    } else {
        patient.data[2] = "Poliklinik";
        patient.data[3] = "3";
    }

    return patient;
}

size_t stringStorageBytes(const Patient &patient) {
    size_t bytes = 0;
    for (int i = 0; i < 7; i++) {
        bytes += patient.data[i].capacity() + 1;
    }
    return bytes;
}

class DoublyLinkedQueue {
private:
    LinkedNode *headDarurat;
    LinkedNode *headLansia;
    LinkedNode *headUmum;
    LinkedNode *front;
    size_t count;

    LinkedNode *createNode(const Patient &patient) {
        LinkedNode *node = new LinkedNode;
        node->patient = patient;
        node->next = nullptr;
        node->prev = nullptr;
        return node;
    }

    void insertAfter(LinkedNode *head, const Patient &patient) {
        LinkedNode *node = createNode(patient);
        node->prev = head;
        node->next = head->next;
        if (head->next != nullptr) {
            head->next->prev = node;
        }
        head->next = node;
        count++;
    }

public:
    DoublyLinkedQueue() : count(0) {
        Patient dummy;
        dummy.data[0] = "";
        dummy.data[1] = "";
        dummy.data[2] = "";
        dummy.data[4] = "";
        dummy.data[5] = "";
        dummy.data[6] = "";

        dummy.data[3] = "1";
        headDarurat = createNode(dummy);
        dummy.data[3] = "2";
        headLansia = createNode(dummy);
        dummy.data[3] = "3";
        headUmum = createNode(dummy);

        headDarurat->next = headLansia;
        headLansia->prev = headDarurat;
        headLansia->next = headUmum;
        headUmum->prev = headLansia;
        front = headDarurat;
    }

    ~DoublyLinkedQueue() {
        clear();
        delete headDarurat;
        delete headLansia;
        delete headUmum;
    }

    void clear() {
        LinkedNode *cur = front;
        while (cur != nullptr) {
            LinkedNode *next = cur->next;
            if (cur != headDarurat && cur != headLansia && cur != headUmum) {
                delete cur;
            }
            cur = next;
        }
        headDarurat->next = headLansia;
        headDarurat->prev = nullptr;
        headLansia->prev = headDarurat;
        headLansia->next = headUmum;
        headUmum->prev = headLansia;
        headUmum->next = nullptr;
        front = headDarurat;
        count = 0;
    }

    void insert(const Patient &patient) {
        if (patient.data[3] == "1") {
            insertAfter(headDarurat, patient);
        } else if (patient.data[3] == "2") {
            insertAfter(headLansia, patient);
        } else {
            insertAfter(headUmum, patient);
        }
    }

    LinkedNode *search(const string &id) const {
        LinkedNode *cur = front;
        while (cur != nullptr) {
            if (cur->patient.data[0] == id) {
                return cur;
            }
            cur = cur->next;
        }
        return nullptr;
    }

    bool update(const string &id, const string &status) {
        LinkedNode *node = search(id);
        if (node == nullptr) {
            return false;
        }
        node->patient.data[6] = status;
        return true;
    }

    bool remove(const string &id) {
        LinkedNode *node = search(id);
        if (node == nullptr || node == headDarurat || node == headLansia || node == headUmum) {
            return false;
        }
        node->prev->next = node->next;
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        }
        delete node;
        count--;
        return true;
    }

    size_t memoryUsageBytes() const {
        size_t bytes = 3 * sizeof(LinkedNode);
        LinkedNode *cur = front;
        while (cur != nullptr) {
            if (cur != headDarurat && cur != headLansia && cur != headUmum) {
                bytes += sizeof(LinkedNode);
                bytes += stringStorageBytes(cur->patient);
            }
            cur = cur->next;
        }
        return bytes;
    }
};

class VectorQueue {
private:
    vector<Patient> queue;

public:
    void reserve(size_t size) {
        queue.reserve(size);
    }

    void insert(const Patient &patient) {
        queue.push_back(patient);
    }

    Patient *search(const string &id) {
        for (size_t i = 0; i < queue.size(); i++) {
            if (queue[i].data[0] == id) {
                return &queue[i];
            }
        }
        return nullptr;
    }

    bool update(const string &id, const string &status) {
        Patient *patient = search(id);
        if (patient == nullptr) {
            return false;
        }
        patient->data[6] = status;
        return true;
    }

    bool remove(const string &id) {
        for (size_t i = 0; i < queue.size(); i++) {
            if (queue[i].data[0] == id) {
                queue.erase(queue.begin() + static_cast<long long>(i));
                return true;
            }
        }
        return false;
    }

    size_t memoryUsageBytes() const {
        size_t bytes = sizeof(queue) + (queue.capacity() * sizeof(Patient));
        for (size_t i = 0; i < queue.size(); i++) {
            bytes += stringStorageBytes(queue[i]);
        }
        return bytes;
    }
};

struct BenchmarkResult {
    long long insertUs;
    long long searchUs;
    long long updateUs;
    long long deleteUs;
    size_t memoryBytes;
};

template <typename Function>
long long measureMicroseconds(Function function) {
    auto start = chrono::high_resolution_clock::now();
    function();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

vector<Patient> loadDataset(size_t limit) {
    string filename = limit <= 1000 ? "dummy_1000.txt" : "dummy_10000.txt";
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Gagal membuka file dataset: " + filename);
    }

    vector<Patient> data;
    data.reserve(limit);
    string id, nama, noAntrian;
    while (data.size() < limit && file >> id >> nama >> noAntrian) {
        data.push_back(makePatient(id, nama, noAntrian));
    }

    if (data.size() < limit) {
        throw runtime_error("Jumlah data pada " + filename + " kurang dari kebutuhan benchmark.");
    }
    return data;
}

BenchmarkResult benchmarkLinkedList(const vector<Patient> &dataset) {
    BenchmarkResult result{};
    DoublyLinkedQueue insertQueue;

    result.insertUs = measureMicroseconds([&]() {
        for (size_t i = 0; i < dataset.size(); i++) {
            insertQueue.insert(dataset[i]);
        }
    });
    result.memoryBytes = insertQueue.memoryUsageBytes();

    DoublyLinkedQueue searchQueue;
    DoublyLinkedQueue updateQueue;
    DoublyLinkedQueue deleteQueue;
    for (size_t i = 0; i < dataset.size(); i++) {
        searchQueue.insert(dataset[i]);
        updateQueue.insert(dataset[i]);
        deleteQueue.insert(dataset[i]);
    }

    volatile size_t successCount = 0;
    result.searchUs = measureMicroseconds([&]() {
        for (size_t i = 0; i < dataset.size(); i++) {
            if (searchQueue.search(dataset[i].data[0]) != nullptr) {
                successCount++;
            }
        }
    });
    result.updateUs = measureMicroseconds([&]() {
        for (size_t i = 0; i < dataset.size(); i++) {
            if (updateQueue.update(dataset[i].data[0], "Ditangani")) {
                successCount++;
            }
        }
    });
    result.deleteUs = measureMicroseconds([&]() {
        for (size_t i = 0; i < dataset.size(); i++) {
            if (deleteQueue.remove(dataset[i].data[0])) {
                successCount++;
            }
        }
    });

    return result;
}

BenchmarkResult benchmarkVector(const vector<Patient> &dataset) {
    BenchmarkResult result{};
    VectorQueue insertQueue;
    insertQueue.reserve(dataset.size());

    result.insertUs = measureMicroseconds([&]() {
        for (size_t i = 0; i < dataset.size(); i++) {
            insertQueue.insert(dataset[i]);
        }
    });
    result.memoryBytes = insertQueue.memoryUsageBytes();

    VectorQueue searchQueue;
    VectorQueue updateQueue;
    VectorQueue deleteQueue;
    searchQueue.reserve(dataset.size());
    updateQueue.reserve(dataset.size());
    deleteQueue.reserve(dataset.size());
    for (size_t i = 0; i < dataset.size(); i++) {
        searchQueue.insert(dataset[i]);
        updateQueue.insert(dataset[i]);
        deleteQueue.insert(dataset[i]);
    }

    volatile size_t successCount = 0;
    result.searchUs = measureMicroseconds([&]() {
        for (size_t i = 0; i < dataset.size(); i++) {
            if (searchQueue.search(dataset[i].data[0]) != nullptr) {
                successCount++;
            }
        }
    });
    result.updateUs = measureMicroseconds([&]() {
        for (size_t i = 0; i < dataset.size(); i++) {
            if (updateQueue.update(dataset[i].data[0], "Ditangani")) {
                successCount++;
            }
        }
    });
    result.deleteUs = measureMicroseconds([&]() {
        for (size_t i = 0; i < dataset.size(); i++) {
            if (deleteQueue.remove(dataset[i].data[0])) {
                successCount++;
            }
        }
    });

    return result;
}

double bytesToKilobytes(size_t bytes) {
    return static_cast<double>(bytes) / 1024.0;
}

void printHeader() {
    cout << "COMPLEXITY ANALYSIS - Doubly Linked List vs Dynamic Array (Vector)\n";
    cout << "Operasi insert, search, update, dan delete diukur terpisah dengan std::chrono.\n";
    cout << "Search, update, dan delete dijalankan untuk seluruh ID dataset agar hasil lebih stabil.\n\n";
}

void printResultRow(size_t datasetSize, const string &structure, const BenchmarkResult &result) {
    cout << left << setw(10) << datasetSize
         << setw(22) << structure
         << right << setw(14) << result.insertUs
         << setw(14) << result.searchUs
         << setw(14) << result.updateUs
         << setw(14) << result.deleteUs
         << setw(16) << fixed << setprecision(2) << bytesToKilobytes(result.memoryBytes)
         << '\n';
}

int main() {
    try {
        printHeader();
        cout << left << setw(10) << "Dataset"
             << setw(22) << "Struktur Data"
             << right << setw(14) << "Insert(us)"
             << setw(14) << "Search(us)"
             << setw(14) << "Update(us)"
             << setw(14) << "Delete(us)"
             << setw(16) << "Memori(KB)" << '\n';
        cout << string(104, '-') << '\n';

        vector<size_t> datasetSizes = {100, 1000, 10000};
        for (size_t i = 0; i < datasetSizes.size(); i++) {
            vector<Patient> dataset = loadDataset(datasetSizes[i]);
            BenchmarkResult linkedListResult = benchmarkLinkedList(dataset);
            BenchmarkResult vectorResult = benchmarkVector(dataset);

            printResultRow(datasetSizes[i], "Doubly Linked List", linkedListResult);
            printResultRow(datasetSizes[i], "Dynamic Array", vectorResult);
            cout << string(104, '-') << '\n';
        }

        cout << "\nCatatan Kompleksitas Teoretis:\n";
        cout << "- Insert linked list: O(1) per data karena masuk setelah head prioritas.\n";
        cout << "- Insert vector: amortized O(1) per data dengan push_back.\n";
        cout << "- Search: O(n) pada kedua struktur karena pencarian berdasarkan ID dilakukan linear.\n";
        cout << "- Update berdasarkan ID: O(n) pada kedua struktur karena perlu mencari ID lebih dulu.\n";
        cout << "- Delete berdasarkan ID: linked list O(n) untuk pencarian + O(1) unlink, vector O(n) untuk pencarian + O(n) shifting.\n";
    } catch (const exception &error) {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
