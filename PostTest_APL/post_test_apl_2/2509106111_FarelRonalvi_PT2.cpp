#include <iostream>
#include <string>
#include <sstream>
using namespace std;

struct User {
    string nama;
    string nim;
    string role;
    bool   aktif;
};

struct Kendaraan {
    string platNomor;
    string merek;
    string tipe;
};

struct Pemilik {
    string nama;
    string noTelepon;
};

struct Servis {
    int       idServis;
    Kendaraan kendaraan;
    Pemilik   pemilik;
    string    jenisServis;
    int       biayaEstimasi;
    int       biayaFinal;
    bool      selesai;
};

#define MAX_SERVIS  100
#define MAX_USER    20
#define MAX_LOGIN   3

int    jumlahServis = 0;
Servis daftarServis[MAX_SERVIS];

int    jumlahUser = 0;
User   daftarUser[MAX_USER];

User   userAktif;

int garisTebal(int n = 72);
int garisTipis(int n = 72);
int judulKotak(string teks, int lebar = 72);
int pauseScreen();
int cetakHeaderServis();
int cetakBarisTabel(int no, Servis s);
int cetakHeaderUser();
int idBerikutnya();
bool loginUser();
int registerUser();
int adminTambahServis();
int adminTampilkanServis();
int adminUbahServis();
int adminHapusServis();
int adminLihatUser();
int adminKelolaUser();
int menuAdmin();
int userLihatStatus();
int userRequestServis();
int menuUser();
int menuAwal();

int main() {
    menuAwal();
    return 0;
}

int menuAwal() {
    daftarUser[0] = {"Admin", "admin123", "admin", true};
    jumlahUser    = 1;

    int pilihan;
    do {
        cout << "\n";
        judulKotak("SELAMAT DATANG - BENGKEL DAENGGG RONALVI");
        cout << "  1. Login\n";
        cout << "  2. Register\n";
        cout << "  0. Keluar\n";
        garisTebal();
        cout << "  Pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                if (loginUser()) {
                    if (userAktif.role == "admin") menuAdmin();
                    else                            menuUser();
                } else {
                    return 0; 
                }
                break;
            case 2:
                registerUser();
                pauseScreen();
                break;
            case 0:
                cout << "\n  Program selesai terimakasih sudah mencobai program ini abng/mbaa kuhhhh. Terima kasih!\n\n";
                break;
            default:
                cout << "  [!] Pilihan tidak valid!\n";
        }
    } while (pilihan != 0);
    return 0;
}

int panjangStr(string s) {
    int n = 0;
    while (s[n] != '\0') n++;
    return n;
}
string pad(string s, int lebar) {
    if (panjangStr(s) >= lebar) {
        string hasil = "";
        for (int i = 0; i < lebar; i++) hasil += s[i];
        return hasil;
    }
    return s + string(lebar - panjangStr(s), ' ');
}
string pad(int angka, int lebar) {
    string s = "";
    int tmp = angka;
    if (tmp == 0) { s = "0"; }
    else {
        while (tmp > 0) { s = char('0' + tmp % 10) + s; tmp /= 10; }
    }
    if (panjangStr(s) >= lebar) {
        string hasil = "";
        for (int i = 0; i < lebar; i++) hasil += s[i];
        return hasil;
    }
    return s + string(lebar - panjangStr(s), ' ');
}

int garisTebal(int n) { for (int i=0;i<n;i++) cout<<"="; cout<<"\n"; return 0; }
int garisTipis(int n) { for (int i=0;i<n;i++) cout<<"-"; cout<<"\n"; return 0; }

int judulKotak(string teks, int lebar) {
    garisTebal(lebar);
    int spasi = (lebar - panjangStr(teks)) / 2;
    if (spasi < 0) spasi = 0;
    cout << string(spasi,' ') << teks << "\n";
    garisTebal(lebar);
    return 0;
}

int pauseScreen() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore();
    cin.get();
    return 0;
}

int cetakHeaderServis() {
    garisTebal(85);
    cout << pad("No",4)  << pad("ID",5)  << pad("Plat",10) << pad("Merek",10)
         << pad("Tipe",7) << pad("Pemilik",14) << pad("Jenis",13)
         << pad("Est.Biaya",10) << pad("Byr Final",10) << pad("Status",9)
         << "\n";
    garisTipis(85);
    return 0;
}

int cetakBarisTabel(int no, Servis s) {
    cout << pad(no, 4)
         << pad(s.idServis, 5)
         << pad(s.kendaraan.platNomor, 10)
         << pad(s.kendaraan.merek, 10)
         << pad(s.kendaraan.tipe, 7)
         << pad(s.pemilik.nama, 14)
         << pad(s.jenisServis, 13)
         << pad(s.biayaEstimasi, 10)
         << pad(s.selesai ? s.biayaFinal : 0, 10)
         << pad(s.selesai ? "[SELESAI]" : "[PROSES]", 9)
         << "\n";
    return 0;
}

int cetakHeaderUser() {
    garisTebal(63);
    cout << pad("No",5) << pad("Nama",22) << pad("NIM",18)
         << pad("Role",8) << pad("Status",10) << "\n";
    garisTipis(63);
    return 0;
}

int idBerikutnya() {
    int mx = 0;
    for (int i = 0; i < jumlahServis; i++)
        if (daftarServis[i].idServis > mx) mx = daftarServis[i].idServis;
    return mx + 1;
}

bool loginUser() {
    judulKotak("LOGIN");
    int coba = 0;
    while (coba < MAX_LOGIN) {
        string nama, nim;
        cin.ignore();
        cout << "  Nama (username) : "; getline(cin, nama);
        cout << "  NIM  (password) : "; getline(cin, nim);

        for (int i = 0; i < jumlahUser; i++) {
            if (daftarUser[i].nama == nama &&
                daftarUser[i].nim  == nim  &&
                daftarUser[i].aktif) {
                userAktif = daftarUser[i];
                cout << "\n  [OK] AZEKKKK Login berhasil! Selamat datang KANDAA KUHHH, "
                     << userAktif.nama << "  [" << userAktif.role << "]\n";
                return true;
            }
        }

        coba++;
        int sisa = MAX_LOGIN - coba;
        if (sisa > 0)
            cout << "  [!] Nama/NIM salah. Sisa percobaan: " << sisa << "\n\n";
    }
    cout << "\n  [X] Yahhh anda sdh mencapai batas login KANDA KUHHH. Program berhenti.\n";
    return false;
}

int registerUser() {
    judulKotak("REGISTER AKUN BARU");
    if (jumlahUser >= MAX_USER) { cout << "  Kapasitas user penuh!\n"; return 0; }

    User u;
    cin.ignore();
    cout << "  Nama (username)        : "; getline(cin, u.nama);
    cout << "  NIM  (password)        : "; getline(cin, u.nim);
    cout << "  Role (1=User, 2=Admin) : ";
    int r; cin >> r;
    u.role  = (r == 2) ? "admin" : "user";
    u.aktif = true;

    for (int i = 0; i < jumlahUser; i++) {
        if (daftarUser[i].nim == u.nim) {
            cout << "\n  [!] NIM sudah terdaftar!\n"; return 0;
        }
    }

    daftarUser[jumlahUser++] = u;
    cout << "\n  [OK] Yeyyy registrasi berhasil sebagai " << u.role << "!\n";
    return 0;
}

int adminTambahServis() {
    judulKotak("[ ADMIN ] TAMBAH DATA SERVIS");
    if (jumlahServis >= MAX_SERVIS) { cout << "  Kapasitas penuh!\n"; return 0; }

    int idx = jumlahServis;
    daftarServis[idx].idServis = idBerikutnya();

    cin.ignore();
    cout << "\n  [ Data Kendaraan ]\n";
    cout << "  Plat Nomor        : "; getline(cin, daftarServis[idx].kendaraan.platNomor);
    cout << "  Merek             : "; getline(cin, daftarServis[idx].kendaraan.merek);
    cout << "  Tipe (Motor/Mobil): "; getline(cin, daftarServis[idx].kendaraan.tipe);

    cout << "\n  [ Data Pemilik ]\n";
    cout << "  Nama Pemilik      : "; getline(cin, daftarServis[idx].pemilik.nama);
    cout << "  No. Telepon       : "; getline(cin, daftarServis[idx].pemilik.noTelepon);

    cout << "\n  [ Data Servis ]\n";
    cout << "  Jenis Servis      : "; getline(cin, daftarServis[idx].jenisServis);
    cout << "  Estimasi Biaya Rp : "; cin >> daftarServis[idx].biayaEstimasi;

    daftarServis[idx].biayaFinal = 0;
    daftarServis[idx].selesai    = false;
    jumlahServis++;

    cout << "\n  [OK] Data ditambahkan! (ID: " << daftarServis[idx].idServis << ")\n";
    return 0;
}

int adminTampilkanServis() {
    judulKotak("[ ADMIN ] DAFTAR SEMUA DATA SERVIS", 85);
    if (jumlahServis == 0) { cout << "  Belum ada data servis.\n"; return 0; }
    cetakHeaderServis();
    for (int i = 0; i < jumlahServis; i++) cetakBarisTabel(i+1, daftarServis[i]);
    garisTebal(85);
    cout << "  Total data: " << jumlahServis << "\n";
    return 0;
}

int adminUbahServis() {
    judulKotak("[ ADMIN ] UBAH DATA SERVIS");
    if (jumlahServis == 0) { cout << "  Belum ada data.\n"; return 0; }

    cetakHeaderServis();
    for (int i = 0; i < jumlahServis; i++) cetakBarisTabel(i+1, daftarServis[i]);
    garisTebal(85);

    int nomor;
    cout << "  Masukkan nomor urut yang diubah: ";
    cin >> nomor;
    if (nomor < 1 || nomor > jumlahServis) { cout << "  [!] Nomor tidak valid!\n"; return 0; }

    int idx = nomor - 1;
    string inp;
    cin.ignore();

    cout << "\n  [ Ubah Kendaraan ] (Enter = tidak diubah)\n";
    cout << "  Plat   [" << daftarServis[idx].kendaraan.platNomor << "]: ";
    getline(cin, inp); if (!inp.empty()) daftarServis[idx].kendaraan.platNomor = inp;

    cout << "  Merek  [" << daftarServis[idx].kendaraan.merek << "]: ";
    getline(cin, inp); if (!inp.empty()) daftarServis[idx].kendaraan.merek = inp;

    cout << "  Tipe   [" << daftarServis[idx].kendaraan.tipe << "]: ";
    getline(cin, inp); if (!inp.empty()) daftarServis[idx].kendaraan.tipe = inp;

    cout << "\n  [ Ubah Pemilik ]\n";
    cout << "  Nama   [" << daftarServis[idx].pemilik.nama << "]: ";
    getline(cin, inp); if (!inp.empty()) daftarServis[idx].pemilik.nama = inp;

    cout << "  Telp   [" << daftarServis[idx].pemilik.noTelepon << "]: ";
    getline(cin, inp); if (!inp.empty()) daftarServis[idx].pemilik.noTelepon = inp;

    cout << "\n  [ Ubah Servis ]\n";
    cout << "  Jenis  [" << daftarServis[idx].jenisServis << "]: ";
    getline(cin, inp); if (!inp.empty()) daftarServis[idx].jenisServis = inp;

    cout << "  Est.Biaya (Enter=skip) [" << daftarServis[idx].biayaEstimasi << "]: ";
    getline(cin, inp);
    if (!inp.empty()) daftarServis[idx].biayaEstimasi = stoi(inp);

    cout << "  Biaya Final (Enter=skip) [" << daftarServis[idx].biayaFinal << "]: ";
    getline(cin, inp);
    if (!inp.empty()) daftarServis[idx].biayaFinal = stoi(inp);

    cout << "  Status Selesai (Enter=skip, 1=Selesai, 0=Proses) [" << daftarServis[idx].selesai << "]: ";
    getline(cin, inp);
    if (!inp.empty()) daftarServis[idx].selesai = (stoi(inp) == 1);

    cout << "\n  [OK] Data berhasil diubah!\n";
    return 0;
}

int adminHapusServis() {
    judulKotak("[ ADMIN ] HAPUS DATA SERVIS");
    if (jumlahServis == 0) { cout << "  Belum ada data.\n"; return 0; }

    cetakHeaderServis();
    for (int i = 0; i < jumlahServis; i++) cetakBarisTabel(i+1, daftarServis[i]);
    garisTebal(85);

    int nomor;
    cout << "  Masukkan nomor urut yang dihapus: ";
    cin >> nomor;
    if (nomor < 1 || nomor > jumlahServis) { cout << "  [!] Nomor tidak valid!\n"; return 0; }

    for (int i = nomor - 1; i < jumlahServis - 1; i++)
        daftarServis[i] = daftarServis[i + 1];
    jumlahServis--;
    cout << "\n  [OK] Data berhasil dihapus!\n";
    return 0;
}

int adminLihatUser() {
    judulKotak("[ ADMIN ] DAFTAR USER TERDAFTAR");
    if (jumlahUser == 0) { cout << "  Belum ada user.\n"; return 0; }
    cetakHeaderUser();
    for (int i = 0; i < jumlahUser; i++) {
        cout << pad(i+1, 5)
             << pad(daftarUser[i].nama, 22)
             << pad(daftarUser[i].nim, 18)
             << pad(daftarUser[i].role, 8)
             << pad(daftarUser[i].aktif ? "[AKTIF]" : "[NONAKTIF]", 10)
             << "\n";
    }
    garisTebal(63);
    cout << "  Total user: " << jumlahUser << "\n";
    return 0;
}

int adminKelolaUser() {
    judulKotak("[ ADMIN ] KELOLA USER");
    cout << "  1. Tambah User Baru\n";
    cout << "  2. Hapus User\n";
    cout << "  0. Kembali\n";
    garisTipis();
    cout << "  Pilihan: ";
    int p; cin >> p;

    if (p == 1) {
        judulKotak("[ ADMIN ] TAMBAH USER BARU");
        if (jumlahUser >= MAX_USER) { cout << "  Kapasitas penuh!\n"; return 0; }
        User u;
        cin.ignore();
        cout << "  Nama (username)        : "; getline(cin, u.nama);
        cout << "  NIM  (password)        : "; getline(cin, u.nim);
        cout << "  Role (1=User, 2=Admin) : ";
        int r; cin >> r;
        u.role  = (r == 2) ? "admin" : "user";
        u.aktif = true;
        for (int i = 0; i < jumlahUser; i++)
            if (daftarUser[i].nim == u.nim) { cout << "\n  [!] username/password telah terpakai/digunakan kanda kuhh!!!\n"; return 0; }
        daftarUser[jumlahUser++] = u;
        cout << "\n  [OK] User \"" << u.nama << "\" ditambahkan sebagai " << u.role << "!\n";

    } else if (p == 2) {
        judulKotak("[ ADMIN ] HAPUS USER");
        if (jumlahUser == 0) { cout << "  Belum ada user.\n"; return 0; }
        cetakHeaderUser();
        for (int i = 0; i < jumlahUser; i++) {
            cout << pad(i+1, 5)
                 << pad(daftarUser[i].nama, 22)
                 << pad(daftarUser[i].nim, 18)
                 << pad(daftarUser[i].role, 8)
                 << pad(daftarUser[i].aktif ? "[AKTIF]" : "[NONAKTIF]", 10)
                 << "\n";
        }
        garisTebal(63);
        cout << "  Nomor user yang dihapus: ";
        int no; cin >> no;
        if (no < 1 || no > jumlahUser) { cout << "  [!] Nomor tidak valid!\n"; return 0; }
        if (daftarUser[no-1].nim == userAktif.nim) {
            cout << "  [!] Tidak bisa menghapus akun sendiri!\n"; return 0;
        }
        string namaHapus = daftarUser[no-1].nama;
        for (int i = no - 1; i < jumlahUser - 1; i++)
            daftarUser[i] = daftarUser[i + 1];
        jumlahUser--;
        cout << "\n  [OK] User \"" << namaHapus << "\" berhasil dihapus!\n";
    }
    return 0;
}

int menuAdmin() {
    int pilihan;
    do {
        cout << "\n";
        judulKotak("MENU ADMIN - SISTEM SERVIS KENDARAAN");
        cout << "  User : " << userAktif.nama
             << "   Role: [" << userAktif.role << "]\n";
        garisTipis();
        cout << "  1. Lihat Data Servis\n";
        cout << "  2. Tambah Data Servis\n";
        cout << "  3. Ubah Data Servis\n";
        cout << "  4. Hapus Data Servis\n";
        cout << "  5. Lihat Daftar User\n";
        cout << "  6. Kelola User (Tambah / Hapus)\n";
        cout << "  0. Logout\n";
        garisTebal();
        cout << "  Pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: adminTampilkanServis(); pauseScreen(); break;
            case 2: adminTambahServis();    pauseScreen(); break;
            case 3: adminUbahServis();      pauseScreen(); break;
            case 4: adminHapusServis();     pauseScreen(); break;
            case 5: adminLihatUser();       pauseScreen(); break;
            case 6: adminKelolaUser();      pauseScreen(); break;
            case 0: cout << "\n  [Logout] Sampai jumpa kembali kanda kuhhh, " << userAktif.nama << "!\n"; break;
            default: cout << "  [!] Pilihan tidak valid!\n";
        }
    } while (pilihan != 0);
    return 0;
}

int userLihatStatus() {
    judulKotak("[ USER ] STATUS & BIAYA SERVIS SAYA");

    bool ada = false;
    cout << pad("No",4) << pad("Plat",10) << pad("Merek",10)
         << pad("Jenis Servis",14) << pad("Est. Biaya",14)
         << pad("Biaya Final",14) << pad("Status",12) << "\n";
    garisTipis(78);

    int no = 1;
    for (int i = 0; i < jumlahServis; i++) {
        if (daftarServis[i].pemilik.nama == userAktif.nama) {
            ada = true;
            cout << pad(no++, 4)
                 << pad(daftarServis[i].kendaraan.platNomor, 10)
                 << pad(daftarServis[i].kendaraan.merek, 10)
                 << pad(daftarServis[i].jenisServis, 14)
                 << pad("Rp " + pad(daftarServis[i].biayaEstimasi, 8), 14)
                 << pad("Rp " + pad(daftarServis[i].selesai ? daftarServis[i].biayaFinal : 0, 8), 14)
                 << pad(daftarServis[i].selesai ? "[SELESAI]" : "[PROSES]", 12)
                 << "\n";
        }
    }
    garisTebal(78);

    if (!ada) {
        cout << "  Belum ada data servis atas nama \"" << userAktif.nama << "\".\n";
    } else {
        cout << "  *Biaya Final = 0 artinya servis masih dalam proses.\n";
    }
    return 0;
}

int userRequestServis() {
    judulKotak("[ USER ] AJUKAN SERVIS KENDARAAN");

    if (jumlahServis >= MAX_SERVIS) {
        cout << "  Kapasitas data penuh, hubungi admin.\n";
        return 0;
    }

    int idx = jumlahServis;
    daftarServis[idx].idServis = idBerikutnya();

    daftarServis[idx].pemilik.nama = userAktif.nama;

    cin.ignore();
    cout << "  No. Telepon              : ";
    getline(cin, daftarServis[idx].pemilik.noTelepon);

    cout << "\n  [ Data Kendaraan ]\n";
    cout << "  Plat Nomor               : "; getline(cin, daftarServis[idx].kendaraan.platNomor);
    cout << "  Merek                    : "; getline(cin, daftarServis[idx].kendaraan.merek);
    cout << "  Tipe (Motor/Mobil)       : "; getline(cin, daftarServis[idx].kendaraan.tipe);

    cout << "\n  [ Permintaan Servis ]\n";
    cout << "  Jenis Servis yang diminta: "; getline(cin, daftarServis[idx].jenisServis);

    daftarServis[idx].biayaEstimasi = 0;
    daftarServis[idx].biayaFinal    = 0;
    daftarServis[idx].selesai       = false;
    jumlahServis++;

    cout << "\n  [OK] Yeyyy permintaan servis berhasil diajukan! (ID: "
         << daftarServis[idx].idServis << ")\n";
    cout << "  Admin akan segera memproses yaa KANDA KUHHH!!!.\n";
    return 0;
}

int menuUser() {
    int pilihan;
    do {
        cout << "\n";
        judulKotak("MENU USER - SISTEM SERVIS KENDARAAN");
        cout << "  User : " << userAktif.nama
             << "   Role: [" << userAktif.role << "]\n";
        garisTipis();
        cout << "  1. Ajukan Servis Kendaraan\n";
        cout << "  2. Lihat Status & Biaya Servis Saya\n";
        cout << "  0. Logout\n";
        garisTebal();
        cout << "  Pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: userRequestServis(); pauseScreen(); break;
            case 2: userLihatStatus();   pauseScreen(); break;
            case 0: cout << "\n  [Logout] Seee youuuu, " << userAktif.nama << "!\n"; break;
            default: cout << "  [!] Pilihan tidak valid!\n";
        }
    } while (pilihan != 0);
    return 0;
}
 
