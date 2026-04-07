#include <iostream>
#include <string>
using namespace std;

#define MAX_SERVIS 100
#define MAX_USER   20
#define MAX_LOGIN  3
#define L          72

struct Kendaraan { string platNomor, merek, tipe; };
struct Pemilik   { string nama, noTelepon; };
struct Servis    { int idServis, biayaEstimasi, biayaFinal; Kendaraan kendaraan; Pemilik pemilik; string jenisServis; bool selesai; };
struct User      { string nama, nim, role; bool aktif; };

int    jumlahServis = 0; Servis daftarServis[MAX_SERVIS];
int    jumlahUser   = 0; User   daftarUser[MAX_USER];
User   userAktif;
int    panjangStr(string s);
string padStr(string s, int lebar);
string padInt(int angka, int lebar);
void   cetakGaris(int n);
void   cetakGaris(int n, char simbol);
void   tampilkanPesan(string pesan);
void   tampilkanPesan(string judul, string isi);
void   judulKotak(string teks, int lebar = L);
void   pauseScreen();
void   cetakHeaderServis();
void   cetakBarisTabel(int no, Servis s);
void   cetakDaftarServis(int jml, Servis daftar[]);
void   cetakDaftarUser(int jml, User daftar[]);
int    hitungTotalBiaya(int idx, int jml, Servis daftar[], string namaPemilik);
int    faktorialDiskon(int n);
int    idBerikutnya(int jml, Servis daftar[]);

void updateBiaya(int &biayaFinal, int &biayaEstimasi, int biayaBaru) {
    biayaEstimasi = biayaBaru;
    biayaFinal    = biayaBaru - 5000;
    cout << "  [PTR-&] Alamat biayaFinal  di fungsi: " << &biayaFinal  << "\n";
    cout << "  [PTR-&] Alamat biayaEstim. di fungsi: " << &biayaEstimasi << "\n";
}

void swapServis(Servis *a, Servis *b) {
    Servis tmp = *a;   
    *a         = *b;   
    *b         = tmp;  
}


void tampilInfoServisPtr(Servis *s) {
    cout << "  [PTR->] ID=" << s->idServis
         << "  Plat=" << s->kendaraan.platNomor
         << "  Pemilik=" << s->pemilik.nama
         << "  Status=" << (s->selesai ? "SELESAI" : "PROSES")
         << "\n";
}

void updatePemilikPtr(Pemilik *p, string namaBaru, string telpBaru) {
    p->nama      = namaBaru;
    p->noTelepon = telpBaru;
}

bool   loginUser(User daftar[], int &jml, User &aktif);
void   registerUser(User daftar[], int &jml);

void   adminTambahServis(Servis daftar[], int &jml);
void   adminTampilkanServis(Servis daftar[], int jml);
void   adminUbahServis(Servis daftar[], int jml);
void   adminHapusServis(Servis daftar[], int &jml);

void   adminLihatUser(User daftar[], int jml);
void   adminTambahUser(User daftar[], int &jml, User aktif);
void   adminHapusUser(User daftar[], int &jml, User aktif);
void   adminKelolaUser(User daftar[], int &jml, User aktif);

void   menuAdmin(Servis dftS[], int &jmlS, User dftU[], int &jmlU, User &aktif);
void   menuUser(Servis dftS[], int &jmlS, User &aktif);
void   menuAwal();

void   userLihatStatus(Servis daftar[], int jml, User aktif);
void   userRequestServis(Servis daftar[], int &jml, User aktif);

int main() { menuAwal(); return 0; }

int panjangStr(string s) { int n = 0; while (s[n] != '\0') n++; return n; }

string padStr(string s, int lebar) {
    int len = panjangStr(s);
    if (len >= lebar) { string h = ""; for (int i = 0; i < lebar; i++) h += s[i]; return h; }
    return s + string(lebar - len, ' ');
}

string padInt(int angka, int lebar) {
    if (angka < 0) {
        string s = "-";
        int tmp = -angka;
        string t = (tmp == 0) ? "0" : "";
        for (int x = tmp; x > 0; x /= 10) t = char('0' + x % 10) + t;
        s += t;
        int len = panjangStr(s);
        if (len >= lebar) { string h = ""; for (int i = 0; i < lebar; i++) h += s[i]; return h; }
        return s + string(lebar - len, ' ');
    }
    string s = (angka == 0) ? "0" : "";
    for (int t = angka; t > 0; t /= 10) s = char('0' + t % 10) + s;
    int len = panjangStr(s);
    if (len >= lebar) { string h = ""; for (int i = 0; i < lebar; i++) h += s[i]; return h; }
    return s + string(lebar - len, ' ');
}

void cetakGaris(int n)           { for (int i=0;i<n;i++) cout<<'='; cout<<"\n"; }
void cetakGaris(int n, char sim) { for (int i=0;i<n;i++) cout<<sim; cout<<"\n"; }

void tampilkanPesan(string pesan)             { cout << "  " << pesan << "\n"; }
void tampilkanPesan(string judul, string isi) { cout << "  [" << judul << "] " << isi << "\n"; }

void judulKotak(string teks, int lebar) {
    cetakGaris(lebar);
    int sp = (lebar - panjangStr(teks)) / 2; if (sp < 0) sp = 0;
    cout << string(sp, ' ') << teks << "\n";
    cetakGaris(lebar);
}

void pauseScreen() { cout << "\nTekan Enter untuk melanjutkan..."; cin.ignore(); cin.get(); }

void cetakHeaderServis() {
    cetakGaris(85);
    cout << padStr("No",4) << padStr("ID",5) << padStr("Plat",10) << padStr("Merek",10)
         << padStr("Tipe",7) << padStr("Pemilik",14) << padStr("Jenis",13)
         << padStr("Est.Biaya",10) << padStr("Byr Final",10) << padStr("Status",9) << "\n";
    cetakGaris(85, '-');
}

void cetakBarisTabel(int no, Servis s) {
    cout << padInt(no,4) << padInt(s.idServis,5) << padStr(s.kendaraan.platNomor,10)
         << padStr(s.kendaraan.merek,10) << padStr(s.kendaraan.tipe,7)
         << padStr(s.pemilik.nama,14) << padStr(s.jenisServis,13)
         << padInt(s.biayaEstimasi,10) << padInt(s.biayaFinal,10)
         << padStr(s.selesai ? "[SELESAI]" : "[PROSES]", 9) << "\n";
}

void cetakDaftarServis(int jml, Servis daftar[]) {
    cetakHeaderServis();
    for (int i = 0; i < jml; i++) cetakBarisTabel(i+1, daftar[i]);
    cetakGaris(85);
}

void cetakDaftarUser(int jml, User daftar[]) {
    cetakGaris(63);
    cout << padStr("No",5) << padStr("Nama",22) << padStr("NIM",18) << padStr("Role",8) << padStr("Status",10) << "\n";
    cetakGaris(63, '-');
    for (int i = 0; i < jml; i++)
        cout << padInt(i+1,5) << padStr(daftar[i].nama,22) << padStr(daftar[i].nim,18)
             << padStr(daftar[i].role,8) << padStr(daftar[i].aktif ? "[AKTIF]" : "[NONAKTIF]",10) << "\n";
    cetakGaris(63);
}

int hitungTotalBiaya(int idx, int jml, Servis daftar[], string namaPemilik) {
    if (idx >= jml) return 0;
    int biaya = (daftar[idx].pemilik.nama == namaPemilik && daftar[idx].selesai) ? daftar[idx].biayaFinal : 0;
    return biaya + hitungTotalBiaya(idx+1, jml, daftar, namaPemilik);
}

int faktorialDiskon(int n) { return (n <= 1) ? 1 : n * faktorialDiskon(n-1); }

int idBerikutnya(int jml, Servis daftar[]) {
    int mx = 0;
    for (int i = 0; i < jml; i++) if (daftar[i].idServis > mx) mx = daftar[i].idServis;
    return mx + 1;
}

bool loginUser(User daftar[], int &jml, User &aktif) {
    judulKotak("LOGIN");
    int coba = 0;
    string nama, nim;
    while (coba < MAX_LOGIN) {
        cin.ignore();
        cout << "  Nama (username) : "; getline(cin, nama);
        cout << "  NIM  (password) : "; getline(cin, nim);
        for (int i = 0; i < jml; i++) {
            if (daftar[i].nama == nama && daftar[i].nim == nim && daftar[i].aktif) {
                aktif = daftar[i];
                tampilkanPesan("OK", "Login berhasil! Selamat datang, " + aktif.nama + " [" + aktif.role + "]");
                return true;
            }
        }
        coba++;
        int sisa = MAX_LOGIN - coba;
        if (sisa > 0) tampilkanPesan("!", "Nama/NIM salah. Sisa percobaan: " + to_string(sisa));
    }
    tampilkanPesan("X", "Batas login tercapai. Program berhenti.");
    return false;
}

void registerUser(User daftar[], int &jml) {
    judulKotak("REGISTER AKUN BARU (CUSTOMER)");
    if (jml >= MAX_USER) { tampilkanPesan("!", "Kapasitas user penuh!"); return; }
    User u; u.role = "user"; u.aktif = true;
    cin.ignore();
    cout << "  Nama (username) : "; getline(cin, u.nama);
    cout << "  NIM  (password) : "; getline(cin, u.nim);
    for (int i = 0; i < jml; i++) {
        if (daftar[i].nim == u.nim) { tampilkanPesan("!", "NIM sudah terdaftar!"); return; }
    }
    daftar[jml++] = u;
    tampilkanPesan("OK", "Registrasi berhasil! \"" + u.nama + "\" terdaftar sebagai Customer.");
}

void adminTambahServis(Servis daftar[], int &jml) {
    judulKotak("[ ADMIN ] TAMBAH DATA SERVIS");
    if (jml >= MAX_SERVIS) { tampilkanPesan("!", "Kapasitas penuh!"); return; }
    int idx = jml;
    daftar[idx].idServis = idBerikutnya(jml, daftar);
    cin.ignore();
    cout << "\n  [ Kendaraan ]\n";
    cout << "  Plat Nomor        : "; getline(cin, daftar[idx].kendaraan.platNomor);
    cout << "  Merek             : "; getline(cin, daftar[idx].kendaraan.merek);
    cout << "  Tipe (Motor/Mobil): "; getline(cin, daftar[idx].kendaraan.tipe);
    cout << "\n  [ Pemilik ]\n";
    cout << "  Nama Pemilik      : "; getline(cin, daftar[idx].pemilik.nama);
    cout << "  No. Telepon       : "; getline(cin, daftar[idx].pemilik.noTelepon);
    cout << "\n  [ Servis ]\n";
    cout << "  Jenis Servis      : "; getline(cin, daftar[idx].jenisServis);
    cout << "  Estimasi Biaya Rp : "; cin >> daftar[idx].biayaEstimasi;
    daftar[idx].biayaFinal = 0; daftar[idx].selesai = false; jml++;
    tampilkanPesan("OK", "Data ditambahkan! (ID: " + to_string(daftar[idx].idServis) + ")");
}

void adminTampilkanServis(Servis daftar[], int jml) {
    judulKotak("[ ADMIN ] DAFTAR SERVIS", 85);
    if (jml == 0) { tampilkanPesan("Belum ada data servis."); return; }
    cetakDaftarServis(jml, daftar);
    cout << "  Total data: " << jml << "\n";

    cout << "\n  --- Ringkasan ---\n";
    for (int i = 0; i < jml; i++) {
        tampilInfoServisPtr(&daftar[i]);   
    }
    cetakGaris(L, '-');
}

void adminUbahServis(Servis daftar[], int jml) {
    judulKotak("[ ADMIN ] UBAH DATA SERVIS");
    if (jml == 0) { tampilkanPesan("Belum ada data."); return; }
    cetakDaftarServis(jml, daftar);
    int nomor; cout << "  Nomor urut yang diubah: "; cin >> nomor;
    if (nomor < 1 || nomor > jml) { tampilkanPesan("!", "Nomor tidak valid!"); return; }
    int idx = nomor - 1; string inp;
    cin.ignore();

    cout << "\n  [ Ubah Kendaraan ] (Enter = skip)\n";
    cout << "  Plat  [" << daftar[idx].kendaraan.platNomor << "]: "; getline(cin,inp); if(!inp.empty()) daftar[idx].kendaraan.platNomor=inp;
    cout << "  Merek [" << daftar[idx].kendaraan.merek     << "]: "; getline(cin,inp); if(!inp.empty()) daftar[idx].kendaraan.merek=inp;
    cout << "  Tipe  [" << daftar[idx].kendaraan.tipe      << "]: "; getline(cin,inp); if(!inp.empty()) daftar[idx].kendaraan.tipe=inp;

    cout << "\n  [ Ubah Pemilik ] (Enter = skip)\n";
    string namaBaru, telpBaru;
    cout << "  Nama  [" << daftar[idx].pemilik.nama        << "]: "; getline(cin, namaBaru);
    cout << "  Telp  [" << daftar[idx].pemilik.noTelepon   << "]: "; getline(cin, telpBaru);

    if (namaBaru.empty()) namaBaru = daftar[idx].pemilik.nama;
    if (telpBaru.empty()) telpBaru = daftar[idx].pemilik.noTelepon;

    cout << "  [PTR->] Alamat struct Pemilik: " << &daftar[idx].pemilik << "\n";
    updatePemilikPtr(&daftar[idx].pemilik, namaBaru, telpBaru);

    cout << "\n  [ Ubah Servis ]\n";
    cout << "  Jenis       [" << daftar[idx].jenisServis    << "]: "; getline(cin,inp); if(!inp.empty()) daftar[idx].jenisServis=inp;

    cout << "  Biaya Final [" << daftar[idx].biayaFinal     << "]: "; getline(cin,inp);
    if (!inp.empty()) {
        int biayaBaru = stoi(inp);
        cout << "  [PTR-&] Alamat biayaFinal  di main: " << &daftar[idx].biayaFinal  << "\n";
        cout << "  [PTR-&] Alamat biayaEstim. di main: " << &daftar[idx].biayaEstimasi << "\n";
        updateBiaya(daftar[idx].biayaFinal, daftar[idx].biayaEstimasi, biayaBaru);
        tampilkanPesan("*", "Biaya administrasi Rp5.000 telah dipotong dari biaya final.");
        tampilkanPesan("*", "Biaya Final sekarang: Rp" + to_string(daftar[idx].biayaFinal));
        daftar[idx].selesai = true;
        tampilkanPesan("*", "Status otomatis diubah ke [SELESAI] karena biaya final sudah diisi.");
    }

    cout << "  Selesai (1=Ya / 0=Tidak) [" << daftar[idx].selesai << "]: "; getline(cin,inp);
    if (!inp.empty()) daftar[idx].selesai = (stoi(inp) == 1);

    tampilkanPesan("OK", "Data berhasil diubah!");
}

void adminHapusServis(Servis daftar[], int &jml) {
    judulKotak("[ ADMIN ] HAPUS DATA SERVIS");
    if (jml == 0) { tampilkanPesan("Belum ada data."); return; }
    cetakDaftarServis(jml, daftar);
    int nomor; cout << "  Nomor urut yang dihapus: "; cin >> nomor;
    if (nomor < 1 || nomor > jml) { tampilkanPesan("!", "Nomor tidak valid!"); return; }

    for (int i = nomor-1; i < jml-1; i++)
        swapServis(&daftar[i], &daftar[i+1]);   

    jml--;
    tampilkanPesan("OK", "Data berhasil dihapus!");
}

void adminLihatUser(User daftar[], int jml) {
    judulKotak("[ ADMIN ] DAFTAR USER");
    if (jml == 0) { tampilkanPesan("Belum ada user."); return; }
    cetakDaftarUser(jml, daftar); cout << "  Total user: " << jml << "\n";
}

void adminTambahUser(User daftar[], int &jml, User aktif) {
    judulKotak("[ ADMIN ] TAMBAH USER");
    if (jml >= MAX_USER) { tampilkanPesan("!", "Kapasitas penuh!"); return; }
    User u; cin.ignore();
    cout << "  Nama (username)        : "; getline(cin, u.nama);
    cout << "  NIM  (password)        : "; getline(cin, u.nim);
    cout << "  Role (1=User, 2=Admin) : "; int r; cin >> r;
    u.role = (r == 2) ? "admin" : "user"; u.aktif = true;
    for (int i = 0; i < jml; i++) { if (daftar[i].nim == u.nim) { tampilkanPesan("!", "NIM sudah digunakan!"); return; } }
    daftar[jml++] = u;
    tampilkanPesan("OK", "User \"" + u.nama + "\" ditambahkan sebagai " + u.role + "!");
}

void adminHapusUser(User daftar[], int &jml, User aktif) {
    judulKotak("[ ADMIN ] HAPUS USER");
    if (jml == 0) { tampilkanPesan("Belum ada user."); return; }
    cetakDaftarUser(jml, daftar);
    int no; cout << "  Nomor user yang dihapus: "; cin >> no;
    if (no < 1 || no > jml)           { tampilkanPesan("!", "Nomor tidak valid!"); return; }
    if (daftar[no-1].nim == aktif.nim) { tampilkanPesan("!", "Tidak bisa menghapus akun sendiri!"); return; }
    string nama = daftar[no-1].nama;
    for (int i = no-1; i < jml-1; i++) daftar[i] = daftar[i+1];
    jml--;
    tampilkanPesan("OK", "User \"" + nama + "\" berhasil dihapus!");
}

void adminKelolaUser(User daftar[], int &jml, User aktif) {
    judulKotak("[ ADMIN ] KELOLA USER");
    cout << "  1. Tambah User\n  2. Hapus User\n  0. Kembali\n";
    cetakGaris(L, '-'); cout << "  Pilihan: "; int p; cin >> p;
    if      (p == 1) adminTambahUser(daftar, jml, aktif);
    else if (p == 2) adminHapusUser(daftar, jml, aktif);
}

void userLihatStatus(Servis daftar[], int jml, User aktif) {
    judulKotak("[ USER ] STATUS SERVIS SAYA");
    cout << padStr("No",4) << padStr("Plat",10) << padStr("Merek",10)
         << padStr("Jenis Servis",14) << padStr("Est.Biaya",14)
         << padStr("Biaya Final",14) << padStr("Status",12) << "\n";
    cetakGaris(78, '-');
    bool ada = false; int no = 1;
    for (int i = 0; i < jml; i++) {
        if (daftar[i].pemilik.nama == aktif.nama) {
            ada = true;
            cout << padInt(no++,4) << padStr(daftar[i].kendaraan.platNomor,10)
                 << padStr(daftar[i].kendaraan.merek,10) << padStr(daftar[i].jenisServis,14)
                 << padStr("Rp "+padInt(daftar[i].biayaEstimasi,8),14)
                 << padStr("Rp "+padInt(daftar[i].selesai ? daftar[i].biayaFinal : 0,8),14)
                 << padStr(daftar[i].selesai ? "[SELESAI]" : "[PROSES]",12) << "\n";
        }
    }
    cetakGaris(78);
    if (!ada) {
        tampilkanPesan("Belum ada data servis atas nama \"" + aktif.nama + "\".");
    } else {
        int total = hitungTotalBiaya(0, jml, daftar, aktif.nama);
        cout << "  Total biaya final : Rp " << total << "\n";
        int kunjungan = no - 1;
        if (kunjungan >= 2) {
            faktorialDiskon(kunjungan > 5 ? 5 : kunjungan);
            tampilkanPesan("*Bonus loyalitas (" + to_string(kunjungan) + "x servis) - info diskon di kasir!");
        }
        tampilkanPesan("*Biaya Final = 0 artinya servis masih dalam proses.");
    }
}

void userRequestServis(Servis daftar[], int &jml, User aktif) {
    judulKotak("[ USER ] AJUKAN SERVIS KENDARAAN");
    if (jml >= MAX_SERVIS) { tampilkanPesan("!", "Kapasitas penuh, hubungi admin."); return; }
    int idx = jml;
    daftar[idx].idServis = idBerikutnya(jml, daftar);
    daftar[idx].pemilik.nama = aktif.nama;
    cin.ignore();
    cout << "  No. Telepon        : "; getline(cin, daftar[idx].pemilik.noTelepon);
    cout << "\n  [ Kendaraan ]\n";
    cout << "  Plat Nomor         : "; getline(cin, daftar[idx].kendaraan.platNomor);
    cout << "  Merek              : "; getline(cin, daftar[idx].kendaraan.merek);
    cout << "  Tipe (Motor/Mobil) : "; getline(cin, daftar[idx].kendaraan.tipe);
    cout << "\n  [ Servis ]\n";
    cout << "  Jenis Servis       : "; getline(cin, daftar[idx].jenisServis);
    daftar[idx].biayaEstimasi = 0; daftar[idx].biayaFinal = 0; daftar[idx].selesai = false; jml++;
    tampilkanPesan("OK", "Servis berhasil diajukan! (ID: " + to_string(daftar[idx].idServis) + ")");
    tampilkanPesan("Admin akan segera memproses kendaraanmu!");
}

void menuAdmin(Servis dftS[], int &jmlS, User dftU[], int &jmlU, User &aktif) {
    int p;
    do {
        cout << "\n"; judulKotak("MENU ADMIN - SISTEM SERVIS KENDARAAN");
        cout << "  User: " << aktif.nama << "  [" << aktif.role << "]\n";
        cetakGaris(L, '-');
        cout << "  1. Lihat Data Servis\n  2. Tambah Data Servis\n  3. Ubah Data Servis\n"
             << "  4. Hapus Data Servis\n  5. Lihat Daftar User\n  6. Kelola User\n  0. Logout\n";
        cetakGaris(L); cout << "  Pilihan: "; cin >> p;
        switch (p) {
            case 1: adminTampilkanServis(dftS, jmlS);        pauseScreen(); break;
            case 2: adminTambahServis(dftS, jmlS);           pauseScreen(); break;
            case 3: adminUbahServis(dftS, jmlS);             pauseScreen(); break;
            case 4: adminHapusServis(dftS, jmlS);            pauseScreen(); break;
            case 5: adminLihatUser(dftU, jmlU);              pauseScreen(); break;
            case 6: adminKelolaUser(dftU, jmlU, aktif);      pauseScreen(); break;
            case 0: tampilkanPesan("Logout", "Sampai jumpa, " + aktif.nama + "!"); break;
            default: tampilkanPesan("!", "Pilihan tidak valid!");
        }
    } while (p != 0);
}

void menuUser(Servis dftS[], int &jmlS, User &aktif) {
    int p;
    do {
        cout << "\n"; judulKotak("MENU USER - SISTEM SERVIS KENDARAAN");
        cout << "  User: " << aktif.nama << "  [" << aktif.role << "]\n";
        cetakGaris(L, '-');
        cout << "  1. Ajukan Servis Kendaraan\n  2. Lihat Status & Biaya Servis\n  0. Logout\n";
        cetakGaris(L); cout << "  Pilihan: "; cin >> p;
        switch (p) {
            case 1: userRequestServis(dftS, jmlS, aktif); pauseScreen(); break;
            case 2: userLihatStatus(dftS, jmlS, aktif);   pauseScreen(); break;
            case 0: tampilkanPesan("Logout", "See you, " + aktif.nama + "!"); break;
            default: tampilkanPesan("!", "Pilihan tidak valid!");
        }
    } while (p != 0);
}

void menuAwal() {
    daftarUser[0] = {"ronalvi", "111", "admin", true}; jumlahUser = 1;
    int p;
    do {
        cout << "\n"; judulKotak("SELAMAT DATANG - BENGKEL DAENGGG RONALVI");
        cout << "  1. Login\n  2. Register (Customer)\n  0. Keluar\n";
        cetakGaris(L); cout << "  Pilihan: "; cin >> p;
        switch (p) {
            case 1:
                if (loginUser(daftarUser, jumlahUser, userAktif)) {
                    if (userAktif.role == "admin") menuAdmin(daftarServis, jumlahServis, daftarUser, jumlahUser, userAktif);
                    else                           menuUser(daftarServis, jumlahServis, userAktif);
                } else return;
                break;
            case 2: registerUser(daftarUser, jumlahUser); pauseScreen(); break;
            case 0: tampilkanPesan("Terima kasih sudah menggunakan program ini!"); break;
            default: tampilkanPesan("!", "Pilihan tidak valid!");
        }
    } while (p != 0);
}