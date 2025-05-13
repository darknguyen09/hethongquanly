#include "user.h"
#include "wallet.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <limits>

using namespace std;

int main() {
    srand(time(0));
    // tai danh sach nguoi dung tu file data/user.txt
    vector<User> users = loadUsers("users.txt");
    // tai danh sach vi tu file data/wallets.txt
    vector<Wallet> wallets = loadWallets("wallets.txt");
    // kiem tra va tao vi cho nguoi dung neu chua co
    bool walletsChanged = false;
    unordered_map<string, bool> existingWallets;
    for (const auto& wallet : wallets) {
        existingWallets[wallet.walletID] = true;
    }
    for (const auto& user : users) {
        if (existingWallets.find(user.walletID) == existingWallets.end()) {
            if (!user.walletID.empty()) {
                // tao vi moi voi so du ban dau la 0
                wallets.emplace_back(user.walletID, 0.0);
                cout << "Da tao vi moi " << user.walletID << " cho nguoi dung " << user.username << endl;
                walletsChanged = true;
            } else {
                cerr << "Loi: Nguoi dung " << user.username << " khong co Wallet ID hop le." << endl;
            }
        }
    }
    // luu lai danh sach vi neu co thay doi
    if (walletsChanged) {
        saveWallets(wallets, "wallets.txt");
    }
    // kiem tra va tao tai khoan admin mac dinh neu chua ton tai
    bool adminExists = false;
    for (const auto& user : users) {
        if (user.username == "admin") {
            adminExists = true;
            break;
        }
    }
    if (!adminExists) {
        cout << "Admin mac dinh chua ton tai. Dang tao admin..." << endl;
        string adminPass = "admin123";
        string adminOathKey = generateOATHSecretKey();
        string hashedAdminPass = hashPassword(adminPass);
        if (!hashedAdminPass.empty()) {
            users.emplace_back("admin", hashedAdminPass, "Quan Tri Vien", "admin@gmail.com", "000000000", "admin", "W_admin_0", adminOathKey, true);
            cout << "Da tao tai khoan admin mac dinh:" << endl;
            cout << "  Username: admin" << endl;
            cout << "  Password: " << adminPass << " (Yeu cau doi ngay!)" << endl;
            cout << "  OATH Secret (Base32): " << adminOathKey << " (Them vao app xac thuc!)" << endl;
            if (existingWallets.find("W_admin_0") == existingWallets.end()) {
                wallets.emplace_back("W_admin_0", 0.0);
                saveWallets(wallets, "wallets.txt");
            }
            saveUsers(users, "users.txt");
        } else {
            cerr << "Loi: Khong the tao mat khau cho admin mac dinh." << endl;
        }
    }

    // menu va lua chon cho nguoi dung
    while (true) {
        // menu chinh
        cout << "\n========== HE THONG QUAN LY ==========" << endl;
        cout << "1. Dang nhap" << endl;
        cout << "2. Dang ky (Nguoi dung tu dang ky)" << endl;
        cout << "3. Thoat" << endl;
        cout << "======================================" << endl;
        cout << "Chon chuc nang: ";
        // kiem tra nhap vao hop le
        int choice;
        cin >> choice;
        if (cin.fail()) {
            cout << "Lua chon khong hop le. Vui long nhap so." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        User* currentUser = nullptr;

        if (choice == 1) {
            // dang nhap
            currentUser = login(users);
            if (!currentUser) {
                cout << "Vui long thu lai." << endl;
                continue;
            }
            // vong lap sau khi nguoi dung dang nhap
            while (currentUser != nullptr) {
                // menu nguoi dung
                cout << "\n---------- Chao " << currentUser->username << " (" << currentUser->role << ") ----------" << endl;
                cout << "1. Xem thong tin ca nhan" << endl;
                cout << "2. Cap nhat thong tin ca nhan" << endl;
                cout << "3. Doi mat khau" << endl;
                cout << "4. Quan ly vi diem thuong" << endl;
                int nextOption = 5;
                if (currentUser->role == "admin") {
                    cout << nextOption++ << ". Xem danh sach nguoi dung" << endl;
                    cout << nextOption++ << ". Tao tai khoan moi (cho nguoi dung)" << endl;
                    cout << nextOption++ << ". Chinh sua tai khoan nguoi dung" << endl;
                }
                cout << nextOption << ". Dang xuat" << endl;
                cout << "------------------------------------------" << endl;
                cout << "Chon chuc nang: ";
                int userChoice;
                cin >> userChoice;
                // kiem tra nhap vao hop le
                if (cin.fail()) {
                    cout << "Lua chon khong hop le. Vui long nhap so." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                // xu ly cac lua chon
                if (userChoice == 1) {
                    cout << "\n--- Thong tin ca nhan ---" << endl;
                    cout << "Username: " << currentUser->username << endl;
                    cout << "Full Name: " << currentUser->fullName << endl;
                    cout << "Email: " << currentUser->email << endl;
                    cout << "Phone: " << currentUser->phone << endl;
                    cout << "Role: " << currentUser->role << endl;
                    cout << "Wallet ID: " << currentUser->walletID << endl;
                    cout << "-------------------------" << endl;
                } else if (userChoice == 2) {
                    // cap nhat thong tin ca nhan
                    updateProfile(*currentUser, users);
                } else if (userChoice == 3) {
                    // doi passwd
                    changePassword(*currentUser, users);
                } else if (userChoice == 4) {
                    while (true) {
                        // menu vi
                        cout << "\n--- Quan ly Vi Diem Thuong ---" << endl;
                        cout << "1. Xem thong tin vi & lich su giao dich" << endl;
                        cout << "2. Chuyen diem" << endl;
                        cout << "3. Quay lai menu truoc" << endl;
                        cout << "------------------------------" << endl;
                        cout << "Chon: ";
                        int walletChoice;
                        cin >> walletChoice;
                        if (cin.fail()) {
                            cout << "Lua chon khong hop le." << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            continue;
                        }
                        if (walletChoice == 1) {
                            viewWallet(*currentUser, wallets);
                        } else if (walletChoice == 2) {
                            transferPoints(*currentUser, users, wallets);
                        } else if (walletChoice == 3) {
                            break;
                        } else {
                            cout << "Lua chon khong hop le." << endl;
                        }
                    }
                } else if (userChoice >= 5) {
                    int logoutOption = 5;
                    if (currentUser->role == "admin") {
                        if (userChoice == logoutOption++) {
                            viewUsers(users);
                        } else if (userChoice == logoutOption++) {
                            cout << "\n--- Tao tai khoan moi (Admin) ---" << endl;
                            User newUser = registerUser(users, true);
                            if (!newUser.username.empty()) {
                                bool newWalletNeeded = true;
                                for (const auto& w : wallets) {
                                    if (w.walletID == newUser.walletID) {
                                        newWalletNeeded = false;
                                        break;
                                    }
                                }
                                if (newWalletNeeded && !newUser.walletID.empty()) {
                                    wallets.emplace_back(newUser.walletID, 0.0);
                                    saveWallets(wallets, "wallets.txt");
                                    cout << "Da tu dong tao vi " << newUser.walletID << " cho nguoi dung moi." << endl;
                                }
                                saveUsers(users, "users.txt");
                            } else {
                                cout << "Tao tai khoan moi that bai." << endl;
                            }
                        } else if (userChoice == logoutOption++) {
                            adminEditUser(users);
                        }
                    }
                    if (userChoice == logoutOption) {
                        cout << "Dang xuat..." << endl;
                        currentUser = nullptr;
                        break;
                    } else if (userChoice > logoutOption) {
                        cout << "Lua chon khong hop le." << endl;
                    }
                } else {
                    cout << "Lua chon khong hop le." << endl;
                }
            }
        } else if (choice == 2) {
            // dang ky tai khoan moi
            cout << "\n--- Dang ky tai khoan moi ---" << endl;
            User newUser = registerUser(users);
            if (!newUser.username.empty()) {
                bool newWalletNeeded = true;
                for (const auto& w : wallets) {
                    if (w.walletID == newUser.walletID) {
                        newWalletNeeded = false;
                        break;
                    }
                }
                if (newWalletNeeded && !newUser.walletID.empty()) {
                    wallets.emplace_back(newUser.walletID, 0.0);
                    saveWallets(wallets, "wallets.txt");
                    cout << "Da tu dong tao vi " << newUser.walletID << " cho ban." << endl;
                }
                saveUsers(users, "users.txt");
            } else {
                cout << "Dang ky that bai (Co the do ten dang nhap da ton tai hoac loi khac)." << endl;
            }
        } else if (choice == 3) {
            // thoat chuong trinh
            cout << "Tam biet!" << endl;
            break;
        } else {
            cout << "Lua chon khong hop le. Vui long chon lai." << endl;
        }
    }
    return 0;
}
