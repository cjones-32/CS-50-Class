from cs50 import get_string

# index = 0.0588 * average number of letters per 100 words - 0.296 * average number of sentances per 100 words - 15.8
letters = 0
words = 1
sentances = 0

# chars indicating end of a sentance
punctuation = ['.', '!', '?']

imput = get_string("Text: ")
# work through the string and count the letters, words and sentances
for char in imput:
    if char.isalpha():
        letters += 1
    elif char.isspace():
        words += 1
    elif char in punctuation:
        sentances += 1

# calculate the index
index = round(0.0588 * (100 * letters / words) - 0.296 * (100 * sentances / words) - 15.8)

# print the index
if index < 1:
    print("Before Grade 1")
elif index > 15:
    print("Grade 16+")
else:
    print(f"Grade {index}")
