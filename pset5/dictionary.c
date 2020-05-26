// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>// strcasecmp

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 50000;
unsigned int DICTIONARY_COUNT = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //if that hash value table location is empty, its not here
    if (table[hash(word)] == NULL)
    {
        return false;
    }
    else
    {
        /*set the cursor to the location of the hash value of the input word, work through
        all the words until the end to see if it is present.*/
        for (node *cursor = table[hash(word)]; cursor != NULL; cursor = cursor->next)
        {
            if (strcasecmp(cursor->word, word) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    /*djb2 hash
    http://www.cse.yorku.ca/~oz/hash.html
    modified slightly in while, add tolower and modulo from below and then changed long to int
    https://www.reddit.com/r/C_Programming/comments/2syijd/understanding_this_hash_function/*/
    unsigned int hash_num = 5381;
    int c;
    while ((c = *word++))
    {
        hash_num = ((hash_num << 5) + hash_num) + tolower(c); /* hash * 33 + lower case of c*/
    }
    //returns modulo the total number of buckts to ensure it fits.
    return hash_num % (N - 1);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    char word[LENGTH + 1];
    //open the file
    FILE *dict_word = fopen(dictionary, "r");
    //if file does not exist, end
    if (dict_word == NULL)
    {
        printf("Dictionary not found\n");
        return false;
    }
    //read words one at a time
    while (fscanf(dict_word, "%s", word) != EOF)
    {
        //if word exists, add to word count then create node and verify it was created successfully
        DICTIONARY_COUNT++;
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(dict_word);
            return false;
        }
        //assign the new word to the node and sets next to null
        strcpy(new_node->word, word);
        new_node->next = NULL;
        //run it through the hash and check that that hash table bucket has not been used before
        //if it has, just add the node to that location
        if (table[hash(word)] == NULL)
        {
            table[hash(word)] = new_node;
        }
        //if that node has been used, insert it
        else
        {
            new_node->next = table[hash(word)];
            table[hash(word)] = new_node;
        }
    }
    fclose(dict_word);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return DICTIONARY_COUNT;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //Loop throught he table, if it has a node, delete all the nodes
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    return true;
}
