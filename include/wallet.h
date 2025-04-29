#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>

class User;

class Wallet {
public:
    std::string walletID;
    double balance;
    std::vector<std::string> transactionLog;

    Wallet(std::string id, double b = 0.0);
};

void saveWallets(const std::vector<Wallet>& wallets, const std::string& relativeFilename);
std::vector<Wallet> loadWallets(const std::string& relativeFilename);

void transferPoints(User& sender, std::vector<User>& users, std::vector<Wallet>& wallets);
void viewWallet(const User& user, const std::vector<Wallet>& wallets);

#endif
