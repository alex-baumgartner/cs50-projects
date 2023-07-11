#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Gets the name from user input
    string name = get_string("What is your name?\n");
    //Prints greeting with supplied name
    printf("hello, %s\n", name);
}