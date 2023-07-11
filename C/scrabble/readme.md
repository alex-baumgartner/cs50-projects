# Lab: Scrabble

In the Scrabble lab, you need to create a program that determines which of two Scrabble words is worth more in terms of points.

## Requirements

1. The program prompts the user to enter two words played by two players.

2. Each letter in the words has a corresponding point value based on the Scrabble rules. The point values are stored in an integer array named `POINTS`, where `POINTS[0]` represents the point value of 'A' or 'a', `POINTS[1]` represents the point value of 'B' or 'b', and so on.

3. The program should calculate the score for each word by summing up the point values of each letter. Uppercase and lowercase letters have the same point values.

4. Characters that are not letters should be given zero points.

5. The program should compare the scores of the two words and determine the winner based on the higher score. If the scores are equal, it should be a tie.

6. The output should be displayed as "Player 1 wins!", "Player 2 wins!", or "Tie!".

## Usage

1. Run the program by executing the command `./scrabble` in the terminal.

2. The program will prompt the user with "Player 1: ".

3. Enter the word played by Player 1 and press Enter.

4. The program will prompt the user with "Player 2: ".

5. Enter the word played by Player 2 and press Enter.

6. The program will determine the winner and display the result.

## Implementation

The implementation of the Scrabble program can be found in the `scrabble.c` file. Open the file to view and modify the code.

Compile the program by executing the command `make scrabble` in the terminal.

Run the program using the command `./scrabble`.

Enjoy playing Scrabble and finding out which word scores higher!
