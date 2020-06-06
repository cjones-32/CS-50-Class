import cs50
from sys import argv

# Verify the user presented the correct number of arguments
if len(argv) != 2:
    print("ERROR: Expecting one house name as argument")
    exit()

# Open the database, if it fails exit
try:
    db = cs50.SQL("sqlite:///students.db")
except:
    print("ERROR: Database not found")
    exit()

# Query for selecting the right students and data
results = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", argv[1])

# Print the names and birth depending if they have a middle name or not
for i in results:
    if i["middle"] is None:
        print(i["first"], i["last"] + ", birth " + str(i["birth"]))
    else:
        print(i["first"], i["middle"], i["last"] + ", birth " + str(i["birth"]))