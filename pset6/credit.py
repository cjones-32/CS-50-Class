import re
from cs50 import get_int


def main():
    # get card number from user
    card_number = get_int("Number: ")
    # get the provider of the card based on numbers
    card_provider = provider(card_number)
    # if card number is valid, check the number
    if card_provider != "INVALID":
        if verify_number(card_number) is False:
            card_provider = "INVALID"
    print(card_provider)

# takes the card number and if it matches the starting numbers and number length returns the provider, otherwise invalid


def provider(card_number):
    if re.search("^34|^37", str(card_number)):
        if len(str(card_number)) == 15:
            return "AMEX"
    elif re.search("^5[1-5]", str(card_number)):
        if len(str(card_number)) == 16:
            return "MASTERCARD"
    elif re.search("^4", str(card_number)):
        if len(str(card_number)) == 13 or len(str(card_number)) == 16:
            return "VISA"
    return "INVALID"


def verify_number(card_number):
    # initialize checksum to 0
    checksum = 0
    # for the digits of the card, alternate between adding the last digit to checksum, and 2x the last digit
    # start at 0, end at length of the card, step by 2 since you check 2 digits at a time
    for i in range(0, len(str(card_number)), 2):
        # //odd digits
        checksum += card_number % 10
        # // makes it do integer division
        card_number //= 10
        # even digits, if its greater then 10, add one and subtract 10 to get the sum of the digits (14 = 1 + 4 = 5)
        if ((card_number % 10) < 5):
            checksum += (card_number % 10) * 2
        else:
            checksum += (((card_number % 10) * 2) - 9)
        card_number //= 10
    # if checksum passes return true, else false
    if checksum % 10 == 0:
        return True
    return False


main()
