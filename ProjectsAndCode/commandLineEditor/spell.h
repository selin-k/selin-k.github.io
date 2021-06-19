#ifndef SPELLER_H
#define SPELLER_H

/** Stores the start and end index of
 * each misspelled word in a given row **/
struct misspelling {
    int start,end;
}spell;

/** Array of misspelling structures **/
struct misspelling *miswords;

/** Given an index, returns that misspelling structure **/
struct misspelling getMisspellings(int index);

/** Given a row of text, its length, fills in the miswords array
 *  and returns the number of misspellings **/
int spellChecker(const char* text, int len, int misspellings);

/** Loads a given dictionary to the Trie, returns 
 * 0 if successful, 1 if not. **/
int loadDictionary();


/** Frees the Trie from memory, returns 0 if
 * successfulm 1 if not.
 */ 
int unloadDictionary();


#endif