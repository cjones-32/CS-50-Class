#include <cs50.h>
#include <stdio.h>

void print_double_pyramids(int n);

int main(void)
{
    int number = 0;
    //gets a number that is between 1 and 8
    do
    {
        number = get_int("Height: ");
    }
    while (number < 1 || number > 8);
    print_double_pyramids(number);
}


void print_double_pyramids(int n)
{
    //goes from one column to the next
    for (int x = 0; x < n; x++)
    {
        //print the spaces
        for (int spaces = 0; spaces < n - x - 1; spaces++)
        {
            printf(" ");
        }
        //print the first hashes
        for (int hash = 0; hash < x + 1; hash++)
        {
            printf("#");
        }
        //print the 2 middle spaces
        printf("  ");
        //print the second hashes
        for (int hash = 0; hash < x + 1; hash++)
        {
            printf("#");
        }
        printf("\n");
    }
}
