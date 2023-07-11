// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

bool word_exists(const char *word, node *parentNode);
void free_node(node *parentNode);

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];
bool dictionaryIsLoaded = false;
int numberOfWords = 0;
// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int index = hash(word);
    node *parentNode = malloc(sizeof(node));
    if (word_exists(word, table[index])) {
        return true;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

bool word_exists(const char *word, node *parentNode) {
    bool wordExists = false;
    node *cursor = parentNode;
    while (!wordExists && cursor != NULL){
        if (strcasecmp(word, cursor->word) == 0) {
            wordExists = true;
        }

        cursor = cursor->next;
    }

    return wordExists;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Todo: check wrong memory allocation or smth
    FILE *dictionaryPtr = fopen(dictionary, "r");
    if (dictionaryPtr == NULL) {
        printf("Could not open dictionary file!");
        return false;
    }

    char word[LENGTH + 1];
    while (fscanf(dictionaryPtr, "%s", word) != EOF) {
        node *n = malloc(sizeof(node));

        if (n == NULL) {
            printf("Could not allocate memory for node to store word %s", word);
            return false;
        }

        strcpy(n->word, word);
        unsigned int index = hash(word);
        // set the pointer for the new node to the address of the first node of the linked list on the index of the hash table returned by the hash function
        if (table[index] == NULL){
            table[index] = malloc(sizeof(node));
        }

        n->next = NULL;

        // allocate memory for the next element of the linked list
        //table[index]->next = malloc(sizeof(node));
        n->next = malloc(sizeof(node));
        // now set the linked list to point at the node item, so it points to the beginning again
        // If the head of the linked list is NULL, set it to the new node
        if (table[index] == NULL){
            table[index] = n;
        } else {
            // Otherwise, add the new node to the head of the linked list
            n->next = table[index];
            table[index] = n;
        }

        numberOfWords++;
    }
    dictionaryIsLoaded = true;
    return dictionaryIsLoaded;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (dictionaryIsLoaded) {
        return numberOfWords;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    bool unloadingSuccessful = true;
    for (int i = 0; i < LENGTH; i++) {

        if (table[i] != NULL) {
            free_node(table[i]);
        }
    }
    // TODO: for each element in the hash table call the free_node function recursively until the next node is null
    return unloadingSuccessful;
}

void free_node(node *parentNode)
{
    if (parentNode->next != NULL) {
        free_node(parentNode->next);
    }
    free(parentNode);
}