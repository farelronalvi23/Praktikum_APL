#include <iostream>
using namespace std;

#define MAX_MAHASISWA 100 // Ukuran maksimum array

int main() {

    int panjang = 0; // Jumlah data mahasiswa saat ini
    string mahasiswa[MAX_MAHASISWA]; // Array 1 dimensi

    int pilihan, index;

    do {
        cout << "\n=== MENU PROGRAM DATA MAHASISWA ===" << endl;
        cout << "1. Tampilkan Mahasiswa" << endl;
        cout << "2. Tambah Mahasiswa" << endl;
        cout << "3. Ubah Mahasiswa" << endl;
        cout << "4. Hapus Mahasiswa" << endl;
        cout << "5. Keluar" << endl;
        cout << "Pilihan: ";
        cin >> pilihan;

        switch (pilihan) {

        case 1: // READ
            if (panjang == 0) {
                cout << "Belum ada mahasiswa." << endl;
            } else {
                for (int i = 0; i < panjang; i++) {
                    cout << "Mahasiswa ke-" << i + 1 
                         << ": " << mahasiswa[i] << endl;
                }
            }
            break;

        case 2: // CREATE
            if (panjang < MAX_MAHASISWA) {
                cout << "Masukkan nama mahasiswa: ";
                cin.ignore();
                getline(cin, mahasiswa[panjang]);
                panjang++;
                cout << "Mahasiswa berhasil ditambahkan." << endl;
            } else {
                cout << "Kapasitas penuh!" << endl;
            }
            break;

        case 3: // UPDATE
            if (panjang == 0) {
                cout << "Belum ada mahasiswa untuk diubah." << endl;
            } else {
                for (int i = 0; i < panjang; i++) {
                    cout << "Mahasiswa ke-" << i + 1 
                         << ": " << mahasiswa[i] << endl;
                }

                cout << "Masukkan nomor mahasiswa yang akan diubah: ";
                cin >> index;

                if (index > 0 && index <= panjang) {
                    cout << "Masukkan nama baru: ";
                    cin.ignore();
                    getline(cin, mahasiswa[index - 1]);
                    cout << "Data berhasil diubah." << endl;
                } else {
                    cout << "Nomor tidak valid." << endl;
                }
            }
            break;

        case 4: // DELETE
            if (panjang == 0) {
                cout << "Belum ada mahasiswa untuk dihapus." << endl;
            } else {
                for (int i = 0; i < panjang; i++) {
                    cout << "Mahasiswa ke-" << i + 1 
                         << ": " << mahasiswa[i] << endl;
                }

                cout << "Masukkan nomor mahasiswa yang akan dihapus: ";
                cin >> index;

                if (index > 0 && index <= panjang) {
                    for (int i = index - 1; i < panjang - 1; i++) {
                        mahasiswa[i] = mahasiswa[i + 1]; 
                    }
                    panjang--;
                    cout << "Mahasiswa berhasil dihapus." << endl;
                } else {
                    cout << "Nomor tidak valid." << endl;
                }
            }
            break;

        case 5:
            cout << "Program selesai." << endl;
            break;

        default:
            cout << "Pilihan tidak valid." << endl;
        }

    } while (pilihan != 5);

    return 0;
}