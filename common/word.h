/*
 * word.c - common module for Tiny Search Engine
 *
 * This module defines the use of query parsing functions used in the
 * Tiny Search Engine's querier. The module allows an input string to be
 * tokenized and parsed, such that scores can be pushed to the driver.
 *
 * Marshall Carey-Matthews - 3 November 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "bag.h"
/*===========================Global Functions===========================*/

/* word_parse: this is the function that splits an input line into tokens.
 *  Furthermore, the function validates whether the words of the query are
 *  appropriate. This includes: 
 *    NO special/numeric characters, 
 *    NO query entries under three characters (except "OR")
 *    NO consecutive and/or words
 *  
 *  The tokens are stored in a bag structure. For more information on the
 *  bag structure, please read the documentation in "bag.h" in the
 *  ../libcs50 directory.
 *
 * Caller provides: an existing input string to be parsed
 *
 * We return:
 *  a pointer to an array of words, if all tokens and arrangements
 *  are valid.
 *
 * Caller is responsible for: freeing the memory allocated to return the
 *  array.
 */

char** word_parse(char* input);


/* word_print: this is the function that prints a query given from a bag
 * of words.
 *
 * Caller provides: a pointer to a bag of strings
 *
 * We do:
 *  print the contents of the bag in the form of a query
 *  Query: xxxx xxxxx xxxxx xxxx
 */

void word_print(char** query);
