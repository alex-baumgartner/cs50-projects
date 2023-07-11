#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int startPopulation;
    do
    {
        startPopulation = get_int("Start size: ");
    } while (startPopulation < 9);
    // TODO: Prompt for start size

    int endPopulation;
    do
    {
        endPopulation = get_int("End size: ");
    } while (endPopulation < startPopulation);

    // TODO: Prompt for end size


    // TODO: Calculate number of years until we reach threshold
    int years = 0;
    int currentPopulation;
    int populationGain;
    int populationLoss;
    currentPopulation = startPopulation;

    while (currentPopulation < endPopulation)
    {
        populationGain = currentPopulation / 3;
        populationLoss = currentPopulation / 4;
        currentPopulation = currentPopulation + populationGain - populationLoss;
        years += 1;
    }

    printf("Years: %i \n", years);
    // TODO: Print number of years
}
