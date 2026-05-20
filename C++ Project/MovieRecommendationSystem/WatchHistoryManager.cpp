#include "WatchHistoryManager.h"
#include <iostream>

WatchHistoryManager::WatchHistoryManager(DataStore& ds) : db(ds) {}

bool WatchHistoryManager::recordView(int userId, int movieId) {
    int affected = db.preparedUpdate(
        "INSERT INTO watch_history (user_id, movie_id) VALUES (?,?)",
        {std::to_string(userId), std::to_string(movieId)}
    );
    if (affected < 0) {
        std::cerr << "Error: Failed to record view for user " << userId
                  << ", movie " << movieId << std::endl;
        return false;
    }
    return true;
}

std::vector<std::pair<Movie, std::string>> WatchHistoryManager::getRecentlyViewed(int userId, int limit) {
    auto rows = db.preparedQuery(
        "SELECT m.id, m.title, m.genre, m.release_year, m.description, "
        "m.poster_path, m.avg_rating, wh.viewed_at "
        "FROM watch_history wh "
        "JOIN movies m ON wh.movie_id = m.id "
        "WHERE wh.user_id = ? "
        "ORDER BY wh.viewed_at DESC "
        "LIMIT ?",
        {std::to_string(userId), std::to_string(limit)}
    );

    std::vector<std::pair<Movie, std::string>> result;

    if (rows.empty()) {
        std::cout << "No watch history found." << std::endl;
        return result;
    }

    for (const auto& row : rows) {
        Movie m;
        m.id          = std::stoi(row[0]);
        m.title       = row[1];
        m.genre       = row[2];
        m.releaseYear = std::stoi(row[3]);
        m.description = row[4];
        m.posterPath  = row[5];
        m.avgRating   = std::stof(row[6]);

        std::string timestamp = row[7];
        result.emplace_back(m, timestamp);
    }

    return result;
}
