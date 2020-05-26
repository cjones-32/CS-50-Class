import csv
from sys import argv
import copy
import re

# verify the user presented the correct number of arguments
if len(argv) != 3:
    print("usage: name $database $sequence")
# open the files and read them
database_file = open(argv[1], "r")
sequence_file = open(argv[2], "r")
database_dict = csv.DictReader(database_file)
# without this you can only itterate over it once
database = list(database_dict)
sequence = sequence_file.readline()

# steals the first ordered dictionary to get a copy to use as a storage for values for values found.
# it already has the keys of the DNA your looking for and a space to put the value of those found
for i in database:
    database_keys = copy.deepcopy(i)
    break

# clear the "max" values
for i in database_keys:
    database_keys[i] = 0

# search the string for 0x, then 1x, then 3x etc of each STR then recorn how many were successfully found
for key in database_keys:
    repetition = 0
    while (key * repetition in sequence):
        repetition += 1
    database_keys[key] = repetition - 1

# see bottom for explination
match = "No match"
for i in database:
    if [k for k in i if i[k] != str(database_keys[k])] == ['name']:
        match = i["name"]
print(match)

# close the files
database_file.close()
sequence_file.close()

"""
above confusing code is the same as below per link
    https://treyhunner.com/2015/12/python-list-comprehensions-now-in-color/
scan over all the people in the database to get their dictionary
for i in database:
    #something to store the differences it finds
    differences = []
    # for all the items for person i, if it doesnt match whats in the database, record the difference as its key value
    for k in i:
        if i[k] != str(database_keys[k]):
            differences.append(k)
    #once you have gone through the persons full sequence, if the oly difference is the name then its a match
    if differences = ["name"]:
        match = i["name"]
"""

