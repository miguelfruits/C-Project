#pragma once
#include "Models.h"
#include "DataStore.h"
#include <vector>
#include <string>

class WatchHistoryManager {
public:
    WatchHistoryManager(DataStore& ds);
    bool recordView(int userId, int movieId);
    std::vector<std::pair<Movie, std::string>> getRecentlyViewed(int userId, int limit = 10);
private:
    DataStore& db;
};
