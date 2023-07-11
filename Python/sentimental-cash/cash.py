from cs50 import get_float

input_valid = False

while not input_valid:
    change = get_float("Change owed: ")
    if change > 0.0:
        input_valid = True

minimum_coins = 0

quarter = 0.25
dime = 0.10
nickel = 0.05
penny = 0.01

if change >= quarter:
    quarter_count = int (change / quarter)
    minimum_coins += quarter_count
    change = change - quarter * quarter_count
    change = round(change, 2)

if change >= dime:
    dime_count = int (change / dime)
    minimum_coins += dime_count
    change = change - dime * dime_count
    change = round(change, 2)

if change >= nickel:
    nickel_count = int (change / nickel)
    minimum_coins += nickel_count
    change = change - nickel * nickel_count
    change = round(change, 2)

if change >= penny:
    penny_count = int (change / penny)
    minimum_coins += penny_count
    change = change - penny * penny_count

print(minimum_coins)
