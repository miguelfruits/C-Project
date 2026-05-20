#pragma once
#include "Models.h"
#include "DataStore.h"
#include <string>
#include <vector>
#include <map>

class RecommendationEngine {
public:
    RecommendationEngine(DataStore& ds);
    std::vector<Movie> getRecommendations(int userId, const std::string& genre);
    void showGenreMenu() const;
private:
    DataStore& db;
    std::map<std::string, std::vector<std::string>> relatedGenres;
    void buildRelatedGenres();
    int scoreMovie(const Movie& m, const std::string& selectedGenre,
                   const std::vector<std::string>& related) const;
};
