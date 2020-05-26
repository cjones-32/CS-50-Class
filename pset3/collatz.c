//
//from recursion short
//
#include <stdio.h>
#include <cs50.h>

void collatz(int number);

//delcared here to prevent constant resetting by recursion
int count = 0;

//get a number from user and send to function collatz
int main(void)
{
    collatz(get_int("Number: "));
}

//if a number is one, print count. if its even devide by 2 and add to count. if its odd, times by 3 then add one.
void collatz(int number)
{
    if (number == 1)
    {
        printf("%i\n", count);
        return;
    }
    else if (number % 2 == 0)
    {
        count++;
        collatz(number / 2);
    }
    else 
    {
        count++;
        collatz(number * 3 + 1);
    }
}
