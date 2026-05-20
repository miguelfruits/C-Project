#pragma once
#include "Models.h"
#include "DataStore.h"
#include <string>
#include <vector>

class MovieManager {
public:
    MovieManager(DataStore& ds);
    std::vector<Movie> getAllMovies();
    std::vector<Movie> searchByGenre(const std::string& keyword);
    std::vector<Movie> getTopRated(int limit = 20);
    bool addMovie(const Movie& m);
    bool removeMovie(const std::string& title);
    void displayMovie(const Movie& m) const;
    void displayMovieList(const std::vector<Movie>& movies) const;
private:
    DataStore& db;
    Movie rowToMovie(const std::vector<std::string>& row) const;
};
