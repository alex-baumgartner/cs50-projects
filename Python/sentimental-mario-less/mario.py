
invalid_input = True
while invalid_input:
    user_input = input("Height: ")
    try:
        height = int(user_input)
        if (height > 0 and height < 9):
            invalid_input = False
    except ValueError:
        print("Invalid Input. Please enter an integer.")

for i in range(height):
    row = ""
    for j in range(height - i - 1):
        print(" ", end="")
    for k in range(i + 1):
        print("#", end ="")
    print()


