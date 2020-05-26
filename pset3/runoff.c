#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

void print_preferences(void);
void print_candidates(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    { 
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }
        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate(); 

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);
        
        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    //scan through each voter, if their name matches that index, add that index to the 2D array at that voting spot.
    for (int i = 0, j = candidate_count; i < j; i++)
    {
        if (strcmp(candidates[i].name, name) == 0) 
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    //scan through each voter, if their first prefrence voter is eliminated go to next and add a vote
    for (int voter = 0; voter < voter_count; voter++)
    {
        int preference = 0;
        while (candidates[preferences[voter][preference]].eliminated == true)
        {
            preference++;
        }
        candidates[preferences[voter][preference]].votes++;
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    for (int candidate = 0; candidate < candidate_count; candidate++)
    {
        if (candidates[candidate].votes > (voter_count / 2))
        {
            printf("%s\n", candidates[candidate].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min_votes = 32767; // max size of an int
    for (int candidate = 0; candidate < candidate_count; candidate++)
    {
        if (candidates[candidate].eliminated == false)
        {
            if (candidates[candidate].votes < min_votes)
            {
                min_votes = candidates[candidate].votes;
            }
        }
        
    }
    return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    for (int candidate = 0; candidate < candidate_count; candidate++)
    {
        if (candidates[candidate].eliminated == false)
        {
            if (candidates[candidate].votes > min)
            {
                return false;
            }
        }
        
    }
    return true;
}

// Eliminate the candidate (or candidiates) in last place -- diagnostic use only
void eliminate(int min)
{
    for (int candidate = 0; candidate < candidate_count; candidate++)
    {
        if (candidates[candidate].eliminated == false)
        {
            if (candidates[candidate].votes == min)
            {
                candidates[candidate].eliminated = true;
            }
        }
        
    }
    return;
}

//prints out the prefrences 2D array to verify its population is correct -- diagnostic use only
void print_preferences(void)
{
    for (int i = 0, j = voter_count; i < j; i++)
    {
        for (int a = 0, b = candidate_count; a < b; a++)
        {
            printf("%i", preferences[i][a]);
        }
        printf("\n");
    }
}

//prints out the candidates name, their votes, and if they've been eliminated
void print_candidates(void)
{
    printf("\nname   votes    elim\n");
    for (int i = 0, j = candidate_count; i < j; i++)
    {
        printf("%s      %i        ", candidates[i].name, candidates[i].votes);
        if (candidates[i].eliminated ==  true)
        {
            printf("T\n");
        }
        else
        {
            printf("F\n");
        }
    }
}
