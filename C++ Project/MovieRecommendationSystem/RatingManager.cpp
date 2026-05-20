#include "RatingManager.h"
#include <iostream>
#include <string>

RatingManager::RatingManager(DataStore& ds) : db(ds) {}

bool RatingManager::rateMovie(int userId, int movieId, int value) {
    if (value < 1 || value > 5) {
        std::cout << "Invalid rating. Please enter a value between 1 and 5." << std::endl;
        return false;
    }

    int result = db.preparedUpdate(
        "INSERT INTO ratings (user_id, movie_id, rating) VALUES (?,?,?) ON DUPLICATE KEY UPDATE rating=?",
        {std::to_string(userId), std::to_string(movieId), std::to_string(value), std::to_string(value)}
    );

    if (result >= 0) {
        updateAvgRating(movieId);
        std::cout << "Rating saved!" << std::endl;
        return true;
    }

    return false;
}

std::vector<Rating> RatingManager::getUserRatings(int userId) {
    auto rows = db.preparedQuery(
        "SELECT user_id, movie_id, rating FROM ratings WHERE user_id = ?",
        {std::to_string(userId)}
    );

    std::vector<Rating> ratings;
    for (const auto& row : rows) {
        if (row.size() < 3) continue;
        Rating r;
        r.userId  = std::stoi(row[0]);
        r.movieId = std::stoi(row[1]);
        r.value   = std::stoi(row[2]);
        ratings.push_back(r);
    }
    return ratings;
}

bool RatingManager::updateAvgRating(int movieId) {
    int result = db.executeUpdate(
        "UPDATE movies SET avg_rating = (SELECT AVG(rating) FROM ratings WHERE movie_id = " +
        std::to_string(movieId) + ") WHERE id = " + std::to_string(movieId)
    );
    return result >= 0;
}
