#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int get_key(string key_string);
void print_message(int key);

int main(int argc, string argv[])
{
    int key = 0;
    if (argc != 2) // if there is not one additional argument, return error
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }
    key = get_key(argv[1]); // get the key from the sting argument and validate it
    if (key >= 0) //  if key is valid, continue
    {
        print_message(key);
    }
    else 
    {
        printf("Usage: %s key\n", argv[0]); // invalid key
        return 1;
    }
}

int get_key(string key_string)
{
    int key = 0;
    for (int i = 0, j = strlen(key_string); i < j; i++)
    {
        if (isdigit(key_string[i]))
        {
            //takes the current key, x10 and add the next character
            key = (key * 10) + (int) key_string[i] - 48;
        }
        else
        {
            return -1;
        }
    }
    key = key % 26;
    return key;
}

void print_message(int key)
{
    string message = get_string("plaintext:  ");
    printf("ciphertext: ");
    for (int i = 0, j = strlen(message); i < j; i++)
    {
        //if the message and translated message are letters of the same caps, add the key
        if ((isupper(message[i]) && isupper(message[i] + key)) || (islower(message[i]) && islower(message[i] + key)))
        {
            printf("%c", message[i] + key);
        }
        //if the key takes a letter past z, bring it back around
        else if (isalpha(message[i]))
        {
            printf("%c", message[i] + key - 26);
        }
        //print non alpha as itself
        else
        {
            printf("%c", message[i]);
        }
    }
    printf("\n");
}
