#pragma once
#include "Models.h"
#include "DataStore.h"
#include "AuthManager.h"
#include "MovieManager.h"
#include "RatingManager.h"
#include "FavouritesManager.h"
#include "RecommendationEngine.h"
#include "WatchHistoryManager.h"

class MenuController {
public:
    MenuController(DataStore& ds, AuthManager& am, MovieManager& mm,
                   RatingManager& rm, FavouritesManager& fm,
                   RecommendationEngine& re, WatchHistoryManager& wh);
    void run();

private:
    DataStore& db;
    AuthManager& auth;
    MovieManager& movies;
    RatingManager& ratings;
    FavouritesManager& favourites;
    RecommendationEngine& recommender;
    WatchHistoryManager& watchHistory;
    User currentUser;  // id == -1 when no session

    void clearScreen();
    void showDashboard();
    void showAuthMenu();
    void showAdminMenu();
    void handleViewMovies();
    void handleSearchMovies();
    void handleAIRecommendations();
    void handleTopRated();
    void handleFavourites();
    void handleRateMovie();
    void routeChoice(int choice);
};
