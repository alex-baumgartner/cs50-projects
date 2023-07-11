-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Search for the crime at the known date and known location
SELECT description
FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28
AND street = 'Humphrey Street';

-- Search for evidence in the bakery security logs at the mentioned time
SELECT minute, activity, license_plate
FROM bakery_security_logs
WHERE year = 2021
AND month = 7
AND day = 28
AND hour = 10
AND minute >= 15
AND activity = 'exit';

-- Search for people leaving the bakery after shortly after the theft
SELECT name, passport_number
FROM people
WHERE license_plate IN
    (SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute >= 15
    and minute <= 25);

-- Search for flight passengers with matching passport numbers
SELECT flight_id, seat
FROM passengers
WHERE passport_number IN
    (SELECT passport_number
    FROM people
    WHERE license_plate IN
        (SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2021
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute >= 15
        AND minute <= 25));

-- Search for clues from the interviews
SELECT transcipt
FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28;

-- Search for atm transactions that match up with the withdrawal seen by the witness in the morning
SELECT p.name, t.account_number, t.amount, t.transaction_type, t.atm_location
FROM atm_transactions t
JOIN bank_accounts a ON t.account_number = a.account_number
JOIN people p ON p.id = a.person_id
WHERE t.atm_location LIKE '%Legget%'
AND t.year = 2021
AND t.month = 7
AND t.day = 28;

-- Join the results of the search for license plates with the results of the atm transactions
SELECT p.name, t.account_number, t.amount, t.transaction_type, t.atm_location
FROM atm_transactions t
JOIN bank_accounts a ON t.account_number = a.account_number
JOIN people p ON p.id = a.person_id
WHERE t.atm_location LIKE '%Legget%'
AND t.year = 2021
AND t.month = 7
AND t.day = 28
AND p.id IN
    (SELECT id
    FROM people
    WHERE license_plate IN
        (SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2021
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute >= 15
        AND minute <= 25));

-- At this point there are 4 suspects: Bruce, Diana, Iman and Luca now the phone calls will get investigated and cross checked with the suspects
SELECT p.name, p.id
FROM atm_transactions t
JOIN bank_accounts a ON t.account_number = a.account_number
JOIN people p ON p.id = a.person_id
JOIN phone_calls pc ON p.phone_number = pc.caller
WHERE pc.duration < 60
AND pc.year = 2021
AND pc.month = 7
AND pc.day = 28
AND t.atm_location LIKE '%Legget%'
AND t.year = 2021
AND t.month = 7
AND t.day = 28
AND p.id IN
    (SELECT id
    FROM people
    WHERE license_plate IN
        (SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2021
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute >= 15
        AND minute <= 25));

-- To keep the queries readable, from now on person_id's will be used for the two remaining suspects
-- Bruce: 686048, Diana: 514354 (686048, 514354)

-- Bruce, Diana remain as the suspects now searching for a flight ticket bought by the receiver of one of those calls
-- No result here maybe the wrong day?
SELECT name, id
FROM people
WHERE phone_number IN
    (SELECT caller
    FROM phone_calls
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND receiver IN
        (SELECT phone_number
        FROM people
        WHERE passport_number IN
            (SELECT passport_number
            FROM passengers
            WHERE flight_id IN
                (SELECT id
                FROM flights
                WHERE year = 2021
                AND month = 7
                AND day = 29
                AND origin_airport_id =
                    (SELECT id
                    FROM airports
                    WHERE city = 'Fiftyville')))))
AND id IN (686048, 514354);

-- Get the phone call information - every suspect had one phone call that short
SELECT p1.name AS caller, p2.name AS receiver
FROM phone_calls pc
JOIN people p1 ON pc.caller = p1.phone_number
JOIN people p2 ON pc.receiver = p2.phone_number
WHERE p1.id IN (686048, 514354)
AND pc.year = 2021
AND pc.month = 7
AND pc.day = 28
AND pc.duration < 60;

-- Is someone of the suspects on one of the planes? - all of them => need to tie it together with the call somehow
SELECT id, name
FROM people
WHERE passport_number IN
    (SELECT passport_number
    FROM passengers
    WHERE flight_id IN
        (SELECT id
        FROM flights
        WHERE year = 2021
        AND month = 7
        AND day = 29
        AND origin_airport_id =
            (SELECT id
            FROM airports
            WHERE city = 'Fiftyville')))
AND id IN (686048, 514354);

-- Find the earliest plane out of fiftyville -> Diana is eliminated, therefore Bruce is the thief
SELECT f.hour, f.minute, p.id, p.name
FROM people p
JOIN passengers pa ON p.passport_number = pa.passport_number
JOIN flights f ON f.id = pa.flight_id
WHERE p.id IN (686048, 514354)
AND year = 2021
AND month = 7
AND day = 29
AND f.origin_airport_id =
    (SELECT id
    FROM airports
    WHERE city = "Fiftyville");

-- Find the name of the accomplice
SELECT p2.name AS accomplice
FROM phone_calls pc
JOIN people p1 ON pc.caller = p1.phone_number
JOIN people p2 ON pc.receiver = p2.phone_number
WHERE p1.id = 686048
AND pc.year = 2021
AND pc.month = 7
AND pc.day = 28
AND pc.duration < 60;

-- Find the city the thief went to
SELECT a2.city AS destination
FROM people p
JOIN passengers pa ON p.passport_number = pa.passport_number
JOIN flights f ON f.id = pa.flight_id
JOIN airports a1 ON f.origin_airport_id = a1.id
JOIN airports a2 ON f.destination_airport_id = a2.id
WHERE f.year = 2021
AND f.month = 7
AND f.day = 29
AND p.id = 686048
AND a1.city = 'Fiftyville';