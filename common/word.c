/*
 * word.c - utility functions for parsing words entered by user into
 * the querier.
 *
 * Marshall Carey-Matthews - 3 November 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "word.h"
#include "bag.h"

/*============================Local Methods===============================*/

/*===========================Public Methods===============================*/

//word_parse: see word.h for documentation

char** word_parse(char* input) {

  //instantiate word array
  int wordCount = 0;
  int arraySize = 10;
  char** wordArray = malloc(sizeof(char*) * arraySize);
  if (wordArray == NULL){
    fprintf(stderr, "Memory error.");
    return NULL;
  }

  //begin parsing
  char* prev = NULL;
  char* word = NULL;
  int wordPos = 0;
  for (int i = 0; input[i]; i++) {
    if (input[i] == EOF){
      break;
    }
    //ignore whitespace, make new pointer if start of word
    if (word == NULL){
      if (isspace(input[i])) {
       continue;
      } 
      word = malloc(sizeof(char) * strlen(input));
    }

    //validate that the character is a letter or space
    if (isalpha(input[i])) {
      //normalize letter and add it to string
      char c = tolower(input[i]);
      word[wordPos] = c;
      wordPos++;
    } else if (isspace(input[i])) {
      //insert a null character just to be safe
      word[wordPos] = '\0';
      //chop off word and make sure that it is long enough
      if((strcmp(word, "or") != 0) && wordPos < 3){
        fprintf(stderr, "Query input too short! Word: %s\n", word);
        for (int j = 0; j < wordCount; j++){
          free(wordArray[j]);
        }
        free(word);
        free(wordArray);
        return NULL;
      }
      //do not allow the query to parse if and/or consecutively
      if(strcmp(word, "and") == 0 || strcmp(word, "or") == 0) {
        if(prev == NULL) {
          fprintf(stderr, "No leading and/or statements!\n");
          for (int j = 0; j < wordCount; j++){
            free(wordArray[j]);
          }
          free(word);
          free(wordArray);
          return NULL;
        }
        if(strcmp(prev, "and") == 0 || strcmp(prev, "or") == 0) {
          fprintf(stderr, "No consecutive and/or statements!\n");
          for (int j = 0; j < wordCount; j++) {
            free(wordArray[j]);
          }
          free(word);
          free(wordArray);
          return NULL;
        }
      }
      //put the word into the bag
    
      if (wordCount >= arraySize) {
        char** tempArray = realloc(wordArray, (arraySize*2 + 1) * sizeof(char*));
        if (tempArray == NULL) {
          free(wordArray);
          fprintf(stderr, "Memory error.");
          return NULL;
        } else {
          wordArray = tempArray;
        }
        arraySize = arraySize * 2;
      }

      wordArray[wordCount] = word;
      prev = word;
      word = NULL;
      wordPos = 0;
      wordCount++;
    } else {
      fprintf(stderr, "Non-letter/whitespace character: %c\n", input[i]);
      for(int j = 0; j < wordCount; j++){
        free(wordArray[j]);
      }
      free(word);
      free(wordArray);
      return NULL;
    }
  }
  //now check to make sure the last item is not and/or
  if (strcmp(prev, "and") == 0 || strcmp(prev, "or") == 0) {
    fprintf(stderr, "Do not end query with and/or!\n");
    for (int j = 0; j < wordCount; j++){
      free(wordArray[j]);
    }
    free(wordArray);
    return NULL;
  }
  wordArray[wordCount] = '\0';
  return wordArray;
}

//word_print: see word.h for documentation
void word_print(char** query){
  if (query == NULL) {
    return;
  }
  fprintf(stdout, "Query: ");
  for (int i = 0; query[i]; i++) {
    fprintf(stdout, "%s ", query[i]);
  }
  fprintf(stdout, "\n");
}
