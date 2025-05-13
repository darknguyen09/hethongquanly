#include "wallet.h"
#include "user.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>

using namespace std;

const string DATA_DIR = "data/"; // thu muc luu du lieu cho vi: wallets.txt

// khoi tao vi voi ID va so du
Wallet::Wallet(string id, double b) : walletID(id), balance(b) {}

// luu danh sach vi vao file ../data/wallets.txt
void saveWallets(const vector<Wallet>& wallets, const string& relativeFilename) {
    string filename = DATA_DIR + relativeFilename;
    string backupFilename = filename + ".bak";
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Loi: Khong the mo file " << filename << " de ghi (Dam bao thu muc 'data/' ton tai)." << endl;
        return;
    }
    for (const auto& wallet : wallets) {
        file << wallet.walletID << "," << wallet.balance << "\n";
        for (const auto& log : wallet.transactionLog) {
            file << "LOG:" << log << "\n";
        }
    }
    file.close();
    string backupCmd = "cp \"" + filename + "\" \"" + backupFilename + "\"";
#ifdef _WIN32
    backupCmd = "copy \"" + filename + "\" \"" + backupFilename + "\"";
#endif
    int result = system(backupCmd.c_str());
    if (result != 0) {
        cerr << "Canh bao: Khong the tao file backup cho " << filename << ". Ma loi: " << result << endl;
    }
}

// tai danh sach vi tu file ../data/wallets.txt
vector<Wallet> loadWallets(const string& relativeFilename) {
    string filename = DATA_DIR + relativeFilename;
    string backupFilename = filename + ".bak";
    vector<Wallet> wallets;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Canh bao: Khong the mo file " << filename << ". Dang thu doc tu file backup " << backupFilename << endl;
        ifstream backupFile(backupFilename);
        if (backupFile.is_open()) {
            file.swap(backupFile);
        } else {
            cerr << "Canh bao: Khong the mo ca file chinh va file backup trong 'data/'. Bat dau voi danh sach trong." << endl;
            return wallets;
        }
    }
    string line;
    Wallet* currentWallet = nullptr;
    while (getline(file, line)) {
        if (line.rfind("LOG:", 0) == 0) {
            if (currentWallet && line.length() > 4) {
                currentWallet->transactionLog.push_back(line.substr(4));
            }
        } else {
            size_t pos = line.find(",");
            if (pos != string::npos) {
                string walletID = line.substr(0, pos);
                try {
                    double balance = stod(line.substr(pos + 1));
                    wallets.emplace_back(walletID, balance);
                    currentWallet = &wallets.back();
                } catch (const invalid_argument& ia) {
                    cerr << "Canh bao: Bo qua dong khong hop le trong wallets.txt (khong phai so): " << line << endl;
                    currentWallet = nullptr;
                } catch (const out_of_range& oor) {
                    cerr << "Canh bao: Bo qua dong khong hop le trong wallets.txt (so qua lon): " << line << endl;
                    currentWallet = nullptr;
                }
            } else {
                cerr << "Canh bao: Bo qua dong khong hop le trong wallets.txt (thieu dau phay): " << line << endl;
                currentWallet = nullptr;
            }
        }
    }
    file.close();
    return wallets;
}

// chuyen diem giua cac vi
void transferPoints(User& sender, vector<User>& users, vector<Wallet>& wallets) {
    string destWalletID;
    double points;
    cout << "Nhap ma vi dich (walletID): ";
    cin >> destWalletID;
    cout << "Nhap so diem can chuyen: ";
    cin >> points;

    if (points <= 0) {
        cout << "So diem chuyen phai lon hon 0.\n";
        return;
    }

    Wallet* senderWallet = nullptr;
    Wallet* destWallet = nullptr;
    for (auto& wallet : wallets) {
        if (wallet.walletID == sender.walletID) senderWallet = &wallet;
        if (wallet.walletID == destWalletID) destWallet = &wallet;
    }

    if (!senderWallet) {
        cerr << "Loi: Khong tim thay vi cua nguoi gui!\n";
        return;
    }
    if (!destWallet) {
        cout << "Vi dich khong ton tai!\n";
        return;
    }
    if (senderWallet == destWallet) {
        cout << "Ban khong the chuyen diem cho chinh minh.\n";
        return;
    }
    if (senderWallet->balance < points) {
        cout << "So du khong du! (So du hien tai: " << senderWallet->balance << ")\n";
        return;
    }

    string destUsername = "Khong ro";
    for (const auto& u : users) {
        if (u.walletID == destWalletID) {
            destUsername = u.username;
            break;
        }
    }

    cout << "Ban chuan bi chuyen " << points << " diem den vi " << destWalletID << " (cua user: " << destUsername << ").\n";
    string otp = generateSimulatedOTP();
    cout << "Ma OTP: " << otp << "\n";
    string inputOtp;
    cout << "Nhap ma OTP de xac nhan giao dich: ";
    cin >> inputOtp;

    if (inputOtp == otp) {
        senderWallet->balance -= points;
        destWallet->balance += points;
        time_t now = time(0);
        string dt = ctime(&now);
        dt.pop_back();
        string logSender = "[" + dt + "] Chuyen " + to_string(points) + " diem toi " + destWallet->walletID + " (User: " + destUsername + ")";
        string logDest = "[" + dt + "] Nhan " + to_string(points) + " diem tu " + senderWallet->walletID + " (User: " + sender.username + ")";
        senderWallet->transactionLog.push_back(logSender);
        destWallet->transactionLog.push_back(logDest);
        saveWallets(wallets, "wallets.txt");
        cout << "Chuyen diem thanh cong!\n";
    } else {
        cout << "Ma OTP khong dung! Giao dich huy bo.\n";
    }
}

// xem thong tin vi (so du, lich su giao dich)
void viewWallet(const User& user, const vector<Wallet>& wallets) {
    bool found = false;
    for (const auto& wallet : wallets) {
        if (wallet.walletID == user.walletID) {
            cout << "\n--- Thong tin vi ---" << endl;
            cout << "Ma vi: " << wallet.walletID << "\n";
            cout << "So du: " << fixed << setprecision(2) << wallet.balance << " diem\n";
            cout << "Lich su giao dich:\n";
            if (wallet.transactionLog.empty()) {
                cout << "  (Chua co giao dich nao)\n";
            } else {
                for (auto it = wallet.transactionLog.rbegin(); it != wallet.transactionLog.rend(); ++it) {
                    cout << "  - " << *it << "\n";
                }
            }
            cout << "--------------------" << endl;
            found = true;
            return;
        }
    }
    if (!found) {
        cerr << "Loi: Khong tim thay vi lien ket voi tai khoan nay (" << user.walletID << ")." << endl;
    }
}
