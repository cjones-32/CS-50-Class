#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

char output[100];

int validate_input_key(string 
                       input);  //return: 0=valid, 1=not 26 characters, 2=nonalphanumaric character, 3=each letter not used once
void set_output_string(string input, string
                       key);  //determines the offset for each character to get correct new value using its decimal ascii table number.

int main(int argc, string argv[])
{
    int offset_values[26];
    string input;
    if (argc == 2) //check if only one argument is added
    {
        if (validate_input_key(argv[1]) == 0) //check if the validation passes
        {
            input = get_string("plaintext:  ");
            set_output_string(input, argv[1]);
            printf("ciphertext: %s\n", output);
        }
        else 
        {
            printf("invalid key");
            return 1;
        }
    }
    else
    {
        printf("invalid arguments");
        return 1;
    }
}

int validate_input_key(string input)
{
    bool letters_used[26];//array to check that each value is a unique letter
    for (int x = 0; x < 26; x++)//initialize array
    {
        letters_used[x] = false;
    }
    if (strlen(input) != 26)//if its not a length of 26, quit right away
    {
        return 1;
    }
    else
    {
        for (int i = 0; i < 26; i++)//work through each letter
        {
            if (isalpha(input[i]))//verifyt its a letter
            {
                if (letters_used[(int) toupper(input[i]) - 65] ==
                    false)//subtracts the now capital letter by 65 (giving A = 0 and Z = 25), goes to that array index and checks that its previous use is galse
                {
                    letters_used[(int) toupper(input[i]) - 65] = true;//set that index to true
                }
                else
                {
                    return 3;//if spot is already true, return 3 signifying multiple of same char used.
                }
            }
            else
            {
                return 2;//if charafter isnt alphabetic, return 2 to signify.
            }
        }
    }
    return 0;
    printf("sucess");
}

void set_output_string(string input, string key)
{
    int offset_index[26];
    for (int i = 0; i < 26; i++) //determine how many characters away the key character is
    {
        offset_index[i] = ((int) toupper(key[i]) - 65) - i;//works
    }
    for (int x = 0, y = strlen(input); x < y; x++) //set the new output based on the offset of the characters
    {
        if (isalpha(input[x]))
        {
            output[x] = (int) input[x] + offset_index[((int) toupper(input[x]) - 65)];
        }
        else
        {
            output[x] = input[x];
        }
    }
}