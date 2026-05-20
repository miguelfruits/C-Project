#include "MovieManager.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

MovieManager::MovieManager(DataStore& ds) : db(ds) {}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

Movie MovieManager::rowToMovie(const std::vector<std::string>& row) const {
    Movie m;
    m.id          = std::stoi(row[0]);
    m.title       = row[1];
    m.genre       = row[2];
    m.releaseYear = std::stoi(row[3]);
    m.description = row[4];
    m.posterPath  = row[5];
    m.avgRating   = std::stof(row[6]);
    return m;
}

// ---------------------------------------------------------------------------
// Public methods
// ---------------------------------------------------------------------------

std::vector<Movie> MovieManager::getAllMovies() {
    auto rows = db.executeQuery(
        "SELECT id, title, genre, release_year, description, poster_path, avg_rating "
        "FROM movies ORDER BY title ASC");

    if (rows.empty()) {
        std::cout << "No movies available.\n";
        return {};
    }

    std::vector<Movie> movies;
    movies.reserve(rows.size());
    for (const auto& row : rows) {
        movies.push_back(rowToMovie(row));
    }
    return movies;
}

std::vector<Movie> MovieManager::searchByGenre(const std::string& keyword) {
    auto rows = db.preparedQuery(
        "SELECT id, title, genre, release_year, description, poster_path, avg_rating "
        "FROM movies WHERE LOWER(genre) LIKE LOWER(?)",
        {"%" + keyword + "%"});

    if (rows.empty()) {
        std::cout << "No movies found for genre: " << keyword << "\n";
        return {};
    }

    std::vector<Movie> movies;
    movies.reserve(rows.size());
    for (const auto& row : rows) {
        movies.push_back(rowToMovie(row));
    }
    return movies;
}

std::vector<Movie> MovieManager::getTopRated(int limit) {
    auto rows = db.preparedQuery(
        "SELECT id, title, genre, release_year, description, poster_path, avg_rating "
        "FROM movies ORDER BY avg_rating DESC LIMIT ?",
        {std::to_string(limit)});

    if (rows.empty()) {
        std::cout << "No rated movies available.\n";
        return {};
    }

    std::vector<Movie> movies;
    movies.reserve(rows.size());
    for (const auto& row : rows) {
        movies.push_back(rowToMovie(row));
    }
    return movies;
}

bool MovieManager::addMovie(const Movie& m) {
    if (m.title.empty()) {
        std::cout << "Error: Movie title cannot be empty.\n";
        return false;
    }
    if (m.genre.empty()) {
        std::cout << "Error: Movie genre cannot be empty.\n";
        return false;
    }

    int affected = db.preparedUpdate(
        "INSERT INTO movies (title, genre, release_year, description, poster_path, avg_rating) "
        "VALUES (?, ?, ?, ?, ?, ?)",
        {m.title,
         m.genre,
         std::to_string(m.releaseYear),
         m.description,
         m.posterPath,
         "0.0"});

    return affected > 0;
}

bool MovieManager::removeMovie(const std::string& title) {
    auto rows = db.preparedQuery(
        "SELECT id FROM movies WHERE title = ?",
        {title});

    if (rows.empty()) {
        std::cout << "Movie not found: " << title << "\n";
        return false;
    }

    const std::string movieId = rows[0][0];

    db.preparedUpdate("DELETE FROM ratings   WHERE movie_id = ?", {movieId});
    db.preparedUpdate("DELETE FROM favorites WHERE movie_id = ?", {movieId});
    db.preparedUpdate("DELETE FROM movies    WHERE id = ?",       {movieId});

    std::cout << "Movie removed successfully.\n";
    return true;
}

void MovieManager::displayMovie(const Movie& m) const {
    std::cout << "  ----------------------------------------\n";
    std::cout << "  Title  : " << m.title       << "\n";
    std::cout << "  Genre  : " << m.genre        << "\n";
    std::cout << "  Year   : " << m.releaseYear  << "\n";
    std::cout << "  Rating : " << std::fixed << std::setprecision(1)
              << m.avgRating << "/5.0\n";
    std::cout << "  Poster : "
              << (m.posterPath.empty() ? "No poster available" : m.posterPath)
              << "\n";
    std::cout << "  ----------------------------------------\n";
}

void MovieManager::displayMovieList(const std::vector<Movie>& movies) const {
    if (movies.empty()) {
        std::cout << "No movies to display.\n";
        return;
    }

    for (std::size_t i = 0; i < movies.size(); ++i) {
        std::cout << "\n  [" << (i + 1) << "]\n";
        displayMovie(movies[i]);
    }
}
