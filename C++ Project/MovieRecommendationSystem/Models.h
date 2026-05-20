#pragma once

#include <string>

// Represents a registered user identified by a unique mobile number.
struct User {
    int id = -1;            // Primary key from the users table; -1 means not persisted
    std::string mobile;     // Mobile number used for login (10–15 digits)
};

// Represents a movie entry in the catalogue.
struct Movie {
    int id = -1;                // Primary key from the movies table; -1 means not persisted
    std::string title;          // Display title of the movie
    std::string genre;          // Genre label (e.g. "Action", "Drama")
    int releaseYear = 0;        // Four-digit release year; 0 if unknown
    std::string description;    // Short synopsis or plot summary
    std::string posterPath;     // Relative or absolute path to the poster image file
    float avgRating = 0.0f;     // Cached arithmetic mean of all user ratings (1–5 scale)
};

// Represents a single user rating for a movie.
struct Rating {
    int userId = -1;    // Foreign key referencing users.id
    int movieId = -1;   // Foreign key referencing movies.id
    int value = 0;      // Rating value in the closed interval [1, 5]
};

// Represents a movie that a user has added to their favourites list.
struct Favourite {
    int userId = -1;    // Foreign key referencing users.id
    int movieId = -1;   // Foreign key referencing movies.id
};
