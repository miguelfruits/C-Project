#pragma once
#include "Models.h"
#include "DataStore.h"
#include <vector>

class RatingManager {
public:
    RatingManager(DataStore& ds);
    bool rateMovie(int userId, int movieId, int value);
    std::vector<Rating> getUserRatings(int userId);
    bool updateAvgRating(int movieId);
private:
    DataStore& db;
};
