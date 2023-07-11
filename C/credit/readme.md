# Problem Set: Credit

This program prompts the user for a credit card number and determines whether it is a valid American Express, MasterCard, or Visa card number.

## Credit Cards

A credit card is a plastic card used for making payments. Each card has a unique number printed on it, which is also stored in a database. Credit card numbers have a specific structure and include a checksum for error detection.

## Luhn's Algorithm

The program uses Luhn's algorithm to determine if a credit card number is valid. The algorithm follows these steps:

1. Multiply every other digit by 2, starting from the second-to-last digit.
2. Sum the digits of the products obtained in the previous step with the sum of the remaining digits.
3. If the total sum modulo 10 is congruent to 0, the credit card number is valid.

## Implementation Details

The program prompts the user to enter a credit card number and checks its validity. It supports American Express (starting with 34 or 37), MasterCard (starting with 51, 52, 53, 54, or 55), and Visa (starting with 4) card numbers.

Here's an example of how the program works:

```
$ ./credit
Number: 4003600000000014
VISA
```

The program ensures that the user's input is entirely numeric and doesn't contain hyphens or leading zeroes. If the input is not a valid credit card number, the program outputs "INVALID".
