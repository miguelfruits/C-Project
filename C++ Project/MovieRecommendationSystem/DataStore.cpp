#include "DataStore.h"
#include "Config.h"

#include <mysql/jdbc.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// ============================================================
// connect()
// ============================================================
bool DataStore::connect() {
    try {
        driver = sql::mysql::get_mysql_driver_instance();

        const std::string url = "tcp://" + DB_HOST + ":" + std::to_string(DB_PORT);
        conn = driver->connect(url, DB_USER, DB_PASS);
        conn->setSchema(DB_NAME);

        return true;
    }
    catch (const sql::SQLException& e) {
        std::cerr << "[DataStore] connect() failed — host: " << DB_HOST
                  << ", port: " << DB_PORT
                  << ", db: "   << DB_NAME
                  << ", user: " << DB_USER
                  << "\n        Error: " << e.what()
                  << " (MySQL error code: " << e.getErrorCode()
                  << ", SQLState: " << e.getSQLState() << ")\n";
        conn = nullptr;
        return false;
    }
}

// ============================================================
// disconnect()
// ============================================================
void DataStore::disconnect() {
    try {
        if (conn != nullptr) {
            delete conn;
            conn = nullptr;
        }
    }
    catch (const sql::SQLException& e) {
        std::cerr << "[DataStore] disconnect() error: " << e.what() << "\n";
        conn = nullptr;
    }
}

// ============================================================
// executeQuery()
// ============================================================
std::vector<std::vector<std::string>> DataStore::executeQuery(const std::string& sql) {
    std::vector<std::vector<std::string>> rows;

    if (conn == nullptr) {
        std::cerr << "[DataStore] executeQuery() called with no active connection.\n";
        return rows;
    }

    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(sql));

        const unsigned int numCols = res->getMetaData()->getColumnCount();

        while (res->next()) {
            std::vector<std::string> row;
            row.reserve(numCols);
            for (unsigned int i = 1; i <= numCols; ++i) {
                // getString() returns an empty string for SQL NULL values
                row.push_back(res->getString(i).asStdString());
            }
            rows.push_back(std::move(row));
        }
    }
    catch (const sql::SQLException& e) {
        std::cerr << "[DataStore] executeQuery() error: " << e.what()
                  << " (MySQL error code: " << e.getErrorCode()
                  << ", SQLState: " << e.getSQLState() << ")\n";
    }

    return rows;
}

// ============================================================
// executeUpdate()
// ============================================================
int DataStore::executeUpdate(const std::string& sql) {
    if (conn == nullptr) {
        std::cerr << "[DataStore] executeUpdate() called with no active connection.\n";
        return -1;
    }

    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        int affected = stmt->executeUpdate(sql);
        return affected;
    }
    catch (const sql::SQLException& e) {
        std::cerr << "[DataStore] executeUpdate() error: " << e.what()
                  << " (MySQL error code: " << e.getErrorCode()
                  << ", SQLState: " << e.getSQLState() << ")\n";
        return -1;
    }
}

// ============================================================
// preparedQuery()
// ============================================================
std::vector<std::vector<std::string>> DataStore::preparedQuery(
    const std::string& sql,
    const std::vector<std::string>& params)
{
    std::vector<std::vector<std::string>> rows;

    if (conn == nullptr) {
        std::cerr << "[DataStore] preparedQuery() called with no active connection.\n";
        return rows;
    }

    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(sql));

        for (std::size_t i = 0; i < params.size(); ++i) {
            pstmt->setString(static_cast<int>(i + 1), params[i]);
        }

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        const unsigned int numCols = res->getMetaData()->getColumnCount();

        while (res->next()) {
            std::vector<std::string> row;
            row.reserve(numCols);
            for (unsigned int i = 1; i <= numCols; ++i) {
                row.push_back(res->getString(i).asStdString());
            }
            rows.push_back(std::move(row));
        }
    }
    catch (const sql::SQLException& e) {
        std::cerr << "[DataStore] preparedQuery() error: " << e.what()
                  << " (MySQL error code: " << e.getErrorCode()
                  << ", SQLState: " << e.getSQLState() << ")\n";
    }

    return rows;
}

// ============================================================
// preparedUpdate()
// ============================================================
int DataStore::preparedUpdate(
    const std::string& sql,
    const std::vector<std::string>& params)
{
    if (conn == nullptr) {
        std::cerr << "[DataStore] preparedUpdate() called with no active connection.\n";
        return -1;
    }

    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(sql));

        for (std::size_t i = 0; i < params.size(); ++i) {
            pstmt->setString(static_cast<int>(i + 1), params[i]);
        }

        int affected = pstmt->executeUpdate();

        // Cache the last insert id after any INSERT
        try {
            std::unique_ptr<sql::Statement> idStmt(conn->createStatement());
            std::unique_ptr<sql::ResultSet> idRes(
                idStmt->executeQuery("SELECT LAST_INSERT_ID()"));
            if (idRes->next()) {
                lastInsertId = idRes->getInt(1);
            }
        }
        catch (const sql::SQLException&) {
            // Non-fatal: last insert id simply won't be updated
        }

        return affected;
    }
    catch (const sql::SQLException& e) {
        std::cerr << "[DataStore] preparedUpdate() error: " << e.what()
                  << " (MySQL error code: " << e.getErrorCode()
                  << ", SQLState: " << e.getSQLState() << ")\n";
        return -1;
    }
}

// ============================================================
// getLastInsertId()
// ============================================================
int DataStore::getLastInsertId() {
    return lastInsertId;
}
