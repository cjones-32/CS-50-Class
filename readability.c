//importing libraries
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//function to get individual scores from input text
void get_readibility_scores(string input_text, float score_results[]);

//main function
int main(void)
{
    string input_text;
    float score_results[5] = {0, 0, 0, 0, 0}; //index[0] total number of letters, index [1] total number of words, [2] total number of sentances, [3] letters per 100 words, [4] sentances per 100 words
    float index;
    
    input_text = get_string("Text: ");
    get_readibility_scores(input_text, score_results);
    //printf("%.0f letter(s)\n%.0f word(s)\n%.0f sentence(s)\n", score_results[0], score_results[1], score_results[2]);
    index = round(0.0588 * score_results[3] - 0.296 * score_results[4] - 15.8);
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index < 16)
    {
        printf("Grade %.0f\n", index);
    }
    else 
    {
        printf("Grade 16+\n");
    }
}


//get individual scores from input text
void get_readibility_scores(string input_text, float score_results[])
{
    for (int i = 0, n = strlen(input_text); i < n; i++)
    {
        if (isalpha(input_text[i])) //if its text, add to text index
        {
            score_results[0]++;
        }
        else if (input_text[i] == ' ') //else if its a space, add to word counter
        {
            if (score_results[1] == 0) //first space counts the next and first words
            {
                score_results[1]++;
            }
            score_results[1]++; 
        }
        else if (input_text[i] == '.' || input_text[i] == '!' || input_text[i] == '?') //else if its punctuation then add to sentance count
        {
            score_results[2]++;
        }
        else //else do nothing and go to next character
        {
            
        }
    }
    score_results[3] = 100 * score_results[0] / score_results[1]; //get letters per 100 words
    score_results[4] = 100 * score_results[2] / score_results[1]; //get sentances per 100 words
}