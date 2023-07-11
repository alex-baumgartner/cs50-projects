SELECT m.title
FROM movies m
JOIN ratings r ON m.id = r.movie_id
JOIN stars s ON m.id = s.movie_id
JOIN people p ON s.person_id = p.id
WHERE p.name = 'Chadwick Boseman'
ORDER BY rating DESC
LIMIT 5;