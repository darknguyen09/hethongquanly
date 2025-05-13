#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>

class User;

// luu tru thong tin vi diem thuong
class Wallet {
public:
    std::string walletID; // id vi
    double balance; // so du vi
    std::vector<std::string> transactionLog; // lich su giao dich

    Wallet(std::string id, double b = 0.0);
};

void saveWallets(const std::vector<Wallet>& wallets, const std::string& relativeFilename); // luu danh sach vi
std::vector<Wallet> loadWallets(const std::string& relativeFilename); // tai danh sach vi

void transferPoints(User& sender, std::vector<User>& users, std::vector<Wallet>& wallets); // chuyen diem
void viewWallet(const User& user, const std::vector<Wallet>& wallets); // xem vi

#endif
