# Movie Posters

This folder stores movie poster image files used by the Movie Recommendation System.

## Naming Convention

Name each poster file using the movie title in **lowercase**, with spaces replaced by underscores.

Examples:
- `the_dark_knight.jpg`
- `inception.jpg`
- `the_shawshank_redemption.png`

## Supported Formats

- `.jpg`
- `.png`

## Database Integration

The `poster_path` column in the `movies` database table should store the relative path to the poster file:

```
posters/the_dark_knight.jpg
```

When adding a movie via the admin menu, enter the relative path in this format.

## Missing Posters

If no poster is available for a movie, leave the `poster_path` column empty in the database. The system will automatically display **"No poster available"** in place of the image.
