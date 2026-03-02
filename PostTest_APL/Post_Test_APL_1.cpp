#include <iostream>
using namespace std;

int main() {
    string nama, password;
    string namaBenar = "ronalvi";      
    string passBenar = "111";        
    int kesempatan = 0;
    bool loginBerhasil = false;

    while (kesempatan < 3) {
        cout << "____ YUK LOGIN DULUU SAHABAT _____" << endl;
        cout << "Masukkan Username     : ";
        cin >> nama;
        cout << "Masukkan Password : ";
        cin >> password;

        if (nama == namaBenar && password == passBenar) {
            loginBerhasil = true;
            break;
        } else {
            cout << "UserName/Password yang ada masukkan salahhhhhhhhh oiiii!" << endl;
            kesempatan++;
        }
    }

    if (!loginBerhasil) {
        cout << "Auehhhh anda telah mancapai batas login. Program berhenti." << endl;
        return 0;
    }

    int pilihan;
    do {
        cout << "\n_____ MENU KONVERSI WAKTU _____" << endl;
        cout << "1. Jam -> Menit & Detik" << endl;
        cout << "2. Menit -> Jam & Detik" << endl;
        cout << "3. Detik -> Jam & Menit" << endl;
        cout << "4. Keluar" << endl;
        cout << "Pilih menu : ";
        cin >> pilihan;

        if (pilihan == 1) {
            int jam;
            cout << "Masukkan Jam : ";
            cin >> jam;
            cout << jam << " Jam = "
                 << jam * 60 << " Menit dan "
                 << jam * 3600 << " Detik" << endl;
        }
        else if (pilihan == 2) {
            int menit;
            cout << "Masukkan Menit : ";
            cin >> menit;
            cout << menit << " Menit = "
                 << menit / 60 << " Jam dan "
                 << menit * 60 << " Detik" << endl;
        }
        else if (pilihan == 3) {
            int detik;
            cout << "Masukkan Detik : ";
            cin >> detik;
            cout << detik << " Detik = "
                 << detik / 3600 << " Jam dan "
                 << detik / 60 << " Menit" << endl;
        }
        else if (pilihan == 4) {
            cout << "Terima kasih telahh mancobai program saya abang/mbaa. Program selesai." << endl;
        }
        else {
            cout << "Pilihan yang abang/mbaa pilih tidak tersedia!" << endl;
        }

    } while (pilihan != 4);

    return 0;
}