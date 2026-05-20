#include "FavouritesManager.h"
#include <iostream>
#include <string>

FavouritesManager::FavouritesManager(DataStore& ds) : db(ds) {}

bool FavouritesManager::addFavourite(int userId, int movieId) {
    auto existing = db.preparedQuery(
        "SELECT id FROM favorites WHERE user_id=? AND movie_id=?",
        {std::to_string(userId), std::to_string(movieId)}
    );

    if (!existing.empty()) {
        std::cout << "Already in favourites." << std::endl;
        return false;
    }

    db.preparedUpdate(
        "INSERT INTO favorites (user_id, movie_id) VALUES (?,?)",
        {std::to_string(userId), std::to_string(movieId)}
    );

    std::cout << "Added to favourites!" << std::endl;
    return true;
}

bool FavouritesManager::removeFavourite(int userId, int movieId) {
    auto existing = db.preparedQuery(
        "SELECT id FROM favorites WHERE user_id=? AND movie_id=?",
        {std::to_string(userId), std::to_string(movieId)}
    );

    if (existing.empty()) {
        std::cout << "Movie not in your favourites." << std::endl;
        return false;
    }

    db.preparedUpdate(
        "DELETE FROM favorites WHERE user_id=? AND movie_id=?",
        {std::to_string(userId), std::to_string(movieId)}
    );

    std::cout << "Removed from favourites." << std::endl;
    return true;
}

std::vector<Movie> FavouritesManager::getFavourites(int userId) {
    auto rows = db.preparedQuery(
        "SELECT m.id, m.title, m.genre, m.release_year, m.description, m.poster_path, m.avg_rating "
        "FROM favorites f JOIN movies m ON f.movie_id = m.id WHERE f.user_id = ?",
        {std::to_string(userId)}
    );

    std::vector<Movie> movies;
    for (const auto& row : rows) {
        if (row.size() < 7) continue;
        Movie m;
        m.id          = std::stoi(row[0]);
        m.title       = row[1];
        m.genre       = row[2];
        m.releaseYear = std::stoi(row[3]);
        m.description = row[4];
        m.posterPath  = row[5];
        m.avgRating   = std::stof(row[6]);
        movies.push_back(m);
    }

    if (movies.empty()) {
        std::cout << "Your favourites list is empty." << std::endl;
    }

    return movies;
}
