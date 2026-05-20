#pragma once

#include <string>

// ============================================================
// WARNING: Do NOT commit this file to a public repository.
// DB_PASS, DB_USER, and ADMIN_MOBILE contain sensitive
// credentials. Use environment variables or a secrets manager
// in production, and add Config.h to your .gitignore.
// ============================================================

// Database connection settings
const std::string DB_HOST = "localhost";
const int         DB_PORT = 3306;
const std::string DB_NAME = "movie_rec_db";
const std::string DB_USER = "root";
const std::string DB_PASS = "";   // Fill in your MySQL password here

// Administrator mobile number (10-digit).
// The user whose mobile matches this constant is granted admin privileges.
const std::string ADMIN_MOBILE = "9999999999";
