#include <cs50.h>
#include <stdio.h>

bool CalculateChecksum(long input);
int LengthOf(long input);
bool IsAmericanExpress(long input);
bool IsVisa(long input);
bool IsMasterCard(long input);
long GetStartingDigits(long input, int numberOfDigits);

int main(void)
{
    // get creditcard number from user
    long creditCardNumber = get_long("Number: ");
    string creditCardType = "INVALID\n";
    //validation which creditcard type the input is
    if (!CalculateChecksum(creditCardNumber))
    {
        creditCardType = "INVALID\n";
    }
    else if (IsAmericanExpress(creditCardNumber))
    {
        creditCardType = "AMEX\n";
    }
    else if (IsVisa(creditCardNumber))
    {
        creditCardType = "VISA\n";
    }
    else if (IsMasterCard(creditCardNumber))
    {
        creditCardType = "MASTERCARD\n";
    }
    printf("%s", creditCardType);
}

bool CalculateChecksum(long input)
{
    long modulo = 1;
    long divisor = 1;
    int sumOfMultipliedDigits = 0;
    int sumOfOtherDigits = 0;
    // for each digit of the number
    for (int i = 0; i < LengthOf(input); i++)
    {
        modulo = divisor * 10; // multiply by ten to get the next digit
        int digit = (input % modulo) / divisor; // get the digit by dividing the modulo by the divisor
        if (i % 2 != 0)
        {
            // if it is an odd digit, multiply it by two and take the sum of the digits
            digit = digit * 2;
            if (digit / 10 <= 1)
            {
                // take the sum of multiplied digits with two digits as a result
                int digitRest = digit % 10;
                int digitDivided = digit / 10;
                digit = digitRest + digitDivided;
            }
            sumOfMultipliedDigits += digit;
        }
        else // else only add the digit
        {
            sumOfOtherDigits += digit;
        }
        // set the divisor equal to the modulo
        divisor = modulo;
    }
    return (sumOfMultipliedDigits + sumOfOtherDigits) % 10 == 0;
}
// calculate length of long variable
int LengthOf(long input)
{
    int inputLength = 0;
    while (input != 0)
    {
        // divide input by 10 if possible then increment length
        input = input / 10;
        inputLength++;
    }
    return inputLength;
}
// validation for mastercard
bool IsMasterCard(long input)
{
    if (LengthOf(input) != 16)
    {
        return false;
    }

    long startsWith = GetStartingDigits(input, 2);
    // if it starts with following digits, it is valid
    if (startsWith > 50 && startsWith < 56)
    {
        return true;
    }
    return false;
}
// validation for visa
bool IsVisa(long input)
{
    if (LengthOf(input) != 13 && LengthOf(input) != 16)
    {
        return false;
    }

    long startsWith = GetStartingDigits(input, 1);

    // if it starts with following digits, it is valid
    if (startsWith == 4)
    {
        return true;
    }
    return false;
}
// validation for AMEX
bool IsAmericanExpress(long input)
{
    if (LengthOf(input) != 15)
    {
        return false;
    }

    long startsWith = GetStartingDigits(input, 2);

    // if it starts with following digits, it is valid
    if (startsWith == 34 || startsWith == 37)
    {
        return true;
    }
    return false;
}
// Get the first x digits from a custom input
long GetStartingDigits(long input, int numberOfDigits)
{
    long divisor = 1;
    for (int i = 0; i < (LengthOf(input) - numberOfDigits); i++)
    {
        divisor = divisor * 10;
    }
    return input / divisor;
}