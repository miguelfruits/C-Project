#pragma once
#include "Models.h"
#include "DataStore.h"
#include <vector>

class FavouritesManager {
public:
    FavouritesManager(DataStore& ds);
    bool addFavourite(int userId, int movieId);
    bool removeFavourite(int userId, int movieId);
    std::vector<Movie> getFavourites(int userId);
private:
    DataStore& db;
};
