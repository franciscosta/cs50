from sys import argv
from sys import exit
import sqlite3

if len(argv) < 2 or len(argv) > 2:
    print("Oops, you didn't to enter the correct no. of arguments.")
    exit(1)

house = argv[1]

# Open the database
students = sqlite3.connect("students.db")

# Connect it to a cursor
c = students.cursor()

c.execute("SELECT first, middle, last, birth FROM students WHERE house = '{}' ORDER BY last;".format(house))

string_result = ''

for row in c.fetchall():

    first, middle, last, birth = row[0], row[1], row[2], row[3]

    if middle == None:
        print(f"{first} {last}, {birth}")

    if middle != None:

        print(f"{first} {middle} {last}, born {birth}")

# Save changes
students.commit()

# Close the file
students.close()