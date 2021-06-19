#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "dictionary.h"

#define ALPHA 27 // alphabet size + '

/** node structure **/
typedef struct node
{
    bool is_word; //val
    struct node* children[ALPHA]; // array of pointers to 26 children nodes
}
node;

/** Root node **/
node* root;
long fileSize;
node* nodeBucket;

/**
 * Given a word, checks if it is in the dictionary.
 * @param word is the word to be checked
 */
bool check(const char* word)
{
    /** Create a traverse pointer and points to root **/
    node* trav = NULL;
    trav = root;

    int i = 0;
    /** Iterate over every char in the given word **/
    while(word[i] != '\0')
    {
        /** Make all letters lower case **/
        char c = ( isalpha(word[i])) ? tolower(word[i]) : word[i];
        
        /** Apostrophes **/
        if(c == '\'')
        {
            /** If pointer to the index array is NULL, word is not 
             * in dictionary **/
            if(trav->children[ALPHA-1] == NULL)
                return false;
            /** Traverse to the next node **/
            trav = trav->children[ALPHA-1];
        }
        
        /** Letters **/
        else if(isalpha(c))
        {
            if(trav->children[c - 'a'] == NULL)     
                return false;
            trav = trav->children[c - 'a'];
        }
        i++;
    }
    
    /** Return the value of the node if
     * none of the nodes were NULL **/
    return trav->is_word;
}


/**
 * Loads dictionary into memory.
 * @param dictionary is the dictionary to be loaded to the tree
 * @return true if successful, false if not.
 */
bool load(const char* dictionary)
{
    /** Open the dictionary **/
    FILE* dict = fopen(dictionary, "rb");
    if(dict == false) 
    {
        printf("Dictionary error.");
        return false;
    }
    
    /** Get the size of the dictionary and allocate
     *  memory for the Trie **/
    fseek(dict, 0, SEEK_END);
    fileSize = ftell(dict);
    fseek(dict, 0, SEEK_SET);
    nodeBucket = calloc((fileSize), sizeof(node));
    
    /** Initialize root and declare the traverse node **/
    node* nextFreeNode = nodeBucket;
    root = nextFreeNode + 1;
    node* trav = NULL;
    
    /** Read the file into a buffer and create
     *  a pointer to it **/
    char* buffer = malloc(fileSize + 1);
    fread(buffer, 1, fileSize, dict);
    buffer[fileSize] = '\0';
    char* words = buffer;
    
    /** Loop through the dictionary buffer until the end **/
    while(*words)
    {
        /** Trace back to root when checking a new word **/
        trav = root;
        
        /** Iterate over the characters in the buffer until
         * a newline to indicate the end of a word **/
        for(; *words != '\n' && *words; words++)
        {
            /** Apostrophe **/
            if(*words == '\'')
            {
                /** If the index of a child node is NULL, go to the next free
                 * node. **/
                if(trav->children[ALPHA-1] == NULL)       
                    trav->children[ALPHA-1] = nextFreeNode++;
                trav = trav->children[ALPHA-1];
            }
            
            /** Letters **/
            else
            {
                if(trav->children[*words - 'a'] == NULL)       
                    trav->children[*words - 'a'] = nextFreeNode++;
                trav = trav->children[*words - 'a'];
            }
        }
        
        /** Set the value of an end node when
         *  a word is completed. **/
        trav->is_word = true;
        
        /** Increment the buffer pointer when at
         *  the end of a word **/
        if(*words == '\n')
            words++;
    }
    
    /** Cleaning up **/
    fclose(dict);
    free(buffer);
    return true;
}

/**
 * Unloads dictionary from memory.
 * @return true if sucessful, false if not.
 */
bool unload()
{
    free(nodeBucket);
    return true;
}
