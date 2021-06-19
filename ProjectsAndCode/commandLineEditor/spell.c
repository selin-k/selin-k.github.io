#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"

#define DICTIONARY "large.txt"
#define INITIAL_SIZE 100

/** info about detected misspelled words **/
struct misspelling {
    int start,end;
}spell;

/** Array of misspelling structures **/
struct misspelling *miswords;

/** Unload the trie structure and free memory
 * @return 0 if sucessful, 1 otherwise 
 */ 
int unloadDictionary() {
    bool unloaded = unload();

    /** Check for errors **/
    if (!unloaded) {
        printf("Could not unload %s.\n", DICTIONARY);
        return 1;
    }
    return 0;
}

/**
 * Loads a dictionary into the Trie.
 * @return 0 if successful, 1 otherwise.
 */ 
int loadDictionary()
{
    bool loaded = load(DICTIONARY);

    /** Check for errors **/
    if (!loaded) {
        printf("Could not load %s.\n", DICTIONARY);
        return 1;
    }

    /** initialize the array of misspelled words **/
    miswords = malloc(INITIAL_SIZE  * sizeof(spell));

    /** Check for malloc errors **/
    if (miswords == NULL) {
        printf("Could not allocate space. \n");
        return 1;
    }
    
    return 0;
}

/**
 * Given the index, return a misspelling structure.
 * This stores the start and end index of a misspelled
 * word in a given row of text.
 * @param index is the nth misspelled word in the row.
 * @return the stored structure of a misspelled word.
 */
struct misspelling getMisspellings(int index) {
    return miswords[index];
}

/**
 * Given a row of text, fills in misspelling structures and 
 * appends them to an array. Uses the Trie created in the 
 * dictionary.c file to check each word in the text against 
 * a loaded dictionary.
 * @param text is the row of text to be checked.
 * @param len is the length of text.
 * @param misspellings is the number of misspellings in the text.
 * @return the number of misspelled words in text.
 */ 
int spellChecker(const char* text, int len, int misspellings) {
    int index = 0, words = 0, cursor = 0;
    char word[LENGTH+1];

    /** Iterate over the given text **/
    for (int i = 0; i < len; i++) {

        /** Find acceptable words that can be checked **/
        if (isalpha(text[i]) || (text[i] == '\'' && index > 0)) {
            /** Accept usual letters and apostrophes, 
             * append to the word string **/

            word[index] = text[i];
            index++;

            /** If the word exceeds the maximum 
             * (45 in English), skip it **/
            if (index > LENGTH) {
                while (isalpha(text[i])) i++;
                index = 0;
            }

        } else if (isdigit(text[i])) {
            /** Skip words with digits **/
            while (isalnum(text[i])) i++;
            index = 0;

        } else if (index > 0) {
            /** For an acceptable word, check
             * its spelling against the dictionary **/

            word[index] = '\0';
            words++;

            if (!check(word)) {
                /** If check returns false, the word is misspelled.
                 * Hence, fill in the misspellings structure and 
                 * put these in the allocated array **/

                /** Reallocate space for the array if it is
                 * exceeded. **/
                if (misspellings-1==INITIAL_SIZE) {
                    struct misspelling *miswords = realloc(miswords, 
                    (sizeof(spell) * INITIAL_SIZE*2));
                }

                miswords[misspellings].start = (cursor-index);
                miswords[misspellings].end = cursor;
                misspellings++;
            }
            index = 0;
        }
        cursor++;
    }

    /** Cleaning up **/
    return misspellings;
}



