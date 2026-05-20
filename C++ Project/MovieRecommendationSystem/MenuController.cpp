#include "MenuController.h"
#include "Config.h"
#include <iostream>
#include <string>
#include <limits>
#include <vector>

// ─────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────
MenuController::MenuController(DataStore& ds, AuthManager& am, MovieManager& mm,
                               RatingManager& rm, FavouritesManager& fm,
                               RecommendationEngine& re, WatchHistoryManager& wh)
    : db(ds), auth(am), movies(mm), ratings(rm),
      favourites(fm), recommender(re), watchHistory(wh)
{
    currentUser.id = -1;  // no session
}

// ─────────────────────────────────────────────
// Utility
// ─────────────────────────────────────────────
void MenuController::clearScreen() {
    system("cls");
}

// ─────────────────────────────────────────────
// Dashboard
// ─────────────────────────────────────────────
void MenuController::showDashboard() {
    std::cout << "=================================\n";
    std::cout << " AI MOVIE RECOMMENDATION SYSTEM\n";
    std::cout << "=================================\n";

    if (currentUser.id != -1) {
        std::cout << " Logged in: " << currentUser.mobile << "\n";
        std::cout << "---------------------------------\n";
        std::cout << "  1. Logout\n";
    } else {
        std::cout << "  1. Sign In\n";
    }

    std::cout << "  2. View Movies\n";
    std::cout << "  3. Search Movies\n";
    std::cout << "  4. AI Recommendations\n";
    std::cout << "  5. Top Rated Movies\n";
    std::cout << "  6. Favorite Movies\n";
    std::cout << "  7. Exit\n";

    if (currentUser.id != -1 && currentUser.mobile == ADMIN_MOBILE) {
        std::cout << "  8. Admin Menu\n";
    }

    std::cout << "=================================\n";
    std::cout << "Enter your choice: ";
}

// ─────────────────────────────────────────────
// Authentication
// ─────────────────────────────────────────────
void MenuController::showAuthMenu() {
    std::string mobile;
    std::cout << "Enter your mobile number (10-15 digits): ";
    std::cin >> mobile;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    User user = auth.loginWithMobile(mobile);
    if (user.id != -1) {
        currentUser = user;
        std::cout << "Login successful! Welcome, " << currentUser.mobile << ".\n";
    } else {
        std::cout << "Invalid mobile number or login failed. Please try again.\n";
    }
}

// ─────────────────────────────────────────────
// View Movies
// ─────────────────────────────────────────────
void MenuController::handleViewMovies() {
    std::vector<Movie> result = movies.getAllMovies();
    movies.displayMovieList(result);

    if (result.empty()) {
        return;
    }

    if (currentUser.id != -1) {
        // Record a view for every movie displayed
        for (const auto& m : result) {
            watchHistory.recordView(currentUser.id, m.id);
        }

        std::cout << "\nEnter movie number to rate (0 to skip): ";
        int choice = 0;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice >= 1 && choice <= static_cast<int>(result.size())) {
            const Movie& selected = result[choice - 1];
            std::cout << "Enter rating for \"" << selected.title << "\" (1-5): ";
            int ratingValue = 0;
            std::cin >> ratingValue;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (ratingValue >= 1 && ratingValue <= 5) {
                if (ratings.rateMovie(currentUser.id, selected.id, ratingValue)) {
                    std::cout << "Rating saved successfully.\n";
                } else {
                    std::cout << "Failed to save rating.\n";
                }
            } else {
                std::cout << "Invalid rating. Must be between 1 and 5.\n";
            }
        }
    }
}

// ─────────────────────────────────────────────
// Search Movies
// ─────────────────────────────────────────────
void MenuController::handleSearchMovies() {
    std::string keyword;
    std::cout << "Enter genre or movie name to search: ";
    std::getline(std::cin, keyword);

    std::vector<Movie> result = movies.searchByGenre(keyword);
    movies.displayMovieList(result);
}

// ─────────────────────────────────────────────
// AI Recommendations
// ─────────────────────────────────────────────
void MenuController::handleAIRecommendations() {
    if (currentUser.id == -1) {
        std::cout << "Please sign in to get AI recommendations.\n";
        return;
    }

    recommender.showGenreMenu();
    std::cout << "Enter genre number: ";
    int genreChoice = 0;
    std::cin >> genreChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Genre list matches std::map alphabetical order from RecommendationEngine
    // Keys: Action, Animation, Comedy, Drama, Horror, Marvel, Romance, Sci-Fi, Thriller
    const std::vector<std::string> genreList = {
        "Action", "Animation", "Comedy", "Drama", "Horror",
        "Marvel", "Romance", "Sci-Fi", "Thriller"
    };

    if (genreChoice < 1 || genreChoice > static_cast<int>(genreList.size())) {
        std::cout << "Invalid genre selection.\n";
        return;
    }

    std::string selectedGenre = genreList[genreChoice - 1];
    std::vector<Movie> result = recommender.getRecommendations(currentUser.id, selectedGenre);
    movies.displayMovieList(result);
}

// ─────────────────────────────────────────────
// Top Rated
// ─────────────────────────────────────────────
void MenuController::handleTopRated() {
    std::vector<Movie> result = movies.getTopRated(20);
    movies.displayMovieList(result);
}

// ─────────────────────────────────────────────
// Favourites
// ─────────────────────────────────────────────
void MenuController::handleFavourites() {
    if (currentUser.id == -1) {
        std::cout << "Please sign in to view favourites.\n";
        return;
    }

    std::cout << "  1. View My Favourites\n";
    std::cout << "  2. Add a Favourite\n";
    std::cout << "  3. Remove a Favourite\n";
    std::cout << "  4. Back\n";
    std::cout << "Enter your choice: ";

    int choice = 0;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1: {
            std::vector<Movie> favList = favourites.getFavourites(currentUser.id);
            movies.displayMovieList(favList);
            break;
        }
        case 2: {
            std::vector<Movie> allMovies = movies.getAllMovies();
            movies.displayMovieList(allMovies);
            if (allMovies.empty()) break;

            std::cout << "Enter movie number to add to favourites: ";
            int movieChoice = 0;
            std::cin >> movieChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (movieChoice >= 1 && movieChoice <= static_cast<int>(allMovies.size())) {
                const Movie& selected = allMovies[movieChoice - 1];
                if (favourites.addFavourite(currentUser.id, selected.id)) {
                    std::cout << "\"" << selected.title << "\" added to favourites.\n";
                } else {
                    std::cout << "Failed to add to favourites (already added?).\n";
                }
            } else {
                std::cout << "Invalid selection.\n";
            }
            break;
        }
        case 3: {
            std::vector<Movie> favList = favourites.getFavourites(currentUser.id);
            movies.displayMovieList(favList);
            if (favList.empty()) break;

            std::cout << "Enter movie number to remove from favourites: ";
            int movieChoice = 0;
            std::cin >> movieChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (movieChoice >= 1 && movieChoice <= static_cast<int>(favList.size())) {
                const Movie& selected = favList[movieChoice - 1];
                if (favourites.removeFavourite(currentUser.id, selected.id)) {
                    std::cout << "\"" << selected.title << "\" removed from favourites.\n";
                } else {
                    std::cout << "Failed to remove from favourites.\n";
                }
            } else {
                std::cout << "Invalid selection.\n";
            }
            break;
        }
        case 4:
            // Back — do nothing
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
    }
}

// ─────────────────────────────────────────────
// Rate Movie (standalone helper, called from handleViewMovies)
// ─────────────────────────────────────────────
void MenuController::handleRateMovie() {
    // Intentionally left as a no-op standalone entry point.
    // Rating is handled inline inside handleViewMovies().
}

// ─────────────────────────────────────────────
// Admin Menu
// ─────────────────────────────────────────────
void MenuController::showAdminMenu() {
    std::cout << "  --- ADMIN MENU ---\n";
    std::cout << "  1. Add Movie\n";
    std::cout << "  2. Remove Movie\n";
    std::cout << "  3. Back\n";
    std::cout << "Enter your choice: ";

    int choice = 0;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1: {
            Movie m;
            std::cout << "Title: ";
            std::getline(std::cin, m.title);

            std::cout << "Genre: ";
            std::getline(std::cin, m.genre);

            std::cout << "Release Year: ";
            std::cin >> m.releaseYear;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Description: ";
            std::getline(std::cin, m.description);

            std::cout << "Poster Path: ";
            std::getline(std::cin, m.posterPath);

            if (movies.addMovie(m)) {
                std::cout << "Movie \"" << m.title << "\" added successfully.\n";
            } else {
                std::cout << "Failed to add movie.\n";
            }
            break;
        }
        case 2: {
            std::string title;
            std::cout << "Enter title of movie to remove: ";
            std::getline(std::cin, title);

            if (movies.removeMovie(title)) {
                std::cout << "Movie \"" << title << "\" removed successfully.\n";
            } else {
                std::cout << "Failed to remove movie (not found?).\n";
            }
            break;
        }
        case 3:
            // Back — do nothing
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
    }
}

// ─────────────────────────────────────────────
// Route Choice
// ─────────────────────────────────────────────
void MenuController::routeChoice(int choice) {
    switch (choice) {
        case 1:
            if (currentUser.id != -1) {
                // Logout
                std::cout << "Logged out. Goodbye, " << currentUser.mobile << "!\n";
                currentUser = User{};  // reset: id = -1, mobile = ""
            } else {
                showAuthMenu();
            }
            break;
        case 2:
            handleViewMovies();
            break;
        case 3:
            handleSearchMovies();
            break;
        case 4:
            handleAIRecommendations();
            break;
        case 5:
            handleTopRated();
            break;
        case 6:
            handleFavourites();
            break;
        case 7:
            std::cout << "Goodbye!\n";
            db.disconnect();
            exit(0);
        case 8:
            if (currentUser.id != -1 && currentUser.mobile == ADMIN_MOBILE) {
                showAdminMenu();
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
    }
}

// ─────────────────────────────────────────────
// Main Loop
// ─────────────────────────────────────────────
void MenuController::run() {
    while (true) {
        showDashboard();

        int choice = 0;
        if (!(std::cin >> choice)) {
            // Non-integer input: clear error state and discard the bad input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            std::cout << "Invalid input. Please enter a number.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            routeChoice(choice);
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
