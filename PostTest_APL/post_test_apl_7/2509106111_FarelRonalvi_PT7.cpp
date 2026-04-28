#ifndef RONALVI_LIB_H
#define RONALVI_LIB_H

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ctime>

using namespace std;

namespace Warna {
    const string RESET   = "\033[0m";
    const string MERAH   = "\033[31m";
    const string HIJAU   = "\033[32m";
    const string KUNING  = "\033[33m";
    const string BIRU    = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN    = "\033[36m";
    const string PUTIH   = "\033[97m";
    const string BOLD    = "\033[1m";
    const string DIM     = "\033[2m";

    const string BG_MERAH  = "\033[41m";
    const string BG_HIJAU  = "\033[42m";
    const string BG_BIRU   = "\033[44m";
    const string BG_KUNING = "\033[43m";
}

string ambilWaktu() {
    time_t now = time(0);
    char buf[20];
    strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&now));
    return string(buf);
}

string ambilTanggal() {
    time_t now = time(0);
    char buf[30];
    strftime(buf, sizeof(buf), "%d-%m-%Y", localtime(&now));
    return string(buf);
}

string ambilTanggalWaktu() {
    time_t now = time(0);
    char buf[30];
    strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", localtime(&now));
    return string(buf);
}

struct LogEntry { string level, pesan, waktu; };
vector<LogEntry> logBuffer;

void catatLog(const string& level, const string& pesan) {
    logBuffer.push_back({level, pesan, ambilWaktu()});
}

void tampilLog() {
    if (logBuffer.empty()) { cout << "  (Tidak ada log)\n"; return; }
    cout << "\n";
    cout << Warna::BOLD << Warna::BIRU;
    cout << "========================================================================\n";
    cout << "                       SESSION LOG – " << ambilTanggal() << "\n";
    cout << "========================================================================\n";
    cout << Warna::RESET;
    for (auto& e : logBuffer) {
        string warna = (e.level == "ERROR") ? Warna::MERAH   :
                       (e.level == "OK")    ? Warna::HIJAU   :
                       (e.level == "WARN")  ? Warna::KUNING  : Warna::CYAN;
        string ikon  = (e.level == "ERROR") ? "✗" :
                       (e.level == "OK")    ? "✓" :
                       (e.level == "WARN")  ? "!" : "i";
        cout << "  " << Warna::DIM << e.waktu << Warna::RESET
             << " " << warna << "[" << ikon << " " << e.level << "]"
             << Warna::RESET << " " << e.pesan << "\n";
    }
    cout << Warna::DIM;
    cout << "------------------------------------------------------------------------\n";
    cout << "  Total log: " << logBuffer.size() << " entri\n";
    cout << "========================================================================\n";
    cout << Warna::RESET;
}

int validasiAngka(const string& input, const string& namaField) {
    if (input.empty())
        throw invalid_argument("Field '" + namaField + "' tidak boleh kosong!");
    for (char c : input) {
        if (c < '0' || c > '9')
            throw invalid_argument("Field '" + namaField + "' harus berupa angka! (input: '" + input + "')");
    }
    long long val = stoll(input);
    if (val < 0)
        throw out_of_range("Field '" + namaField + "' tidak boleh negatif!");
    if (val > 2000000000LL)
        throw out_of_range("Field '" + namaField + "' melebihi batas maksimal (2 Miliar)!");
    return (int)val;
}

string validasiTeks(const string& input, const string& namaField, int maxLen = 50) {
    if (input.empty())
        throw invalid_argument("Field '" + namaField + "' tidak boleh kosong!");
    if ((int)input.size() > maxLen)
        throw length_error("Field '" + namaField + "' terlalu panjang! (maks " + to_string(maxLen) + " karakter)");
    return input;
}

string validasiPlat(const string& input) {
    string plat = input;
    bool adaHuruf = false, adaAngka = false;
    for (char c : plat) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) adaHuruf = true;
        if (c >= '0' && c <= '9') adaAngka = true;
        if (c == ' ') continue;
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
              (c >= '0' && c <= '9') || c == ' '))
            throw invalid_argument("Plat nomor mengandung karakter tidak valid: '" + string(1,c) + "'");
    }
    if (!adaHuruf) throw invalid_argument("Plat nomor harus mengandung huruf!");
    if (!adaAngka) throw invalid_argument("Plat nomor harus mengandung angka!");
    return plat;
}

int bacaInt(const string& prompt);

int panjangStr(const string& s) { return (int)s.size(); }

string padStr(string s, int lebar) {
    int len = panjangStr(s);
    if (len >= lebar) return s.substr(0, lebar);
    return s + string(lebar - len, ' ');
}

string padInt(int angka, int lebar) {
    return padStr(to_string(angka), lebar);
}

string uangRupiah(int nilai) {
    string s = to_string(nilai);
    string hasil = "";
    int cnt = 0;
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        if (cnt > 0 && cnt % 3 == 0) hasil = "." + hasil;
        hasil = s[i] + hasil;
        cnt++;
    }
    return "Rp " + hasil;
}

#endif

#define MAX_SERVIS 100
#define MAX_USER   20
#define MAX_LOGIN  3
#define L          72

struct Kendaraan { string platNomor, merek, tipe; };
struct Pemilik   { string nama, noTelepon; };
struct Servis    {
    int idServis, biayaEstimasi, biayaFinal;
    Kendaraan kendaraan;
    Pemilik pemilik;
    string jenisServis;
    bool selesai;
};
struct User { string nama, nim, role; bool aktif; };

int    jumlahServis = 0; Servis daftarServis[MAX_SERVIS];
int    jumlahUser   = 0; User   daftarUser[MAX_USER];
User   userAktif;

void   cetakGaris(int n);
void   cetakGaris(int n, char simbol);
void   tampilkanPesan(const string& pesan, const string& tipe = "INFO");
void   judulKotak(const string& teks, int lebar = L);
void   pauseScreen();
void   cetakHeaderServis();
void   cetakBarisTabel(int no, const Servis& s);
void   cetakDaftarServis(int jml, Servis daftar[]);
void   cetakDaftarUser(int jml, User daftar[]);
int    hitungTotalBiaya(int idx, int jml, Servis daftar[], const string& namaPemilik);
int    faktorialDiskon(int n);
int    idBerikutnya(int jml, Servis daftar[]);

void cetakGaris(int n) {
    cout << Warna::DIM;
    for (int i = 0; i < n; i++) cout << '=';
    cout << Warna::RESET << "\n";
}
void cetakGaris(int n, char sim) {
    cout << Warna::DIM;
    for (int i = 0; i < n; i++) cout << sim;
    cout << Warna::RESET << "\n";
}

void tampilkanPesan(const string& pesan, const string& tipe) {
    string ikon, warna;
    if      (tipe == "OK")   { ikon = "✓"; warna = Warna::HIJAU; }
    else if (tipe == "ERR")  { ikon = "✗"; warna = Warna::MERAH; }
    else if (tipe == "WARN") { ikon = "!"; warna = Warna::KUNING; }
    else if (tipe == "INFO") { ikon = "i"; warna = Warna::CYAN; }
    else                     { ikon = "*"; warna = Warna::MAGENTA; }

    cout << "  " << Warna::DIM << ambilWaktu() << " " << Warna::RESET
         << warna << "[" << ikon << "]" << Warna::RESET
         << " " << pesan << "\n";
}

void judulKotak(const string& teks, int lebar) {
    cout << Warna::BOLD << Warna::BIRU;
    for (int i = 0; i < lebar; i++) cout << '=';
    cout << "\n";

    int spTeks = (lebar - (int)teks.size()) / 2;
    if (spTeks < 0) spTeks = 0;
    cout << string(spTeks, ' ') << teks << "\n";

    for (int i = 0; i < lebar; i++) cout << '=';
    cout << "\n" << Warna::RESET;
}

void cetakWaktuHeader(int lebar = L) {
    string waktuLabel = ambilTanggal() + "  " + ambilWaktu();
    int sp = (lebar - (int)waktuLabel.size() - 2) / 2;
    if (sp < 0) sp = 0;
    cout << string(sp, ' ')
         << Warna::KUNING << Warna::BOLD
         << "[" << waktuLabel << "]"
         << Warna::RESET << "\n";
    cetakGaris(lebar, '-');
}

void pauseScreen() {
    cout << "\n  " << Warna::DIM << "Tekan Enter untuk melanjutkan..."
         << Warna::RESET;
    cin.get();
}

void animasiLoading(const string& pesan, int langkah = 3) {
    cout << "  " << Warna::CYAN << pesan << Warna::RESET;
    for (int i = 0; i < langkah; i++) {
        cout << "." << flush;
        for (volatile long j = 0; j < 30000000L; j++);
    }
    cout << " " << Warna::HIJAU << "✓ OK!" << Warna::RESET << "\n";
}

void cetakHeaderServis() {
    cout << Warna::BOLD;
    cetakGaris(92);
    cout << padStr("No",4)  << padStr("ID",5)  << padStr("Plat",11)
         << padStr("Merek",11) << padStr("Tipe",7) << padStr("Pemilik",14)
         << padStr("Jenis",13) << padStr("Est.Biaya",13)
         << padStr("Byr Final",13) << padStr("Status",9) << "\n";
    cout << Warna::RESET;
    cetakGaris(92, '-');
}

void cetakBarisTabel(int no, const Servis& s) {
    string statusWarna = s.selesai ? Warna::HIJAU : Warna::KUNING;
    cout << padInt(no,4) << padInt(s.idServis,5)
         << padStr(s.kendaraan.platNomor,11) << padStr(s.kendaraan.merek,11)
         << padStr(s.kendaraan.tipe,7)       << padStr(s.pemilik.nama,14)
         << padStr(s.jenisServis,13)
         << padStr(uangRupiah(s.biayaEstimasi),13)
         << padStr(uangRupiah(s.biayaFinal),13)
         << statusWarna
         << padStr(s.selesai ? "[SELESAI]" : "[PROSES]", 9)
         << Warna::RESET << "\n";
}

void cetakDaftarServis(int jml, Servis daftar[]) {
    cetakHeaderServis();
    for (int i = 0; i < jml; i++) cetakBarisTabel(i+1, daftar[i]);
    cetakGaris(92);
}

void cetakDaftarUser(int jml, User daftar[]) {
    cetakGaris(65);
    cout << Warna::BOLD
         << padStr("No",5) << padStr("Nama",22) << padStr("NIM",18)
         << padStr("Role",8) << padStr("Status",12)
         << Warna::RESET << "\n";
    cetakGaris(65, '-');
    for (int i = 0; i < jml; i++) {
        string stWarna = daftar[i].aktif ? Warna::HIJAU : Warna::MERAH;
        cout << padInt(i+1,5) << padStr(daftar[i].nama,22)
             << padStr(daftar[i].nim,18) << padStr(daftar[i].role,8)
             << stWarna << padStr(daftar[i].aktif?"[AKTIF]":"[NONAKTIF]",12)
             << Warna::RESET << "\n";
    }
    cetakGaris(65);
}

int hitungTotalBiaya(int idx, int jml, Servis daftar[], const string& namaPemilik) {
    if (idx >= jml) return 0;
    int biaya = (daftar[idx].pemilik.nama == namaPemilik && daftar[idx].selesai)
                ? daftar[idx].biayaFinal : 0;
    return biaya + hitungTotalBiaya(idx+1, jml, daftar, namaPemilik);
}
int faktorialDiskon(int n) { return (n <= 1) ? 1 : n * faktorialDiskon(n-1); }
int idBerikutnya(int jml, Servis daftar[]) {
    int mx = 0;
    for (int i = 0; i < jml; i++) if (daftar[i].idServis > mx) mx = daftar[i].idServis;
    return mx + 1;
}

void swapServis(Servis *a, Servis *b) { Servis t=*a; *a=*b; *b=t; }

void sortByIdServis(Servis* d, int jml) {
    for (int i=1;i<jml;i++){
        Servis k=d[i]; int j=i-1;
        while(j>=0 && d[j].idServis>k.idServis){d[j+1]=d[j];j--;}
        d[j+1]=k;
    }
}
void insertionSortByNama(Servis d[], int jml) {
    for (int i=1;i<jml;i++){
        Servis k=d[i]; int j=i-1;
        while(j>=0 && d[j].pemilik.nama>k.pemilik.nama){d[j+1]=d[j];j--;}
        d[j+1]=k;
    }
}
void selectionSortByBiayaDesc(Servis d[], int jml) {
    for(int i=0;i<jml-1;i++){
        int im=i;
        for(int j=i+1;j<jml;j++) if(d[j].biayaFinal>d[im].biayaFinal) im=j;
        if(im!=i) swapServis(&d[i],&d[im]);
    }
}
void mergeByPlat(Servis d[], int ki, int te, int ka) {
    int uK=te-ki+1, uKa=ka-te;
    Servis *tK=new Servis[uK], *tKa=new Servis[uKa];
    for(int i=0;i<uK;i++)  tK[i]=d[ki+i];
    for(int j=0;j<uKa;j++) tKa[j]=d[te+1+j];
    int i=0,j=0,k=ki;
    while(i<uK&&j<uKa){
        if(tK[i].kendaraan.platNomor<=tKa[j].kendaraan.platNomor) d[k++]=tK[i++];
        else d[k++]=tKa[j++];
    }
    while(i<uK)  d[k++]=tK[i++];
    while(j<uKa) d[k++]=tKa[j++];
    delete[] tK; delete[] tKa;
}
void mergeSortByPlat(Servis d[], int ki, int ka) {
    if(ki<ka){ int t=ki+(ka-ki)/2; mergeSortByPlat(d,ki,t); mergeSortByPlat(d,t+1,ka); mergeByPlat(d,ki,t,ka); }
}

string toLowerManual(string s) {
    for (char& c : s) if(c>='A'&&c<='Z') c+=32;
    return s;
}
bool containsStr(const string& h, const string& n) {
    string hh=toLowerManual(h), nn=toLowerManual(n);
    if(nn.empty()) return true;
    if(nn.size()>hh.size()) return false;
    for(int i=0;i<=(int)(hh.size()-nn.size());i++){
        bool ok=true;
        for(int j=0;j<(int)nn.size();j++) if(hh[i+j]!=nn[j]){ok=false;break;}
        if(ok) return true;
    }
    return false;
}
int binarySearchById(Servis* d, int jml, int id) {
    int lo=0, hi=jml-1;
    while(lo<=hi){
        int mid=lo+(hi-lo)/2;
        if(d[mid].idServis==id) return mid;
        else if(d[mid].idServis<id) lo=mid+1;
        else hi=mid-1;
    }
    return -1;
}

int bacaInt(const string& prompt) {
    while (true) {
        if (!prompt.empty()) cout << prompt;
        string inp;
        getline(cin, inp);
        try {
            if (inp.empty()) throw invalid_argument("Input tidak boleh kosong!");
            for (char c : inp) {
                if (c < '0' || c > '9')
                    throw invalid_argument("Harus berupa angka bulat positif!");
            }
            return stoi(inp);
        } catch (const invalid_argument& e) {
            catatLog("WARN", "Input tidak valid: " + string(e.what()));
            cout << "  " << Warna::DIM << ambilWaktu() << " " << Warna::RESET
                 << Warna::KUNING << "[!] " << e.what()
                 << " Coba lagi." << Warna::RESET << "\n";
        } catch (const out_of_range&) {
            catatLog("WARN", "Input melebihi batas integer");
            cout << "  " << Warna::DIM << ambilWaktu() << " " << Warna::RESET
                 << Warna::KUNING << "[!] Angka terlalu besar! Coba lagi."
                 << Warna::RESET << "\n";
        }
    }
}

string bacaTeks(const string& prompt, const string& namaField, int maxLen=50) {
    while (true) {
        cout << prompt;
        string inp; getline(cin, inp);
        try {
            return validasiTeks(inp, namaField, maxLen);
        } catch (const length_error& e) {
            catatLog("WARN", string(e.what()));
            cout << "  " << Warna::DIM << ambilWaktu() << " " << Warna::RESET
                 << Warna::KUNING << "[!] " << e.what() << Warna::RESET << "\n";
        } catch (const invalid_argument& e) {
            catatLog("WARN", string(e.what()));
            cout << "  " << Warna::DIM << ambilWaktu() << " " << Warna::RESET
                 << Warna::KUNING << "[!] " << e.what() << Warna::RESET << "\n";
        }
    }
}

string bacaPlat(const string& prompt) {
    while (true) {
        cout << prompt;
        string inp; getline(cin, inp);
        try {
            return validasiPlat(inp);
        } catch (const invalid_argument& e) {
            catatLog("WARN", string(e.what()));
            cout << "  " << Warna::DIM << ambilWaktu() << " " << Warna::RESET
                 << Warna::KUNING << "[!] " << e.what() << Warna::RESET << "\n";
        }
    }
}

int bacaBiaya(const string& prompt) {
    while (true) {
        cout << prompt;
        string inp; getline(cin, inp);
        try {
            return validasiAngka(inp, "Biaya");
        } catch (const invalid_argument& e) {
            catatLog("WARN", string(e.what()));
            cout << "  " << Warna::DIM << ambilWaktu() << " " << Warna::RESET
                 << Warna::KUNING << "[!] " << e.what() << Warna::RESET << "\n";
        } catch (const out_of_range& e) {
            catatLog("WARN", string(e.what()));
            cout << "  " << Warna::DIM << ambilWaktu() << " " << Warna::RESET
                 << Warna::KUNING << "[!] " << e.what() << Warna::RESET << "\n";
        }
    }
}

void tampilInfoServisPtr(Servis* s) {
    cout << "  " << Warna::DIM << "[PTR->]" << Warna::RESET
         << " ID=" << s->idServis
         << "  Plat=" << s->kendaraan.platNomor
         << "  Pemilik=" << s->pemilik.nama
         << "  Status=" << (s->selesai
             ? Warna::HIJAU + string("SELESAI")
             : Warna::KUNING + string("PROSES"))
         << Warna::RESET << "\n";
}

void updateBiaya(int& biayaFinal, int& biayaEstimasi, int biayaBaru) {
    const int BIAYA_ADMIN = 5000;
    biayaEstimasi = biayaBaru;
    biayaFinal    = biayaBaru - BIAYA_ADMIN;
    if (biayaFinal < 0) {
        throw runtime_error("Biaya final tidak boleh negatif! "
            "Estimasi terlalu kecil (minimal Rp 5.001).");
    }
}

void updatePemilikPtr(Pemilik* p, const string& nm, const string& tlp) {
    p->nama = nm; p->noTelepon = tlp;
}

bool loginUser(User daftar[], int& jml, User& aktif) {
    judulKotak("MASUK AKUN  /  LOGIN");
    cetakWaktuHeader();
    int coba = 0;
    while (coba < MAX_LOGIN) {
        try {
            cout << "\n";
            string nama, nim;
            cout << "  Nama (username) : "; getline(cin, nama);
            cout << "  NIM  (password) : "; getline(cin, nim);

            if (nama.empty() || nim.empty())
                throw invalid_argument("Nama dan NIM tidak boleh kosong!");

            bool ketemu = false;
            for (int i = 0; i < jml; i++) {
                if (daftar[i].nama == nama && daftar[i].nim == nim) {
                    if (!daftar[i].aktif)
                        throw runtime_error("Akun '" + nama + "' dinonaktifkan. Hubungi admin.");
                    aktif = daftar[i];
                    ketemu = true;
                    break;
                }
            }
            if (!ketemu) throw runtime_error("Nama atau NIM salah!");

            animasiLoading("  Memverifikasi akun");
            catatLog("OK", "Login berhasil: " + aktif.nama + " [" + aktif.role + "]");
            tampilkanPesan("Selamat datang, " + Warna::BOLD + aktif.nama
                + Warna::RESET + "! [" + aktif.role + "]", "OK");
            return true;

        } catch (const invalid_argument& e) {
            coba++;
            catatLog("WARN", string(e.what()));
            tampilkanPesan(e.what(), "WARN");
        } catch (const runtime_error& e) {
            coba++;
            catatLog("ERROR", string(e.what()));
            tampilkanPesan(e.what(), "ERR");
        }
        int sisa = MAX_LOGIN - coba;
        if (sisa > 0)
            cout << "  " << Warna::DIM << "Sisa percobaan: " << sisa
                 << Warna::RESET << "\n";
    }
    catatLog("ERROR", "Batas login tercapai");
    tampilkanPesan("Batas login tercapai! Program berhenti.", "ERR");
    return false;
}

void registerUser(User daftar[], int& jml) {
    judulKotak("DAFTAR AKUN BARU  /  REGISTER");
    cetakWaktuHeader();
    try {
        if (jml >= MAX_USER)
            throw length_error("Kapasitas user penuh! Hubungi admin.");

        User u; u.role = "user"; u.aktif = true;
        u.nama = bacaTeks("  Nama (username) : ", "Nama", 30);
        u.nim  = bacaTeks("  NIM  (password) : ", "NIM", 20);

        for (int i = 0; i < jml; i++) {
            if (daftar[i].nim == u.nim)
                throw runtime_error("NIM '" + u.nim + "' sudah terdaftar!");
        }
        if ((int)u.nama.size() < 3)
            throw invalid_argument("Nama terlalu pendek (minimal 3 karakter)!");

        animasiLoading("  Mendaftarkan akun");
        daftar[jml++] = u;
        catatLog("OK", "Register berhasil: " + u.nama);
        tampilkanPesan("Registrasi berhasil! \"" + u.nama + "\" terdaftar sebagai Customer.", "OK");

    } catch (const length_error& e)    { catatLog("ERROR",e.what()); tampilkanPesan(e.what(),"ERR"); }
    catch  (const runtime_error& e)   { catatLog("ERROR",e.what()); tampilkanPesan(e.what(),"ERR"); }
    catch  (const invalid_argument& e){ catatLog("WARN",e.what());  tampilkanPesan(e.what(),"WARN"); }
}

void adminTambahServis(Servis daftar[], int& jml) {
    judulKotak("[ ADMIN ]  TAMBAH DATA SERVIS");
    cetakWaktuHeader();
    try {
        if (jml >= MAX_SERVIS)
            throw length_error("Kapasitas servis penuh! (maks " + to_string(MAX_SERVIS) + ")");

        int idx = jml;
        daftar[idx].idServis = idBerikutnya(jml, daftar);

        cout << "\n  " << Warna::BOLD << Warna::CYAN << "[ Kendaraan ]" << Warna::RESET << "\n";
        daftar[idx].kendaraan.platNomor = bacaPlat("  Plat Nomor         : ");
        daftar[idx].kendaraan.merek     = bacaTeks("  Merek              : ", "Merek", 20);

        cout << "  Tipe (Motor/Mobil)  : ";
        string tipe; getline(cin, tipe);
        tipe = toLowerManual(tipe);
        if (tipe != "motor" && tipe != "mobil")
            throw invalid_argument("Tipe harus 'Motor' atau 'Mobil'!");
        daftar[idx].kendaraan.tipe = tipe;

        cout << "\n  " << Warna::BOLD << Warna::CYAN << "[ Pemilik ]" << Warna::RESET << "\n";
        daftar[idx].pemilik.nama      = bacaTeks("  Nama Pemilik       : ", "Nama Pemilik", 30);
        daftar[idx].pemilik.noTelepon = bacaTeks("  No. Telepon        : ", "No. Telepon", 15);

        cout << "\n  " << Warna::BOLD << Warna::CYAN << "[ Servis ]" << Warna::RESET << "\n";
        daftar[idx].jenisServis   = bacaTeks("  Jenis Servis       : ", "Jenis Servis", 30);
        daftar[idx].biayaEstimasi = bacaBiaya("  Estimasi Biaya Rp  : ");
        daftar[idx].biayaFinal    = 0;
        daftar[idx].selesai       = false;
        jml++;

        animasiLoading("  Menyimpan data");
        catatLog("OK", "Servis ditambahkan ID=" + to_string(daftar[idx].idServis));
        tampilkanPesan("Data berhasil ditambahkan! (ID: " + to_string(daftar[idx].idServis) + ")", "OK");

    } catch (const length_error& e)    { catatLog("ERROR",e.what()); tampilkanPesan(e.what(),"ERR"); }
    catch  (const invalid_argument& e){ catatLog("WARN",e.what());  tampilkanPesan(e.what(),"WARN"); }
}

void adminUbahServis(Servis daftar[], int jml) {
    judulKotak("[ ADMIN ]  UBAH DATA SERVIS");
    cetakWaktuHeader();
    try {
        if (jml == 0) throw runtime_error("Belum ada data servis!");
        cetakDaftarServis(jml, daftar);

        int nomor = bacaInt("  Nomor urut yang diubah: ");
        if (nomor < 1 || nomor > jml)
            throw out_of_range("Nomor " + to_string(nomor) + " tidak valid! (1-" + to_string(jml) + ")");

        int idx = nomor - 1;
        cout << "\n  " << Warna::DIM << "(Enter = skip / tidak ubah)" << Warna::RESET << "\n\n";

        cout << "  " << Warna::BOLD << Warna::CYAN << "[ Ubah Kendaraan ]" << Warna::RESET << "\n";
        cout << "  Plat  [" << daftar[idx].kendaraan.platNomor << "]: ";
        string inp; getline(cin, inp);
        if (!inp.empty()) daftar[idx].kendaraan.platNomor = validasiPlat(inp);

        cout << "  Merek [" << daftar[idx].kendaraan.merek << "]: ";
        getline(cin, inp);
        if (!inp.empty()) daftar[idx].kendaraan.merek = validasiTeks(inp, "Merek", 20);

        cout << "\n  " << Warna::BOLD << Warna::CYAN << "[ Ubah Pemilik ]" << Warna::RESET << "\n";
        string nm, tlp;
        cout << "  Nama  [" << daftar[idx].pemilik.nama      << "]: "; getline(cin, nm);
        cout << "  Telp  [" << daftar[idx].pemilik.noTelepon << "]: "; getline(cin, tlp);
        if (nm.empty())  nm  = daftar[idx].pemilik.nama;
        if (tlp.empty()) tlp = daftar[idx].pemilik.noTelepon;
        updatePemilikPtr(&daftar[idx].pemilik, nm, tlp);

        cout << "\n  " << Warna::BOLD << Warna::CYAN << "[ Ubah Servis ]" << Warna::RESET << "\n";
        cout << "  Jenis [" << daftar[idx].jenisServis << "]: ";
        getline(cin, inp);
        if (!inp.empty()) daftar[idx].jenisServis = validasiTeks(inp,"Jenis Servis",30);

        cout << "  Biaya Final [" << uangRupiah(daftar[idx].biayaFinal) << "] (kosongkan=skip): ";
        getline(cin, inp);
        if (!inp.empty()) {
            int biayaBaru = validasiAngka(inp, "Biaya Final");
            updateBiaya(daftar[idx].biayaFinal, daftar[idx].biayaEstimasi, biayaBaru);
            tampilkanPesan("Potongan admin Rp5.000 diterapkan.", "INFO");
            tampilkanPesan("Biaya Final: " + uangRupiah(daftar[idx].biayaFinal), "INFO");
            daftar[idx].selesai = true;
            tampilkanPesan("Status otomatis → [SELESAI]", "INFO");
        }

        cout << "  Selesai manual (1=Ya / 0=Tidak / Enter=skip): ";
        getline(cin, inp);
        if (!inp.empty()) {
            if (inp != "0" && inp != "1")
                throw invalid_argument("Pilihan status harus 0 atau 1!");
            daftar[idx].selesai = (inp == "1");
        }

        catatLog("OK", "Data servis ID=" + to_string(daftar[idx].idServis) + " diubah");
        tampilkanPesan("Data berhasil diubah!", "OK");

    } catch (const out_of_range& e)   { catatLog("WARN",e.what());  tampilkanPesan(e.what(),"WARN"); }
    catch  (const runtime_error& e)   { catatLog("ERROR",e.what()); tampilkanPesan(e.what(),"ERR"); }
    catch  (const invalid_argument& e){ catatLog("WARN",e.what());  tampilkanPesan(e.what(),"WARN"); }
}

void adminHapusServis(Servis daftar[], int& jml) {
    judulKotak("[ ADMIN ]  HAPUS DATA SERVIS");
    cetakWaktuHeader();
    try {
        if (jml == 0) throw runtime_error("Belum ada data servis!");
        cetakDaftarServis(jml, daftar);

        int nomor = bacaInt("  Nomor urut yang dihapus: ");
        if (nomor < 1 || nomor > jml)
            throw out_of_range("Nomor " + to_string(nomor) + " tidak valid! (1-" + to_string(jml) + ")");

        cout << "  " << Warna::KUNING
             << "Yakin hapus ID=" << daftar[nomor-1].idServis
             << " (Pemilik: " << daftar[nomor-1].pemilik.nama << ")? [y/N]: "
             << Warna::RESET;
        string konfirm; getline(cin, konfirm);
        if (konfirm != "y" && konfirm != "Y")
            throw runtime_error("Penghapusan dibatalkan oleh pengguna.");

        string namaDihapus = daftar[nomor-1].pemilik.nama;
        int    idDihapus   = daftar[nomor-1].idServis;
        for (int i = nomor-1; i < jml-1; i++) daftar[i] = daftar[i+1];
        jml--;

        catatLog("WARN", "Servis ID=" + to_string(idDihapus) + " milik " + namaDihapus + " dihapus");
        tampilkanPesan("Data ID=" + to_string(idDihapus) + " berhasil dihapus!", "OK");

    } catch (const out_of_range& e)  { catatLog("WARN",e.what());  tampilkanPesan(e.what(),"WARN"); }
    catch  (const runtime_error& e)  { catatLog("INFO",e.what());  tampilkanPesan(e.what(),"INFO"); }
}

void adminTampilkanServis(Servis daftar[], int jml) {
    Servis salinan[MAX_SERVIS];
    for (int i = 0; i < jml; i++) salinan[i] = daftar[i];
    judulKotak("[ ADMIN ]  LIHAT DATA SERVIS");
    cetakWaktuHeader();
    try {
        if (jml == 0) throw runtime_error("Belum ada data servis!");
        cout << "\n  Tampilkan dengan urutan:\n";
        cetakGaris(L, '-');
        cout << "  " << Warna::CYAN << "1." << Warna::RESET << " Urutan Default\n"
             << "  " << Warna::CYAN << "2." << Warna::RESET << " Insertion Sort – Nama Pemilik (A→Z)\n"
             << "  " << Warna::CYAN << "3." << Warna::RESET << " Selection Sort – Biaya Final (Terbesar)\n"
             << "  " << Warna::CYAN << "4." << Warna::RESET << " Merge Sort – Plat Nomor (A→Z)\n";
        cetakGaris(L, '-');

        int pilih = bacaInt("  Pilihan: ");
        if (pilih < 1 || pilih > 4)
            throw out_of_range("Pilihan harus 1-4!");

        string label;
        switch (pilih) {
            case 1: label = "Default (urutan input)"; break;
            case 2: insertionSortByNama(salinan, jml);      label = "Insertion Sort – Nama Pemilik (A-Z)"; break;
            case 3: selectionSortByBiayaDesc(salinan, jml); label = "Selection Sort – Biaya Final (Terbesar)"; break;
            case 4: mergeSortByPlat(salinan, 0, jml-1);     label = "Merge Sort – Plat Nomor (A-Z)"; break;
        }
        animasiLoading("  Mengurutkan data");
        judulKotak("[ HASIL ]  " + label, 92);
        cetakDaftarServis(jml, salinan);
        cout << "  Total data: " << Warna::BOLD << jml << Warna::RESET << "\n";
        cout << "\n  " << Warna::DIM << "--- Ringkasan Pointer ---" << Warna::RESET << "\n";
        for (int i = 0; i < jml; i++) tampilInfoServisPtr(&salinan[i]);

    } catch (const runtime_error& e) { tampilkanPesan(e.what(),"ERR"); }
    catch  (const out_of_range& e)   { tampilkanPesan(e.what(),"WARN"); }
}

void menuSearchingAdmin(Servis* daftar, int jml) {
    judulKotak("[ ADMIN ]  CARI DATA SERVIS", 92);
    cetakWaktuHeader(92);
    try {
        if (jml == 0) throw runtime_error("Belum ada data servis!");
        cout << "\n  " << Warna::CYAN << "1." << Warna::RESET << " Binary Search  – ID Servis\n"
             << "  " << Warna::CYAN << "2." << Warna::RESET << " Linear Search  – Nama Pemilik\n"
             << "  " << Warna::DIM  << "0. Kembali\n" << Warna::RESET;
        cetakGaris(L, '-');

        int pilih = bacaInt("  Pilihan: ");
        if (pilih == 0) return;
        if (pilih < 0 || pilih > 2) throw invalid_argument("Pilihan tidak valid! (0-2)");

        if (pilih == 1) {
            judulKotak("[ BINARY SEARCH ]  Cari ID Servis", 92);
            Servis salinan[MAX_SERVIS];
            for(int i=0;i<jml;i++) salinan[i]=daftar[i];
            sortByIdServis(salinan, jml);
            cetakDaftarServis(jml, salinan);
            int targetId = bacaInt("  Masukkan ID Servis yang dicari: ");
            cout << "\n  " << Warna::DIM << "[PTR] Alamat array salinan: " << salinan << Warna::RESET << "\n";
            int hasil = binarySearchById(salinan, jml, targetId);
            cetakGaris(L,'=');
            if (hasil != -1) {
                cout << "  " << Warna::HIJAU << Warna::BOLD << "[BINARY SEARCH] DITEMUKAN!"
                     << Warna::RESET << " ID=" << targetId
                     << " pada indeks ke-" << hasil << "\n";
                cout << "  " << Warna::DIM << "[PTR] Alamat elemen: " << (salinan+hasil)
                     << Warna::RESET << "\n";
                cetakGaris(L,'-');
                cetakHeaderServis();
                cetakBarisTabel(1, salinan[hasil]);
                cetakGaris(92);
            } else {
                throw runtime_error("ID " + to_string(targetId) + " tidak ditemukan dalam data.");
            }
        }
        if (pilih == 2) {
            judulKotak("[ LINEAR SEARCH ]  Cari Nama Pemilik", 92);
            string keyword = bacaTeks("  Kata kunci nama pemilik: ", "Keyword", 30);
            int hasilIdx[MAX_SERVIS], jmlH = 0;
            cout << "\n  " << Warna::DIM << "[PTR] Alamat array daftar: " << daftar << Warna::RESET << "\n";
            for(int i=0;i<jml;i++)
                if(containsStr(daftar[i].pemilik.nama, keyword))
                    hasilIdx[jmlH++] = i;
            cetakGaris(L,'=');
            if (jmlH > 0) {
                cout << "  " << Warna::HIJAU << Warna::BOLD << "[LINEAR SEARCH] Ditemukan "
                     << jmlH << " data" << Warna::RESET << " keyword: \""
                     << Warna::KUNING << keyword << Warna::RESET << "\"\n";
                cetakGaris(L,'-');
                cetakHeaderServis();
                for(int i=0;i<jmlH;i++) {
                    cout << "  " << Warna::DIM << "[PTR] Alamat [" << hasilIdx[i] << "]: "
                         << (daftar+hasilIdx[i]) << Warna::RESET << "\n";
                    cetakBarisTabel(i+1, daftar[hasilIdx[i]]);
                }
                cetakGaris(92);
            } else {
                throw runtime_error("Tidak ada pemilik dengan nama mengandung \"" + keyword + "\".");
            }
        }

    } catch (const runtime_error& e)  { catatLog("INFO",e.what());  tampilkanPesan(e.what(),"WARN"); }
    catch  (const invalid_argument& e){ catatLog("WARN",e.what());  tampilkanPesan(e.what(),"WARN"); }
}

void menuSearchingUser(Servis* daftar, int jml, User aktif) {
    judulKotak("[ USER ]  CARI SERVIS SAYA", 92);
    cetakWaktuHeader(92);
    try {
        if (jml == 0) throw runtime_error("Belum ada data servis!");
        cout << "\n  " << Warna::CYAN << "1." << Warna::RESET << " Binary Search  – ID Servis\n"
             << "  " << Warna::CYAN << "2." << Warna::RESET << " Linear Search  – Jenis Servis\n"
             << "  " << Warna::DIM  << "0. Kembali\n" << Warna::RESET;
        cetakGaris(L, '-');
        int pilih = bacaInt("  Pilihan: ");
        if (pilih == 0) return;
        if (pilih < 0 || pilih > 2) throw invalid_argument("Pilihan tidak valid! (0-2)");

        if (pilih == 1) {
            Servis milikku[MAX_SERVIS]; int jmlMilikku=0;
            for(int i=0;i<jml;i++)
                if(daftar[i].pemilik.nama == aktif.nama)
                    milikku[jmlMilikku++] = daftar[i];
            if (jmlMilikku == 0)
                throw runtime_error("Anda belum memiliki data servis!");
            sortByIdServis(milikku, jmlMilikku);
            cetakDaftarServis(jmlMilikku, milikku);
            int targetId = bacaInt("  Masukkan ID Servis yang dicari: ");
            int hasil = binarySearchById(milikku, jmlMilikku, targetId);
            cetakGaris(L,'=');
            if (hasil != -1) {
                cout << "  " << Warna::HIJAU << Warna::BOLD << "[BINARY SEARCH] DITEMUKAN!"
                     << Warna::RESET << " ID=" << targetId << "\n";
                cetakHeaderServis(); cetakBarisTabel(1, milikku[hasil]); cetakGaris(92);
            } else {
                throw runtime_error("ID " + to_string(targetId) + " tidak ada di data Anda.");
            }
        }
        if (pilih == 2) {
            string keyword = bacaTeks("  Kata kunci jenis servis: ", "Keyword", 30);
            int hasilIdx[MAX_SERVIS], jmlH=0;
            for(int i=0;i<jml;i++)
                if(daftar[i].pemilik.nama==aktif.nama && containsStr(daftar[i].jenisServis,keyword))
                    hasilIdx[jmlH++] = i;
            cetakGaris(L,'=');
            if (jmlH > 0) {
                cout << "  " << Warna::HIJAU << Warna::BOLD << "[LINEAR SEARCH] Ditemukan "
                     << jmlH << " servis" << Warna::RESET << "\n";
                cetakHeaderServis();
                for(int i=0;i<jmlH;i++) cetakBarisTabel(i+1, daftar[hasilIdx[i]]);
                cetakGaris(92);
            } else {
                throw runtime_error("Tidak ada jenis servis \"" + keyword + "\" pada data Anda.");
            }
        }
    } catch (const runtime_error& e)  { catatLog("INFO",e.what());  tampilkanPesan(e.what(),"WARN"); }
    catch  (const invalid_argument& e){ catatLog("WARN",e.what());  tampilkanPesan(e.what(),"WARN"); }
}

void userLihatStatus(Servis daftar[], int jml, User aktif) {
    judulKotak("[ USER ]  STATUS SERVIS SAYA");
    cetakWaktuHeader();
    cout << Warna::BOLD
         << padStr("No",4) << padStr("Plat",11) << padStr("Merek",11)
         << padStr("Jenis",14) << padStr("Est.Biaya",14)
         << padStr("Biaya Final",14) << padStr("Status",12)
         << Warna::RESET << "\n";
    cetakGaris(80, '-');

    bool ada = false; int no = 1;
    for (int i = 0; i < jml; i++) {
        if (daftar[i].pemilik.nama == aktif.nama) {
            ada = true;
            string stWarna = daftar[i].selesai ? Warna::HIJAU : Warna::KUNING;
            cout << padInt(no++,4)
                 << padStr(daftar[i].kendaraan.platNomor,11)
                 << padStr(daftar[i].kendaraan.merek,11)
                 << padStr(daftar[i].jenisServis,14)
                 << padStr(uangRupiah(daftar[i].biayaEstimasi),14)
                 << padStr(uangRupiah(daftar[i].selesai ? daftar[i].biayaFinal : 0),14)
                 << stWarna << padStr(daftar[i].selesai?"[SELESAI]":"[PROSES]",12)
                 << Warna::RESET << "\n";
        }
    }
    cetakGaris(80);
    if (!ada) {
        tampilkanPesan("Belum ada data servis atas nama \"" + aktif.nama + "\".", "WARN");
    } else {
        int total      = hitungTotalBiaya(0, jml, daftar, aktif.nama);
        int kunjungan  = no - 1;
        cout << "\n  " << Warna::BOLD << "Total biaya final: "
             << Warna::HIJAU << uangRupiah(total) << Warna::RESET << "\n";
        if (kunjungan >= 2) {
            int diskon = faktorialDiskon(kunjungan > 5 ? 5 : kunjungan);
            cout << "  " << Warna::MAGENTA
                 << "Bonus loyalitas " << kunjungan
                 << "x servis! Poin diskon: " << diskon << " (info di kasir)"
                 << Warna::RESET << "\n";
        }
        tampilkanPesan("Biaya Final = Rp 0 artinya servis masih diproses.", "INFO");
    }
}

void userRequestServis(Servis daftar[], int& jml, User aktif) {
    judulKotak("[ USER ]  AJUKAN SERVIS KENDARAAN");
    cetakWaktuHeader();
    try {
        if (jml >= MAX_SERVIS)
            throw length_error("Kapasitas penuh. Hubungi admin.");

        int idx = jml;
        daftar[idx].idServis       = idBerikutnya(jml, daftar);
        daftar[idx].pemilik.nama   = aktif.nama;
        daftar[idx].pemilik.noTelepon = bacaTeks("  No. Telepon        : ", "No. Telepon", 15);

        cout << "\n  " << Warna::BOLD << Warna::CYAN << "[ Kendaraan ]" << Warna::RESET << "\n";
        daftar[idx].kendaraan.platNomor = bacaPlat("  Plat Nomor         : ");
        daftar[idx].kendaraan.merek     = bacaTeks("  Merek              : ", "Merek", 20);

        cout << "  Tipe (Motor/Mobil)  : ";
        string tipe; getline(cin, tipe);
        tipe = toLowerManual(tipe);
        if (tipe != "motor" && tipe != "mobil")
            throw invalid_argument("Tipe harus 'Motor' atau 'Mobil'!");
        daftar[idx].kendaraan.tipe = tipe;

        cout << "\n  " << Warna::BOLD << Warna::CYAN << "[ Servis ]" << Warna::RESET << "\n";
        daftar[idx].jenisServis   = bacaTeks("  Jenis Servis       : ", "Jenis Servis", 30);
        daftar[idx].biayaEstimasi = 0;
        daftar[idx].biayaFinal    = 0;
        daftar[idx].selesai       = false;
        jml++;

        animasiLoading("  Mengirim pengajuan");
        catatLog("OK", "Servis diajukan oleh " + aktif.nama + " ID=" + to_string(daftar[idx].idServis));
        tampilkanPesan("Servis berhasil diajukan! (ID: " + to_string(daftar[idx].idServis) + ")", "OK");
        tampilkanPesan("Admin akan segera memproses kendaraanmu yaaa KANBDAA KUHHH!!!!", "INFO");

    } catch (const length_error& e)    { catatLog("ERROR",e.what()); tampilkanPesan(e.what(),"ERR"); }
    catch  (const invalid_argument& e){ catatLog("WARN",e.what());  tampilkanPesan(e.what(),"WARN"); }
}

void adminLihatUser(User daftar[], int jml) {
    judulKotak("[ ADMIN ]  DAFTAR USER");
    cetakWaktuHeader();
    if (jml == 0) { tampilkanPesan("Belum ada user.", "WARN"); return; }
    cetakDaftarUser(jml, daftar);
    cout << "  Total user: " << Warna::BOLD << jml << Warna::RESET << "\n";
}
void adminTambahUser(User daftar[], int& jml, User aktif) {
    judulKotak("[ ADMIN ]  TAMBAH USER");
    cetakWaktuHeader();
    try {
        if (jml >= MAX_USER) throw length_error("Kapasitas user penuh!");
        User u; u.aktif = true;
        u.nama = bacaTeks("  Nama (username)        : ", "Nama", 30);
        u.nim  = bacaTeks("  NIM  (password)        : ", "NIM", 20);
        cout << "  Role (1=User, 2=Admin)  : ";
        int r = bacaInt("");
        if (r != 1 && r != 2) throw invalid_argument("Role harus 1 atau 2!");
        u.role = (r == 2) ? "admin" : "user";
        for (int i=0;i<jml;i++)
            if (daftar[i].nim == u.nim)
                throw runtime_error("NIM '" + u.nim + "' sudah digunakan!");
        daftar[jml++] = u;
        catatLog("OK", "User ditambahkan: " + u.nama + " [" + u.role + "]");
        tampilkanPesan("User \"" + u.nama + "\" berhasil ditambahkan sebagai " + u.role + "!", "OK");
    } catch (const length_error& e)    { catatLog("ERROR",e.what()); tampilkanPesan(e.what(),"ERR"); }
    catch  (const runtime_error& e)   { catatLog("ERROR",e.what()); tampilkanPesan(e.what(),"ERR"); }
    catch  (const invalid_argument& e){ catatLog("WARN",e.what());  tampilkanPesan(e.what(),"WARN"); }
}
void adminHapusUser(User daftar[], int& jml, User aktif) {
    judulKotak("[ ADMIN ]  HAPUS USER");
    cetakWaktuHeader();
    try {
        if (jml == 0) throw runtime_error("Belum ada user!");
        cetakDaftarUser(jml, daftar);
        int no = bacaInt("  Nomor user yang dihapus: ");
        if (no < 1 || no > jml)
            throw out_of_range("Nomor " + to_string(no) + " tidak valid!");
        if (daftar[no-1].nim == aktif.nim)
            throw runtime_error("Tidak bisa menghapus akun sendiri!");
        string nama = daftar[no-1].nama;
        for(int i=no-1;i<jml-1;i++) daftar[i]=daftar[i+1];
        jml--;
        catatLog("WARN", "User dihapus: " + nama);
        tampilkanPesan("User \"" + nama + "\" berhasil dihapus!", "OK");
    } catch (const out_of_range& e)  { catatLog("WARN",e.what());  tampilkanPesan(e.what(),"WARN"); }
    catch  (const runtime_error& e)  { catatLog("ERROR",e.what()); tampilkanPesan(e.what(),"ERR"); }
}
void adminKelolaUser(User daftar[], int& jml, User aktif) {
    judulKotak("[ ADMIN ]  KELOLA USER");
    cetakWaktuHeader();
    cout << "  " << Warna::CYAN << "1." << Warna::RESET << " Tambah User\n"
         << "  " << Warna::CYAN << "2." << Warna::RESET << " Hapus User\n"
         << "  " << Warna::DIM  << "0. Kembali\n" << Warna::RESET;
    cetakGaris(L,'-');
    int p = bacaInt("  Pilihan: ");
    if      (p==1) adminTambahUser(daftar,jml,aktif);
    else if (p==2) adminHapusUser(daftar,jml,aktif);
    else if (p!=0) tampilkanPesan("Pilihan tidak valid!","WARN");
}

void menuAdmin(Servis dftS[], int& jmlS, User dftU[], int& jmlU, User& aktif) {
    int p;
    do {
        cout << "\n";
        judulKotak("MENU ADMIN  –  BENGKEL DAENGGG RONALVI");
        cetakWaktuHeader();
        cout << "  " << Warna::CYAN << "User : " << Warna::BOLD << aktif.nama
             << Warna::RESET << Warna::CYAN << "  [" << aktif.role << "]"
             << Warna::RESET << "\n";
        cetakGaris(L,'-');
        cout << "  " << Warna::CYAN << "1." << Warna::RESET << " Lihat Data Servis\n"
             << "  " << Warna::CYAN << "2." << Warna::RESET << " Tambah Data Servis\n"
             << "  " << Warna::CYAN << "3." << Warna::RESET << " Ubah Data Servis\n"
             << "  " << Warna::CYAN << "4." << Warna::RESET << " Hapus Data Servis\n"
             << "  " << Warna::CYAN << "5." << Warna::RESET << " Lihat Daftar User\n"
             << "  " << Warna::CYAN << "6." << Warna::RESET << " Kelola User\n"
             << "  " << Warna::CYAN << "7." << Warna::RESET << " Cari Data Servis\n"
             << "  " << Warna::MERAH << "0. Logout\n" << Warna::RESET;
        cetakGaris(L);
        p = bacaInt("  Pilihan: ");
        switch(p) {
            case 1: adminTampilkanServis(dftS,jmlS);          pauseScreen(); break;
            case 2: adminTambahServis(dftS,jmlS);             pauseScreen(); break;
            case 3: adminUbahServis(dftS,jmlS);               pauseScreen(); break;
            case 4: adminHapusServis(dftS,jmlS);              pauseScreen(); break;
            case 5: adminLihatUser(dftU,jmlU);                pauseScreen(); break;
            case 6: adminKelolaUser(dftU,jmlU,aktif);         pauseScreen(); break;
            case 7: menuSearchingAdmin(dftS,jmlS);            pauseScreen(); break;
            case 0: catatLog("OK","Logout: "+aktif.nama);
                    tampilkanPesan("Sampai jumpa, " + aktif.nama + "!", "OK"); break;
            default: tampilkanPesan("Pilihan tidak valid! (0-8)", "WARN");
        }
    } while (p != 0);
}

void menuUser(Servis dftS[], int& jmlS, User& aktif) {
    int p;
    do {
        cout << "\n";
        judulKotak("MENU USER  –  BENGKEL DAENGGG RONALVI");
        cetakWaktuHeader();
        cout << "  " << Warna::CYAN << "User : " << Warna::BOLD << aktif.nama
             << Warna::RESET << Warna::CYAN << "  [" << aktif.role << "]"
             << Warna::RESET << "\n";
        cetakGaris(L,'-');
        cout << "  " << Warna::CYAN << "1." << Warna::RESET << " Ajukan Servis Kendaraan\n"
             << "  " << Warna::CYAN << "2." << Warna::RESET << " Lihat Status & Biaya Servis\n"
             << "  " << Warna::CYAN << "3." << Warna::RESET << " Cari Servis Saya\n"
             << "  " << Warna::MERAH << "0. Logout\n" << Warna::RESET;
        cetakGaris(L);
        p = bacaInt("  Pilihan: ");
        switch(p) {
            case 1: userRequestServis(dftS,jmlS,aktif); pauseScreen(); break;
            case 2: userLihatStatus(dftS,jmlS,aktif);   pauseScreen(); break;
            case 3: menuSearchingUser(dftS,jmlS,aktif); pauseScreen(); break;
            case 0: catatLog("OK","Logout: "+aktif.nama);
                    tampilkanPesan("See you, " + aktif.nama + "!", "OK"); break;
            default: tampilkanPesan("Pilihan tidak valid! (0-4)", "WARN");
        }
    } while (p != 0);
}

void cetakBanner() {
    cout << Warna::MERAH << Warna::BOLD;
    cout << "\n";
    cout << "  ██████╗ ███████╗███╗   ██╗ ██████╗ ██╗  ██╗███████╗██╗     \n";
    cout << "  ██╔══██╗██╔════╝████╗  ██║██╔════╝ ██║ ██╔╝██╔════╝██║     \n";
    cout << "  ██████╔╝█████╗  ██╔██╗ ██║██║  ███╗█████╔╝ █████╗  ██║     \n";
    cout << "  ██╔══██╗██╔══╝  ██║╚██╗██║██║   ██║██╔═██╗ ██╔══╝  ██║     \n";
    cout << "  ██████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██╗███████╗███████╗\n";
    cout << "  ╚═════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝\n";
    cout << Warna::RESET;
    cout << Warna::KUNING << Warna::BOLD;
    cout << "          D A E N G G G   R O N A L V I\n";
    cout << Warna::RESET;
    cout << Warna::DIM;
    cout << "\n";
    cout << Warna::RESET << Warna::CYAN << Warna::BOLD;
    cout << "                   Tanggal : " << ambilTanggal() << "\n";
    cout << "                   Waktu   : " << ambilWaktu()   << "\n";
    cout << Warna::RESET << "\n";
}

void menuAwal() {
    daftarUser[0] = {"ronalvi", "111", "admin", true};
    jumlahUser = 1;

    cetakBanner();
    animasiLoading("  Memuat sistem bengkel");
    catatLog("OK", "Sistem dimulai pada " + ambilTanggalWaktu());

    int p;
    do {
        cout << "\n";
        judulKotak("SELAMAT DATANG  –  BENGKEL DAENGGG RONALVI");
        cetakWaktuHeader();
        cout << "  " << Warna::CYAN  << "1." << Warna::RESET << " Login\n"
             << "  " << Warna::CYAN  << "2." << Warna::RESET << " Register (Customer)\n"
             << "  " << Warna::MERAH << "0. Keluar\n" << Warna::RESET;
        cetakGaris(L);
        p = bacaInt("  Pilihan: ");
        switch(p) {
            case 1:
                if (loginUser(daftarUser, jumlahUser, userAktif)) {
                    if (userAktif.role == "admin")
                        menuAdmin(daftarServis, jumlahServis, daftarUser, jumlahUser, userAktif);
                    else
                        menuUser(daftarServis, jumlahServis, userAktif);
                } else return;
                break;
            case 2: registerUser(daftarUser, jumlahUser); pauseScreen(); break;
            case 0:
                tampilLog();
                cout << "\n";
                catatLog("OK", "Sistem ditutup pada " + ambilTanggalWaktu());
                tampilkanPesan("Terima kasih yaaaa abang/mba kuhhhh! Sampai jumpa!", "OK");
                break;
            default: tampilkanPesan("Pilihan tidak valid! (0-2)", "WARN");
        }
    } while (p != 0);
}

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
    system("cls");
#endif
    menuAwal();
    return 0;
}