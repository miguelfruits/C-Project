#include "DataStore.h"
#include "AuthManager.h"
#include "MovieManager.h"
#include "RatingManager.h"
#include "FavouritesManager.h"
#include "RecommendationEngine.h"
#include "WatchHistoryManager.h"
#include "MenuController.h"
#include <iostream>

int main() {
    DataStore db;
    if (!db.connect()) {
        std::cerr << "Failed to connect to database. Exiting.\n";
        return 1;
    }

    AuthManager        auth(db);
    MovieManager       movies(db);
    RatingManager      ratings(db);
    FavouritesManager  favourites(db);
    RecommendationEngine recommender(db);
    WatchHistoryManager  watchHistory(db);

    MenuController menu(db, auth, movies, ratings, favourites, recommender, watchHistory);
    menu.run();

    db.disconnect();
    return 0;
}
