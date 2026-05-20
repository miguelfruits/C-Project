#include "AuthManager.h"
#include <algorithm>
#include <cctype>
#include <iostream>

AuthManager::AuthManager(DataStore& ds) : db(ds) {}

bool AuthManager::validateMobile(const std::string& mobile) {
    if (mobile.length() < 10 || mobile.length() > 15) {
        return false;
    }
    return std::all_of(mobile.begin(), mobile.end(),
                       [](unsigned char c) { return std::isdigit(c); });
}

User AuthManager::loginWithMobile(const std::string& mobile) {
    if (!validateMobile(mobile)) {
        std::cerr << "Error: Invalid mobile number. Must be 10-15 digits.\n";
        return User{-1, ""};
    }

    auto rows = db.preparedQuery(
        "SELECT id, mobile FROM users WHERE mobile = ?",
        {mobile});

    if (!rows.empty()) {
        int id = std::stoi(rows[0][0]);
        std::cout << "Welcome back!\n";
        return User{id, mobile};
    }

    db.preparedUpdate("INSERT INTO users (mobile) VALUES (?)", {mobile});
    int newId = db.getLastInsertId();
    std::cout << "Account created!\n";
    return User{newId, mobile};
}
