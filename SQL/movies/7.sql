SELECT m.title, r.rating
FROM movies m
JOIN ratings r ON m.id = r.movie_id
WHERE year = 2010
ORDER BY rating DESC, title;