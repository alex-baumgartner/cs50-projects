#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int GetLetterIndex(char letter);
string ConvertToUpper(string stringToConvert);
string Decipher(string textToDecipher, string cipher);
bool CheckCipherValidity(string text);
string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int main(int argc, string argv[])
{
    string cipher = "";
    if (argc != 2)
    {
        printf("Usage: ./substitution key");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.");
        return 1;
    }
    else
    {
        cipher = argv[1];
        cipher = ConvertToUpper(cipher);
        //Check if any chars are duplicate in the cipher
        if (!CheckCipherValidity(cipher))
        {
            return 1;
        }
    }

    string textToDecipher = get_string("plaintext: ");
    string decipheredText = Decipher(textToDecipher, cipher);
    printf("ciphertext: %s\n", decipheredText);
    return 0;
}

string ConvertToUpper(string stringToConvert)
{
    for(int i = 0; i < strlen(stringToConvert); i++)
    {
        if (isalpha(stringToConvert[i]) && islower(stringToConvert[i]))
        {
            stringToConvert[i] = toupper(stringToConvert[i]);
        }
    }
    return stringToConvert;
}

int GetLetterIndex(char letter)
{
    for(int i = 0; i < strlen(ALPHABET); i++)
    {
        char formattedLetter = letter;
        if (!isupper(letter))
        {
            formattedLetter = toupper(letter);
        }

        if (ALPHABET[i] == formattedLetter)
        {
            return i;
        }
    }
    return -1;
}

string Decipher(string textToDecipher, string cipher)
{
    string decipheredText = textToDecipher;
    //decipheredText = (string)malloc(2000);
    //strcpy(textToDecipher, decipheredText);
    for (int i = 0; i < strlen(textToDecipher); i++)
    {
        int letterIndex = GetLetterIndex(textToDecipher[i]);

        if (letterIndex != -1)
        {
            char decipheredChar = cipher[letterIndex];

            if (islower(textToDecipher[i]))
            {
                decipheredText[i] = tolower(decipheredChar);
            }
            else
            {
                decipheredText[i] = decipheredChar;
            }
        }

    }
    return decipheredText;
}

bool CheckCipherValidity(string text)
{
    // iterate over characters in string
    for (int i = 0; i < strlen(text); i++) {
        int count = 0;
        count = 1;
        // check if character is present more than once in string
        for (int j = i+1; j < strlen(text); j++) {
            if (text[i] == text[j]) {
                count++;
                text[j] = '0';
            }
        }
        // if character is present more than once, cipher is not valid
        if (count > 1 && text[i] != '0')
        {
            return false;
        }
        // if string contains non alphabetic characters, cipher is not valid
        if (!isalpha(text[i]))
        {
            return false;
        }
    }
    return true;
}