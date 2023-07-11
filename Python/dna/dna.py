import csv
import sys


def main():
    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python tournament.py FILENAME")


    # TODO: Read database file into a variable
    database_file = sys.argv[1]
    sequence_database = []

    with open(database_file) as file:
        reader = csv.DictReader(file)
        for sequence in reader:
            sequence["AGATC"] = int(sequence["AGATC"])
            sequence["AATG"] = int(sequence["AATG"])
            sequence["TATC"] = int(sequence["TATC"])
            sequence_database.append(sequence)

    # TODO: Read DNA sequence file into a variable
    sequence_file = sys.argv[2]
    sequence = ""
    with open(sequence_file) as file:
        sequence = file.read()

    # TODO: Find longest match of each STR in DNA sequence
    sequence_matches = {
        "AGATC": longest_match(sequence, "AGATC"),
        "AATG": longest_match(sequence, "AATG"),
        "TATC": longest_match(sequence, "TATC")
    }
    # TODO: Check database for matching profiles
    match = False
    match_name = ""
    for i in range(0, len(sequence_database)):
        match = check_match(sequence_database[i], sequence_matches)
        if match:
            match_name = sequence_database[i]["name"]
            break

    if match:
        print(match_name)
    else:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run

def check_match(database_sequence, sequence_matches):

    if database_sequence["AGATC"] != sequence_matches["AGATC"]:
        return False

    if database_sequence["AATG"] != sequence_matches["AATG"]:
        return False

    if database_sequence["TATC"] != sequence_matches["TATC"]:
        return False

    return True

main()
