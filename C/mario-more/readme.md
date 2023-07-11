# Mario More Problem Set

The "Mario More" problem set involves creating a program that generates pyramid patterns using hashes (#) in text format. The program prompts the user to specify the height of the pyramids and then generates the corresponding pattern.

## Requirements

1. The program should prompt the user for a positive integer between 1 and 8 (inclusive) to determine the height of the pyramids.

2. If the user enters an invalid input (not a positive integer between 1 and 8), the program should re-prompt the user until a valid input is provided.

3. The program should generate the pyramid pattern based on the specified height. Each row of the pyramid should consist of hashes (#) and spaces to create the appropriate pyramid shape.

4. The width of the gap between adjacent pyramids should be equal to the width of two hashes, regardless of the pyramid's height.

## Usage

1. Run the program by executing the command `./mario` in the terminal.

2. The program will prompt the user with the message "Height: ".

3. Enter a positive integer between 1 and 8 (inclusive) to specify the height of the pyramids.

4. The program will generate the pyramid pattern based on the specified height and display it in the terminal.

5. If the user enters an invalid input, the program will re-prompt for a valid input until it is provided.

## Examples

Here are some examples of the program's output for different user inputs:

```
$ ./mario
Height: 4
   #  #
  ##  ##
 ###  ###
####  ####
```

```
$ ./mario
Height: 2
 #  #
##  ##
```

```
$ ./mario
Height: 1
#  #
```

## Implementation

The program's implementation can be found in the `mario.c` file. Open the file to view and modify the code.

Compile the program by executing the command `make mario` in the terminal.

Run the program using the command `./mario`.

Enjoy creating pyramid patterns with hashes and exploring different heights for the pyramids!
