//Libraries
#include <stdio.h>
#include <cs50.h>

//Delcaring functions
int get_card_length(long);
string get_card_type(long);
bool verify_card_checksum(long);

//Main function 
int main(void)
{
    long card_number = get_long("Number: ");
    int card_length = 0;
    string card_type = "INVALID";
    bool checksum_valid = false;
    card_type = get_card_type(card_number);
    if (verify_card_checksum(card_number) == false) //only verify checksum if provider is valid
    {
        card_type = "INVALID";
    }
    printf("%s\n", card_type);
}


//Get the number of digits in the card
int get_card_length(long card_number)
{   
    int i = 0;
    while (card_number >= 1)
    {
        card_number = card_number / 10;
        i++;
    }
    return i;
}


//Get the provider of the credit card
string get_card_type(long card_number)
{   
    //Get first two digits
    string a = "INVALID";
    int b = get_card_length(card_number);
    int first_two_digits;
    for (int x = get_card_length(card_number) - 2; x > 0; x--)
    {
        card_number = card_number / 10;
    }
    first_two_digits = card_number;
    //Determine card provider to digit lengths
    if (first_two_digits == 34 || first_two_digits == 37)
    {
        if (b == 15)
        {
            a = "AMEX";
        }
    }
    else if (first_two_digits > 50 && first_two_digits < 56)
    {
        if (b == 16)
        {
            a = "MASTERCARD";
        }
    }
    else if (first_two_digits > 39 && first_two_digits < 50)
    {
        if (b == 13 || b == 16)
        {
            a = "VISA";
        }
    }
    else
    {
        a = "INVALID";
    }
    return a;
}


//Verify that the card checksum is valid
bool verify_card_checksum(long card_number)
{
    int check_sum = 0;
    //Get the checksum
    while (card_number >  0)
    {
        //odd digits
        check_sum += card_number % 10;
        card_number /= 10;
        //even digits
        if ((card_number % 10) < 5)
        {
            check_sum += (card_number % 10) * 2;
        }
        else
        {
            check_sum += (((card_number % 10) * 2) - 10) + 1;
        } 
        card_number /= 10;
    }
    //Verify the checksum
    if (check_sum % 10 == 0)
    {
        return true;        
    }
    else
    {
        return false;        
    }
}
