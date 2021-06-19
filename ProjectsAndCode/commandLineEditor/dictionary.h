#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <stdbool.h>

/** maximum length for a word **/
#define LENGTH 45

/** Returns true if word is in dictionary 
 * else false. **/
bool check(const char* word);

/** Loads dictionary into memory. Returns 
 * true if successful else false. **/
bool load(const char* dictionary);

/** Unloads dictionary from memory. 
 * Returns true if successful else false. **/
bool unload();

#endif
