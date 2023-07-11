# Population Growth Lab

The "Population Growth" lab is a program that calculates the number of years required for a population to reach a particular size. The program simulates the growth of a population of llamas and takes into account the birth and death rates of the llamas.

The user is prompted to enter the starting population size and the desired ending population size. The program then calculates the number of years it will take for the population to reach the end size based on the birth and death rates.

The birth rate is determined by dividing the current population size by 3, while the death rate is determined by dividing the current population size by 4. These rates represent the number of llamas that are born and die each year.

The program handles various scenarios, such as:

- If the user enters a starting population size less than 9, they will be prompted to enter a valid starting size greater than or equal to 9. This ensures that the population doesn't become stagnant.
- If the user enters an ending population size less than the starting population size, they will be prompted to enter a valid ending size greater than or equal to the starting size. This ensures that the population grows over time.

Once the calculations are complete, the program displays the number of years it will take for the population to reach the desired end size.

The program provides a template file called "population.c" that you can use to implement the required logic. Fill in the missing parts of the code and run the program to see the results.

Remember to compile the program before running it by executing the following command in the terminal:

```
make population
```

To run the program, use the following command:

```
./population
```

Provide the starting and ending population sizes when prompted, and the program will display the number of years required for the population to reach the desired size.

Enjoy exploring population growth and understanding how the birth and death rates impact the overall population size over time!
