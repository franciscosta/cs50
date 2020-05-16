from sys import argv
from sys import exit
import csv


def main():

    # Check if user entered the right ammount of arguments
    if len(argv) < 3 or len(argv) > 3:
        print('Usage: python dna.py data.csv sequence.txt')
        exit(1)

    # Assign the file paths to variables
    db, sq = argv[1], argv[2]

    # Extract the STRs from the database
    ListOfStr = GetStrs(db)  # This is a lists

    # Create a list with the results
    ListOfResults = []  # more effective, it keeps the order

    # Open the sequence
    sequence = open(sq, 'r')
    StringSequence = sequence.read()

    # My two temporary variables
    strcopy = "0000"
    counter = 0

    for i in range(len(ListOfStr)):

        # Copies current STR onto a temp variable
        strcopy = ListOfStr[i]

        # Is that STR present?
        o = FindOccurrences(StringSequence, ListOfStr[i])

        # If so
        if len(o) > 0:

            # Now, based on the number of times its present
            for a in range(len(o)):

                # Is it present
                if strcopy in StringSequence:
                    counter += 1  # Increase counter
                    strcopy = strcopy + ListOfStr[i]  # Double the STR

        # Update the results
        ListOfResults.append(counter)

        # Reset the counter
        counter = 0

    # Close the file
    sequence.close()

    # Compare results to database
    with open(db, 'r') as database:
        readDb = csv.reader(database)

        # Skip the first line
        next(readDb)

        # Row by row
        for row in readDb:

            rowCopy = row
            name = rowCopy[0]
            rowCopy.remove(rowCopy[0])

            for c in range(len(rowCopy)):
                rowCopy[c] = int(rowCopy[c])

            if ListOfResults == rowCopy:
                print(name)
                exit(2)

        print("No match")


# This function extracts the STRs from the database
def GetStrs(database):
    with open(database, 'r') as db:
        for row in csv.reader(db):
            strs = row
            break
        strs.remove(strs[0])
    return strs


# Finds occurences
def FindOccurrences(sequence, str):
    counter = sequence.count(str)  # Counts how manu times word occurs in sequence
    locations = []
    k = 0
    for i in range(counter):
        k = sequence.find(str, k)
        locations.append(k)
        k += 1
    return locations


main()