from cs50 import get_float

#My main function
def main():

    # Ask for the change
    change = get_change()

    #Round the change
    round_change = round(change * 100)

    # Define my coins
    quarter = 25
    dime = 10
    nickel = 5
    penny = 1

    # Coins used
    quarters_used = 0
    dimes_used = 0
    nickels_used = 0
    pennies_used = 0

    #Coins generally
    coins = 0

    # My starting point
    leftover = round_change

    # Check coin usage
    if leftover >= quarter:
        quarters_used = int(leftover / quarter)
        leftover = leftover - (quarters_used * quarter)

    if leftover >= dime:
        dimes_used = int(leftover / dime)
        leftover = leftover - (dimes_used * dime)


    if leftover >= nickel:
        nickels_used = int(leftover / nickel)
        leftover = leftover - (nickels_used * nickel)


    if leftover >= penny:
        pennies_used = int(leftover / penny)
        leftover = leftover - (pennies_used * penny)

    # Add up all used coins
    coins = quarters_used + dimes_used + nickels_used + pennies_used

    #Print
    print(coins)

#Get change function
def get_change():
    change = 0
    while change <= 0:
        change = get_float("Change owed: ")
    return change

#Final main call
main()