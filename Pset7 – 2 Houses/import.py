from sys import argv
from sys import exit
from collections import Counter
import csv
import sqlite3

if len(argv) < 2 or len(argv) > 2:
    print("Oops, you didn't to enter the correct no. of arguments.")
    exit(1)

with open(argv[1], 'r') as characters:

    # Read the csv
    reader = csv.reader(characters)

    # Assign all my variables
    name = "null"
    first = "null"
    middle = "null"
    last = "null"
    house = "null"
    birth = "null"
    counter = 1

    # Skip the first line
    next(reader)

    # Iterate through the CSV
    for row in reader:

        # Split the name into a list
        name = row[0].split()

        # If there's only two names
        if len(name) == 2:
            first = name[0]
            middle = "NULL"
            last = name[1]

        # If there are three names
        if len(name) == 3:
            first = name[0]
            middle = name[1]
            last = name[2]

        # The rest of the variables
        house = row[1]
        birth = row[2]

        # Open the database
        students = sqlite3.connect("students.db")

        # Connect it to a cursor
        c = students.cursor()

        # If no middle name
        if middle == "NULL":
            c.execute("INSERT INTO students VALUES ({}, '{}', {}, '{}', '{}', '{}')".format(
                counter, first, middle, last, house, birth))

        # If there is a middle name
        if middle != "NULL":
            c.execute("INSERT INTO students VALUES ({}, '{}', '{}', '{}', '{}', '{}')".format(
                counter, first, middle, last, house, birth))

        # Save changes
        students.commit()

        # Close the file
        students.close()

        # Increase unique ID
        counter += 1

