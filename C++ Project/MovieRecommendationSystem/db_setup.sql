-- =============================================================================
-- Movie Recommendation System — Database Setup
-- =============================================================================
-- Run this script once to initialise the schema and seed sample data.
-- Usage: mysql -u <user> -p < db_setup.sql
-- =============================================================================

CREATE DATABASE IF NOT EXISTS movie_rec_db;
USE movie_rec_db;

-- -----------------------------------------------------------------------------
-- Table: users
-- Stores registered users identified solely by their mobile number.
-- A new row is inserted automatically on first login (no explicit registration).
-- -----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS users (
  id     INT PRIMARY KEY AUTO_INCREMENT,
  mobile VARCHAR(15) UNIQUE NOT NULL
);

-- -----------------------------------------------------------------------------
-- Table: movies
-- Master catalogue of all movies available in the system.
-- avg_rating is maintained by RatingManager::updateAvgRating() and is
-- recomputed as the arithmetic mean of all rows in the ratings table for
-- that movie whenever a new rating is submitted or updated.
-- -----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS movies (
  id           INT PRIMARY KEY AUTO_INCREMENT,
  title        VARCHAR(200) NOT NULL,
  genre        VARCHAR(100) NOT NULL,
  release_year INT,
  description  TEXT,
  poster_path  VARCHAR(300),
  avg_rating   FLOAT DEFAULT 0.0
);

-- -----------------------------------------------------------------------------
-- Table: ratings
-- Records each user's rating (1–5) for a movie.
-- The UNIQUE constraint on (user_id, movie_id) ensures a user can rate a
-- given movie only once; updates are handled via ON DUPLICATE KEY UPDATE.
-- -----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS ratings (
  id       INT PRIMARY KEY AUTO_INCREMENT,
  user_id  INT,
  movie_id INT,
  rating   INT CHECK(rating BETWEEN 1 AND 5),
  FOREIGN KEY (user_id)  REFERENCES users(id),
  FOREIGN KEY (movie_id) REFERENCES movies(id),
  UNIQUE(user_id, movie_id)
);

-- -----------------------------------------------------------------------------
-- Table: favorites
-- Tracks which movies a user has added to their favourites list.
-- The UNIQUE constraint prevents duplicate entries for the same
-- (user_id, movie_id) pair.
-- -----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS favorites (
  id       INT PRIMARY KEY AUTO_INCREMENT,
  user_id  INT,
  movie_id INT,
  FOREIGN KEY (user_id)  REFERENCES users(id),
  FOREIGN KEY (movie_id) REFERENCES movies(id),
  UNIQUE(user_id, movie_id)
);

-- -----------------------------------------------------------------------------
-- Table: watch_history
-- Logs every time a user views a movie's detail page.
-- Multiple rows for the same (user_id, movie_id) are allowed so the full
-- viewing timeline is preserved. viewed_at defaults to the current timestamp.
-- -----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS watch_history (
  id        INT PRIMARY KEY AUTO_INCREMENT,
  user_id   INT,
  movie_id  INT,
  viewed_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (user_id)  REFERENCES users(id),
  FOREIGN KEY (movie_id) REFERENCES movies(id)
);

-- =============================================================================
-- Sample Data — 15 movies across 9 genres
-- These seed rows give the recommendation engine enough variety to produce
-- meaningful results immediately after deployment.
-- =============================================================================

INSERT INTO movies (title, genre, release_year, description, poster_path, avg_rating) VALUES

-- Action
('Mad Max: Fury Road',
 'Action', 2015,
 'In a post-apocalyptic wasteland, Max teams up with Furiosa to flee a warlord and his army in a high-octane chase across the desert.',
 'posters/mad_max_fury_road.jpg', 4.7),

('John Wick',
 'Action', 2014,
 'A retired hitman seeks vengeance against the gangsters who killed his dog and stole his car, unleashing his lethal skills once more.',
 'posters/john_wick.jpg', 4.6),

-- Sci-Fi
('Interstellar',
 'Sci-Fi', 2014,
 'A team of astronauts travels through a wormhole near Saturn in search of a new home for humanity as Earth faces extinction.',
 'posters/interstellar.jpg', 4.8),

('The Matrix',
 'Sci-Fi', 1999,
 'A computer hacker discovers that reality is a simulation and joins a rebellion against the machines that have enslaved humanity.',
 'posters/the_matrix.jpg', 4.9),

-- Romance
('The Notebook',
 'Romance', 2004,
 'A poor young man and a rich young woman fall in love during the 1940s, but their relationship is threatened by her parents and the onset of war.',
 'posters/the_notebook.jpg', 4.3),

-- Horror
('Get Out',
 'Horror', 2017,
 'A young African-American man visits his white girlfriend''s family estate and uncovers a disturbing secret hidden beneath the surface.',
 'posters/get_out.jpg', 4.5),

('A Quiet Place',
 'Horror', 2018,
 'A family struggles to survive in a post-apocalyptic world inhabited by blind monsters with an acute sense of hearing.',
 'posters/a_quiet_place.jpg', 4.4),

-- Comedy
('The Grand Budapest Hotel',
 'Comedy', 2014,
 'A legendary concierge and his protégé become embroiled in the theft of a priceless painting and a family inheritance battle.',
 'posters/grand_budapest_hotel.jpg', 4.5),

-- Drama
('The Shawshank Redemption',
 'Drama', 1994,
 'Two imprisoned men bond over years, finding solace and eventual redemption through acts of common decency.',
 'posters/shawshank_redemption.jpg', 5.0),

-- Thriller
('Gone Girl',
 'Thriller', 2014,
 'On the morning of his fifth wedding anniversary, Nick Dunne reports that his wife Amy has gone missing, triggering a media frenzy.',
 'posters/gone_girl.jpg', 4.4),

('Parasite',
 'Thriller', 2019,
 'A poor family schemes to become employed by a wealthy family, leading to an unexpected and darkly comic confrontation.',
 'posters/parasite.jpg', 4.8),

-- Marvel / Superhero
('Avengers: Endgame',
 'Marvel', 2019,
 'The Avengers assemble once more to reverse the devastating actions of Thanos and restore balance to the universe.',
 'posters/avengers_endgame.jpg', 4.7),

('Black Panther',
 'Marvel', 2018,
 'T''Challa returns home to Wakanda to take his rightful place as king, but faces a challenger who threatens to plunge the nation into war.',
 'posters/black_panther.jpg', 4.6),

-- Animation
('Spider-Man: Into the Spider-Verse',
 'Animation', 2018,
 'Teen Miles Morales becomes Spider-Man and must team up with alternate-universe versions of the hero to save New York.',
 'posters/spiderverse.jpg', 4.9),

-- Drama (second entry for variety)
('Schindler''s List',
 'Drama', 1993,
 'German industrialist Oskar Schindler saves the lives of more than a thousand Jewish refugees during the Holocaust.',
 'posters/schindlers_list.jpg', 4.9);
