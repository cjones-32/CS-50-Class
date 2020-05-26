# import get_int
from cs50 import get_int

# get the height until valid
height = 0
while height > 8 or height < 1:
    height = get_int("Height: ")

# print the lines
for i in range(height):
    print((" " * (height - 1 - i)) + ("#" * (i + 1)) + "  " + ("#" * (i + 1)))
