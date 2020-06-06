import csv
import cs50
from sys import argv

# verify the user presented the correct number of arguments
if len(argv) != 2:
    print("ERROR: Expecting one .csv argument")
    exit()

# open the file, if it fails exit
try:
    roster_file = open(argv[1], "r")
except:
    print("ERROR: Invalid CSV file name")
    exit()

# Create DictReader
student_dict = csv.DictReader(roster_file)

# Create database
open("students.db", "w").close()
db = cs50.SQL("sqlite:///students.db")

# Create a table for each student their houses and have the ID autoincrement
db.execute("CREATE TABLE students (id INTEGER PRIMARY KEY, first TEXT, middle TEXT, last TEXT, birth DATE, house TEXT)")

# Iterate over students in CSV file
for row in student_dict:

    # Not happy with how I handle if they have middle name or not. Too much code
    # If length of names is 2, just do first and last name
    if len(row["name"].split(" ")) == 2:
        for counter, i in enumerate(row["name"].split(" "), 1):
            if counter == 1:
                first_name = i
            else:
                last_name = i
        # populate in the person data
        db.execute("INSERT INTO students (first, last, birth, house) VALUES (?, ?, ?, ?)",
                   first_name, last_name, row["birth"], row["house"])

    # else if the name has a length of 3, its first middle and last
    elif len(row["name"].split(" ")) == 3:
        for counter, i in enumerate(row["name"].split(" "), 1):
            if counter == 1:
                first_name = i
            elif counter == 2:
                middle_name = i
            else:
                last_name = i
        # populate the person data
        db.execute("INSERT INTO students (first, middle, last, birth, house) VALUES (?, ?, ?, ?, ?)",
                   first_name, middle_name, last_name, row["birth"], row["house"])

# close the roster file
roster_file.close()

