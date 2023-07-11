#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int GetScoreOfLetter(char letter);
int FindIndexOfLetter(char letter);

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
int ARRAY_LENGTH = 26;
char LETTERS[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }

}

int compute_score(string word)
{
    int score = 0;
    int wordLength = strlen(word);
    for (int i = 0; i < wordLength; i++)
    {
        score = score + GetScoreOfLetter(word[i]);
    }
    return score;
}


int GetScoreOfLetter(char letter)
{
    int index;
    index = FindIndexOfLetter(letter);

    if (index == -1 || index > ARRAY_LENGTH)
    {
        return 0;
    }

    return POINTS[index];
}

int FindIndexOfLetter(char letter)
{
    for (int i = 0; i < ARRAY_LENGTH; i++)
    {
        if (toupper(letter) == LETTERS[i])
        {
            return i;
        }
    }
    return -1;
}