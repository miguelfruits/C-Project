#pragma once
#include "Models.h"
#include "DataStore.h"
#include <string>

class AuthManager {
public:
    AuthManager(DataStore& ds);
    // Returns populated User on success; User.id == -1 on invalid mobile
    User loginWithMobile(const std::string& mobile);
    // Returns true iff mobile is 10-15 digits only
    bool validateMobile(const std::string& mobile);
private:
    DataStore& db;
};
