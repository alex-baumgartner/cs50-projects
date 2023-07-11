#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int CountLetters(string text);
int CountWords(string text);
int CountSentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    double letters = CountLetters(text);
    double words = CountWords(text);
    double sentences = CountSentences(text);

    double sentenceAverage = sentences / words * 100;
    double letterAverage = letters / words * 100;

    int index = round(0.0588 * letterAverage - 0.296 * sentenceAverage - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int CountLetters(string text)
{
    int count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            count++;
        }
    }
    return count;
}

int CountWords(string text)
{
    int count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (i == (strlen(text) - 2) && isalpha(text[strlen(text) - 2]))
        {
            count++;
        }

        if (i != 0 && text[i] == ' ')
        {
            count++;
        }
    }
    return count;
}

int CountSentences(string text)
{
    int count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }
    return count;
}