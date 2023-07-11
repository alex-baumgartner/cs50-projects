#include <cs50.h>
#include <stdio.h>
void PrintFirstPartOfLine(int i);
void PrintSpaces(int i);
int main(void)
{
    int height;
    do
    {
        // Get height from user until between 1 and 8
        height = get_int("Height: ");
    }
    while (0 >= height || height > 8);

    int amountSpaces = 0;
    for (int i = 0; i <= height; i++)
    {
        // skip first line which would be empty
        if (i == 0)
        {
            continue;
        }
        // calculate amount of spaces to put before hashes and print them
        amountSpaces = height - i;
        PrintSpaces(amountSpaces);
        PrintFirstPartOfLine(i);
        printf("  ");
        PrintFirstPartOfLine(i);
        printf("\n");
    }
}

void PrintSpaces(int amountSpaces)
{
    //print specific amount of spaces depending on input
    for (int i = 0; i < amountSpaces; i++)
    {
        printf(" ");
    }
}

void PrintFirstPartOfLine(int i)
{
    //print specific amount of hashes depending on input
    for (int k = 0; k < i; k++)
    {
        printf("#");
    }
}

