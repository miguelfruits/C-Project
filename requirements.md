# Requirements Document

## Introduction

This document defines the requirements for a console-based AI-assisted Movie Recommendation System implemented in C++. The system allows users to register and log in, browse and search movies by genre, rate movies, manage a favourites list, and receive personalised movie recommendations based on genre matching and user rating history. Data is persisted using file handling. The system is designed as a BSc IT mini project demonstrating OOP principles, STL usage, and modular code structure.

---

## Glossary

- **System**: The Movie Recommendation System application.
- **User**: A registered person who interacts with the System.
- **Guest**: An unregistered person attempting to use the System.
- **Movie**: A record containing a title, genre(s), release year, and description stored in the System.
- **Rating**: A numeric score (1–5) assigned by a User to a Movie.
- **Favourite**: A Movie explicitly bookmarked by a User for quick access.
- **Recommendation**: A ranked list of Movies suggested to a User based on genre preferences and rating history.
- **Genre**: A category label (e.g., Action, Comedy, Drama, Sci-Fi, Horror, Romance, Thriller) used to classify Movies.
- **UserProfile**: The stored record of a User's credentials, ratings, and favourites.
- **DataStore**: The file-based persistence layer that stores User and Movie data.
- **AuthManager**: The module responsible for user registration and login.
- **MovieManager**: The module responsible for storing, searching, and browsing Movies.
- **RatingManager**: The module responsible for recording and retrieving Ratings.
- **FavouritesManager**: The module responsible for managing a User's Favourite list.
- **RecommendationEngine**: The module that generates personalised Movie Recommendations.
- **MenuController**: The module that drives the console interface and routes user input.

---

## Requirements

### Requirement 1: User Registration

**User Story:** As a Guest, I want to register an account, so that I can access personalised features of the System.

#### Acceptance Criteria

1. WHEN a Guest provides a unique username and a non-empty password, THE AuthManager SHALL create a new UserProfile and persist it to the DataStore.
2. WHEN a Guest provides a username that already exists in the DataStore, THE AuthManager SHALL reject the registration and display an error message indicating the username is taken.
3. WHEN a Guest provides an empty username or an empty password, THE AuthManager SHALL reject the registration and display an error message indicating the fields are required.
4. THE AuthManager SHALL store passwords using a basic hash (e.g., std::hash) so that plain-text passwords are not written to the DataStore.

---

### Requirement 2: User Login and Logout

**User Story:** As a registered User, I want to log in and log out, so that I can access my personal data securely.

#### Acceptance Criteria

1. WHEN a User provides a correct username and matching password, THE AuthManager SHALL authenticate the User and establish an active session.
2. WHEN a User provides an incorrect username or a non-matching password, THE AuthManager SHALL deny access and display an error message.
3. WHEN an authenticated User requests logout, THE System SHALL terminate the active session and return to the main menu.
4. WHILE a User session is active, THE System SHALL display the logged-in username in the console menu header.

---

### Requirement 3: Movie Browsing

**User Story:** As a User, I want to browse all available movies, so that I can discover titles to watch or rate.

#### Acceptance Criteria

1. WHEN an authenticated User requests the movie list, THE MovieManager SHALL display all Movies stored in the DataStore, showing title, genre(s), release year, and average rating.
2. WHEN the DataStore contains no Movies, THE MovieManager SHALL display a message indicating no movies are available alongside any movie display elements.
3. THE MovieManager SHALL display Movies sorted alphabetically by title by default.

---

### Requirement 4: Movie Search by Genre

**User Story:** As a User, I want to search for movies by genre, so that I can quickly find titles matching my interests.

#### Acceptance Criteria

1. WHEN an authenticated User enters a genre keyword, THE MovieManager SHALL return all Movies whose genre list contains a case-insensitive match for that keyword.
2. WHEN no Movies match the entered genre keyword, THE MovieManager SHALL display a message indicating no results were found.
3. WHEN search results are returned, THE MovieManager SHALL display each matching Movie's title, genre(s), release year, and average rating.

---

### Requirement 5: Movie Rating

**User Story:** As a User, I want to rate movies I have watched, so that I can record my opinion and improve my recommendations.

#### Acceptance Criteria

1. WHEN an authenticated User selects a Movie and submits a rating value between 1 and 5 inclusive, THE RatingManager SHALL record the rating and associate it with the User's UserProfile.
2. WHEN an authenticated User rates a Movie they have already rated, THE RatingManager SHALL update the existing rating with the new value.
3. WHEN an authenticated User submits a rating value outside the range 1–5, THE RatingManager SHALL reject the input and display an error message specifying the valid range.
4. WHEN a Movie receives one or more ratings, THE RatingManager SHALL compute and store the average rating for that Movie.
5. THE RatingManager SHALL persist all ratings to the DataStore immediately after recording.

---

### Requirement 6: Favourites Management

**User Story:** As a User, I want to add and remove movies from my favourites list, so that I can quickly access movies I enjoy.

#### Acceptance Criteria

1. WHEN an authenticated User adds a Movie to their favourites, THE FavouritesManager SHALL add the Movie to the User's Favourite list and persist the change to the DataStore.
2. WHEN an authenticated User attempts to add a Movie that is already in their Favourite list, THE FavouritesManager SHALL display a message indicating the Movie is already a favourite.
3. WHEN an authenticated User removes a Movie from their favourites and the Movie is present in the Favourite list, THE FavouritesManager SHALL remove the Movie from the Favourite list and persist the change to the DataStore.
4. WHEN an authenticated User attempts to remove a Movie that is not in their Favourite list, THE FavouritesManager SHALL display a message indicating the Movie is not in the list.
5. WHEN an authenticated User requests their favourites list, THE FavouritesManager SHALL display all Movies in the Favourite list with title, genre(s), and average rating.
6. WHEN the Favourite list is empty, THE FavouritesManager SHALL display a message indicating no favourites have been added.

---

### Requirement 7: Movie Recommendations

**User Story:** As a User, I want to receive personalised movie recommendations, so that I can discover new movies aligned with my tastes.

#### Acceptance Criteria

1. WHEN an authenticated User requests recommendations, THE RecommendationEngine SHALL generate a ranked list of up to 10 Movies not yet rated by the User.
2. THE RecommendationEngine SHALL score each candidate Movie by counting how many of its genres match the genres present in Movies the User has rated 4 or 5.
3. WHEN two candidate Movies have equal genre-match scores, THE RecommendationEngine SHALL rank the Movie with the higher average rating first.
4. WHEN a User has no rated Movies, THE RecommendationEngine SHALL return the top 10 Movies by average rating as a default recommendation list.
5. WHEN fewer than 10 unrated Movies exist, THE RecommendationEngine SHALL return all available unrated Movies.
6. WHEN recommendations are displayed and a Movie title is unavailable due to a technical issue, THE System SHALL show the Movie's genre(s), average rating, and genre-match score without the title.

---

### Requirement 8: Data Persistence

**User Story:** As a system operator, I want all data to be saved to files, so that user and movie data survives application restarts.

#### Acceptance Criteria

1. THE DataStore SHALL persist all UserProfile records (username, hashed password, ratings, favourites) to a dedicated text file.
2. THE DataStore SHALL persist all Movie records (title, genre(s), release year, description, ratings) to a dedicated text file.
3. WHEN the System starts, THE DataStore SHALL load all UserProfile and Movie records from their respective files into memory.
4. IF a data file is missing at startup, THE DataStore SHALL create an empty file and continue without error.
5. IF a data file is corrupted or contains malformed records, THE DataStore SHALL skip the malformed record, log a warning to the console, and continue loading remaining records. IF the corruption recovery process itself fails, THE System SHALL terminate startup with an error message.

---

### Requirement 9: Console Menu Interface

**User Story:** As a User, I want a clear menu-driven interface, so that I can navigate the System without prior training.

#### Acceptance Criteria

1. THE MenuController SHALL display a main menu with options for Login, Register, and Exit when no User session is active.
2. WHILE a User session is active, THE MenuController SHALL display an authenticated menu with options for Browse Movies, Search by Genre, Rate a Movie, Manage Favourites, Get Recommendations, and Logout.
3. WHEN a User enters an invalid menu option, THE MenuController SHALL display an error message and re-display the current menu.
4. WHEN a User selects Exit from the main menu, THE System SHALL attempt to save all pending data to the DataStore and terminate. IF the save operation fails, THE System SHALL still terminate, prioritising user control over data integrity.
5. THE MenuController SHALL clear the console screen between menu transitions to maintain a clean display. THE MenuController SHALL NOT clear the screen during application startup.

---

### Requirement 10: Admin Movie Management

**User Story:** As an administrator, I want to add and remove movies from the system, so that the movie catalogue can be maintained.

#### Acceptance Criteria

1. WHEN a User with the username "admin" logs in, THE MenuController SHALL display an additional Admin Menu option alongside the standard authenticated menu.
2. WHEN the admin selects Add Movie, THE MovieManager SHALL prompt for title, genre(s), release year, and description, then persist the new Movie to the DataStore.
3. WHEN the admin provides an empty title or an empty genre list when adding a Movie, THE MovieManager SHALL reject the input, display an error message, and prevent the Movie from being saved to the DataStore.
4. WHEN the admin selects Remove Movie and provides a valid Movie title, THE MovieManager SHALL remove the Movie from the DataStore and delete all associated ratings and favourites references.
5. WHEN the admin provides a Movie title that does not exist, THE MovieManager SHALL display a message indicating the Movie was not found.
