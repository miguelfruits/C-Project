#include "RecommendationEngine.h"
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <sstream>

RecommendationEngine::RecommendationEngine(DataStore& ds) : db(ds) {
    buildRelatedGenres();
}

void RecommendationEngine::buildRelatedGenres() {
    relatedGenres["Action"]    = {"Adventure", "Thriller"};
    relatedGenres["Sci-Fi"]    = {"Thriller", "Action", "Fantasy"};
    relatedGenres["Romance"]   = {"Drama", "Comedy"};
    relatedGenres["Horror"]    = {"Thriller", "Mystery"};
    relatedGenres["Comedy"]    = {"Romance", "Drama"};
    relatedGenres["Drama"]     = {"Romance", "Thriller"};
    relatedGenres["Thriller"]  = {"Action", "Horror", "Mystery"};
    relatedGenres["Marvel"]    = {"Action", "Sci-Fi", "Adventure"};
    relatedGenres["Animation"] = {"Comedy", "Fantasy", "Family"};
}

void RecommendationEngine::showGenreMenu() const {
    std::cout << "Select a genre:\n";
    int i = 1;
    for (const auto& entry : relatedGenres) {
        std::cout << "  " << i++ << ". " << entry.first << "\n";
    }
}

int RecommendationEngine::scoreMovie(const Movie& m, const std::string& selectedGenre,
                                     const std::vector<std::string>& related) const {
    // Convert movie genre and selected genre to lowercase for comparison
    std::string movieGenreLower = m.genre;
    std::transform(movieGenreLower.begin(), movieGenreLower.end(),
                   movieGenreLower.begin(), ::tolower);

    std::string selectedLower = selectedGenre;
    std::transform(selectedLower.begin(), selectedLower.end(),
                   selectedLower.begin(), ::tolower);

    if (movieGenreLower == selectedLower) {
        return 2;
    }

    for (const auto& rel : related) {
        std::string relLower = rel;
        std::transform(relLower.begin(), relLower.end(), relLower.begin(), ::tolower);
        if (movieGenreLower == relLower) {
            return 1;
        }
    }

    return 0;
}

std::vector<Movie> RecommendationEngine::getRecommendations(int userId, const std::string& genre) {
    // Step 1: Get user's already-rated movie IDs
    auto ratedRows = db.preparedQuery(
        "SELECT movie_id FROM ratings WHERE user_id = ?",
        {std::to_string(userId)}
    );

    std::unordered_set<int> ratedIds;
    for (const auto& row : ratedRows) {
        ratedIds.insert(std::stoi(row[0]));
    }

    // Step 2: Get related genres for the selected genre
    std::vector<std::string> related;
    auto it = relatedGenres.find(genre);
    if (it != relatedGenres.end()) {
        related = it->second;
    }

    // Step 3: Build SQL query with dynamic WHERE clause using LOWER() LIKE
    std::ostringstream whereClause;
    whereClause << "LOWER(genre) LIKE LOWER('" << genre << "')";
    for (const auto& rel : related) {
        whereClause << " OR LOWER(genre) LIKE LOWER('" << rel << "')";
    }

    std::string sql =
        "SELECT id, title, genre, release_year, description, poster_path, avg_rating "
        "FROM movies WHERE " + whereClause.str();

    auto rows = db.executeQuery(sql);

    // Step 4: Build scored list, excluding already-rated movies
    std::vector<std::pair<int, Movie>> scored; // {score, movie}

    for (const auto& row : rows) {
        Movie m;
        m.id          = std::stoi(row[0]);
        m.title       = row[1];
        m.genre       = row[2];
        m.releaseYear = std::stoi(row[3]);
        m.description = row[4];
        m.posterPath  = row[5];
        m.avgRating   = std::stof(row[6]);

        if (ratedIds.count(m.id)) {
            continue; // Skip movies the user has already rated
        }

        int score = scoreMovie(m, genre, related);
        scored.emplace_back(score, m);
    }

    // Step 5: Sort by score DESC, then avgRating DESC
    std::sort(scored.begin(), scored.end(),
        [](const std::pair<int, Movie>& a, const std::pair<int, Movie>& b) {
            if (a.first != b.first) return a.first > b.first;
            return a.second.avgRating > b.second.avgRating;
        }
    );

    // Step 6: Return top 10
    std::vector<Movie> result;
    int count = 0;
    for (const auto& entry : scored) {
        if (count >= 10) break;
        result.push_back(entry.second);
        ++count;
    }

    // Step 7: Print message if no results found
    if (result.empty()) {
        std::cout << "No recommendations found for genre: " << genre << std::endl;
    }

    return result;
}
