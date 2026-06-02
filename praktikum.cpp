#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <stdexcept>
#include <vector>
#include <sstream>

using namespace std;

// ==========================================
// 1. STRUKTUR KELAS TOKO ELEKTRONIK (OOP)
// ==========================================
class TokoElektronik {
private:
    // Array berkapasitas 3 elemen (Enkapsulasi etalase toko)
    array<string, 3> etalase;

public:
    // Constructor: Mengisi data awal ke dalam array secara otomatis
    TokoElektronik() {
        etalase[0] = "Laptop ASUS ROG";
        etalase[1] = "Smartphone Samsung S24";
        etalase[2] = "Smart TV LG 43 Inch";
    }

    // Method Public untuk mengambil produk berdasarkan nomor rak
    string ambilProduk(size_t nomorRak) {
        try {
            // Menggunakan .at() untuk validasi batas indeks array
            return etalase.at(nomorRak);
        }
        catch (const out_of_range& e) {
            // 2. MEKANISME EXCEPTION HANDLING
            // Menangkap error bawaan out_of_range dan melemparkan pesan kustom
            throw runtime_error("Gagal Mengambil Barang : Rak nomor [" + to_string(nomorRak) + "] kosong atau tidak tersedia!");
        }
    }
};

// ==========================================
// 3. FITUR MANAJEMEN GUDANG (FILE I/O & CRUD)
// ==========================================
const string FILE_NAME = "gudang.txt";

// Struktur data untuk mempermudah manipulasi barang di memori
struct Barang {
    string id;
    string nama;
    string harga;
};

// Fungsi Helper: Membaca file dan memasukkannya ke dalam vector
vector<Barang> muatDataGudang() {
    vector<Barang> daftarBarang;
    ifstream fileInput(FILE_NAME);
    
    // Jika file belum ada, buat filenya terlebih dahulu
    if (!fileInput.is_open()) {
        ofstream fileBaru(FILE_NAME);
        fileBaru.close();
        return daftarBarang;
    }

    string baris;
    while (getline(fileInput, baris)) {
        stringstream ss(baris);
        string id, nama, harga;
        // Membaca data dengan pemisah koma (CSV sederhana)
        if (getline(ss, id, ',') && getline(ss, nama, ',') && getline(ss, harga, ',')) {
            daftarBarang.push_back({id, nama, harga});
        }
    }
    fileInput.close();
    return daftarBarang;
}

// Fungsi Helper: Menyimpan kembali seluruh data vector ke dalam file
void simpanDataGudang(const vector<Barang>& daftarBarang) {
    ofstream fileOutput(FILE_NAME, ios::trunc); // Overwrite file lama
    if (fileOutput.is_open()) {
        for (const auto& b : daftarBarang) {
            fileOutput << b.id << "," << b.nama << "," << b.harga << "\n";
        }
        fileOutput.close();
    }
}

// [READ] Menampilkan daftar barang dari file .txt
void tampilkanGudang() {
    vector<Barang> daftarBarang = muatDataGudang();
    cout << "\n=============================================\n";
    cout << "         DAFTAR BARANG DI GUDANG (.TXT)       \n";
    cout << "=============================================\n";
    if (daftarBarang.empty()) {
        cout << "[ Gudang Masih Kosong / File Belum Berisi Data ]\n";
    } else {
        cout << "ID\t|\tNama Barang\t\t|\tHarga\n";
        cout << "---------------------------------------------\n";
        for (const auto& b : daftarBarang) {
            cout << b.id << "\t|\t" << b.nama << "\t\t|\tRp. " << b.harga << "\n";
        }
    }
    cout << "=============================================\n";
}

// [CREATE] Menambahkan data barang baru
void tambahBarang() {
    vector<Barang> daftarBarang = muatDataGudang();
    Barang bBaru;
    
    cout << "\n--- Tambah Barang Gudang ---\n";
    cout << "Masukkan ID Barang (cth: B01): ";
    cin >> bBaru.id;
    
    // Validasi apakah ID sudah digunakan
    for (const auto& b : daftarBarang) {
        if (b.id == bBaru.id) {
            cout << "Error: Barang dengan ID tersebut sudah ada!\n";
            return;
        }
    }
    
    cin.ignore(); // Membersihkan buffer
    cout << "Masukkan Nama Barang         : ";
    getline(cin, bBaru.nama);
    cout << "Masukkan Harga Barang        : ";
    getline(cin, bBaru.harga);

    daftarBarang.push_back(bBaru);
    simpanDataGudang(daftarBarang);
    cout << "✔ Barang berhasil ditambahkan ke gudang.txt!\n";
}

// [UPDATE] Memperbarui data barang berdasarkan ID
void updateBarang() {
    vector<Barang> daftarBarang = muatDataGudang();
    string idCari;
    bool ditemukan = false;

    cout << "\n--- Update Barang Gudang ---\n";
    cout << "Masukkan ID Barang yang akan diubah: ";
    cin >> idCari;

    for (auto& b : daftarBarang) {
        if (b.id == idCari) {
            ditemukan = true;
            cin.ignore();
            cout << "Data Lama: " << b.nama << " (Rp. " << b.harga << ")\n";
            cout << "Masukkan Nama Baru : ";
            getline(cin, b.nama);
            cout << "Masukkan Harga Baru: ";
            getline(cin, b.harga);
            break;
        }
    }

    if (ditemukan) {
        simpanDataGudang(daftarBarang);
        cout << "✔ Data barang berhasil diperbarui di gudang.txt!\n";
    } else {
        cout << "❌ Barang dengan ID " << idCari << " tidak ditemukan.\n";
    }
}

// [DELETE] Menghapus data barang berdasarkan ID
void deleteBarang() {
    vector<Barang> daftarBarang = muatDataGudang();
    string idCari;
    bool ditemukan = false;

    cout << "\n--- Hapus Barang Gudang ---\n";
    cout << "Masukkan ID Barang yang akan dihapus: ";
    cin >> idCari;

    for (auto it = daftarBarang.begin(); it != daftarBarang.end(); ++it) {
        if (it->id == idCari) {
            daftarBarang.erase(it);
            ditemukan = true;
            break;
        }
    }

    if (ditemukan) {
        simpanDataGudang(daftarBarang);
        cout << "✔ Barang berhasil dihapus dari gudang.txt!\n";
    } else {
        cout << "❌ Barang dengan ID " << idCari << " tidak ditemukan.\n";
    }
}

// ==========================================
// SIMULASI ETALASE (EXCEPTION HANDLING POIN 2)
// ==========================================
void jalankanSimulasiEtalase(TokoElektronik& toko) {
    cout << "\n=============================================\n";
    cout << "         SIMULASI PENGAMBILAN ETALASE        \n";
    cout << "=============================================\n";

    // SKENARIO 1: Pengambilan barang di rak indeks ke-1 (Harus Sukses)
    cout << "[Skenario 1]: Mengambil barang di rak indeks 1...\n";
    try {
        string barang = toko.ambilProduk(1);
        cout << "-> Sukses! Barang ditemukan: " << barang << "\n";
    } 
    catch (const runtime_error& e) {
        cout << "-> Terjadi Error: " << e.what() << "\n";
    }

    cout << "---------------------------------------------\n";

    // SKENARIO 2: Pengambilan barang di rak indeks ke-5 (Harus Gagal)
    cout << "[Skenario 2]: Mengambil barang di rak indeks 5...\n";
    try {
        string barang = toko.ambilProduk(5);
        cout << "-> Sukses! Barang ditemukan: " << barang << "\n";
    } 
    catch (const runtime_error& e) {
        // Menangkap custom error message yang di-throw oleh method ambilProduk
        cout << "-> Terjadi Error: " << e.what() << "\n";
    }
    cout << "=============================================\n";
}

// ==========================================
// 4. PENGUJIAN PADA FUNGSI MAIN()
// ==========================================
int main() {
    TokoElektronik tokoGibran;
    int pilihanMenu;

    do {
        // Otomatis menjalankan fitur READ (menampilkan isi file) setiap kali menu utama dimuat
        tampilkanGudang();

        cout << "\nSISTEM MANAJEMEN DIGITAL TOKO \"GIBRAN JAYA\"\n";
        cout << "1. Tambah Barang Gudang (Create)\n";
        cout << "2. Update Barang Gudang (Update)\n";
        cout << "3. Hapus Barang Gudang (Delete)\n";
        cout << "4. Jalankan Simulasi Etalase (Exception Handling)\n";
        cout << "5. Keluar Aplikasi\n";
        cout << "Pilih Menu [1-5]: ";
        
        // Validasi input menu agar tidak infinite loop jika user memasukkan karakter/huruf
        if (!(cin >> pilihanMenu)) {
            cout << "Input tidak valid! Masukkan angka.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (pilihanMenu) {
            case 1:
                tambahBarang();
                break;
            case 2:
                updateBarang();
                break;
            case 3:
                deleteBarang();
                break;
            case 4:
                jalankanSimulasiEtalase(tokoGibran);
                break;
            case 5:
                cout << "\nTerima kasih telah menggunakan sistem Toko Gibran Jaya!\n";
                break;
            default:
                cout << "Pilihan tidak tersedia. Silakan coba lagi.\n";
        }
        
        // Pause sederhana sebelum mengulang loop agar user sempat membaca output
        if (pilihanMenu != 5) {
            cout << "\nTekan Enter untuk kembali ke menu utama...";
            cin.ignore(1000, '\n');
            cin.get();
        }

    } while (pilihanMenu != 5);

    return 0;
}