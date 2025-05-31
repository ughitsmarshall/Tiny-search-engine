/*
 * index.c - TSE index module
 *
 * see index.h for more information
 *
 * Marshall Carey-Matthews - 24 October 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hashtable.h"
#include "counters.h"
#include "mem.h"
#include "file.h"

/********** file-local global variables **********/
//none

/********** local types **********/
//none

/********** global types **********/
typedef struct index {
  hashtable_t* hashtable;
} index_t;

/*============================Local Methods============================*/
/* line_build: take a counters and filepath and append the key and value
 * pair to the file. Helper function for parse_index.
 *
 * Caller provides:
 *  valid key, item pair for the counter, valid filepath
 * We return:
 *  void, as we just want to modify the opened file
 */
static void line_build(void* fp, const int key, const int count) {
  fprintf(fp, " %d %d", key, count);
}

/* parse_index: take a filepath and counterset as argument and build a
 * string to return in order to write a line of an index file.
 *
 * Caller provides:
 *  valid key, item pair for the counterset
 * We return:
 *  a constructed string, or NULL if counters could not be parsed
 */
static void parse_index(void* fp, const char* key, void* item) {
  if (fp == NULL) { //this means fp is NULL or was failed to pass
    return;
  }

  fprintf(fp, "%s", key);
  
  //pass an iterator down to the counters to collect docID and count
  counters_iterate(item, fp, line_build);

  //add newline for next set of word and counts
  fprintf(fp, "\n");
}

/* remove_count: given a void* item representing a counter, delete the
 * counter if it is not NULL. Helper function for index_delete.
 *
 * Caller provides:
 *  a pointer to an item that is a counter
 * We return: nothing
 */
static void remove_count(void* counter) {
  if (counter != NULL) {
    counters_delete(counter);
  }
}

/*============================Public Methods===========================*/

//index_new: see index.h for documentation

index_t* index_new(int slots) {
  index_t* index = mem_malloc(sizeof(index_t));

  if (index != NULL) { //otherwise memory error
    index->hashtable = hashtable_new(slots);
    //if there is an error handled in hashtable, index->hashtable == NULL
  }
  if (index->hashtable == NULL) { //don't return an empty index shell
    mem_free(index);
  }
  return index;
}

//index_add: see index.h for documentation

int index_add(index_t* index, const char* word, const int docID) {
  //search for existing counter
  //you need not verify if docID is contained because of the functionality
  //of counters_add()
  counters_t* found = hashtable_find(index->hashtable, word);
  if (found == NULL) { //then it is not found, make new counter
    hashtable_insert(index->hashtable, word, counters_new());
  }
  return counters_add(hashtable_find(index->hashtable, word), docID);
}

//index_save: see index.h for documentation

bool index_save(index_t* index, const char* filepath) {
  //attempt to write to file
  FILE* fp = fopen(filepath, "w");
  if (fp == NULL) { //could not open file
    fprintf(stderr, "Could not open file at path: %s\n", filepath);
    return false;
  }

  //now we want to iterate through every counter and add the k,v pairs
  //to the file, with the word at the beginning of each line
  hashtable_iterate(index->hashtable, fp, parse_index);

  fclose(fp);
  return true;
}

//index_read: see index.h for documentation

index_t* index_read(char* filepath) {
  //attempt to read file
  FILE* fp = fopen(filepath, "r");
  if (fp == NULL) { //could not open file
    fprintf(stderr, "Could not open file at path: %s\n", filepath);
    return NULL;
  }

  //instantiate the index to be returned (1.3*nWords efficient w/o math)
  index_t* newIndex = index_new(1.3*(file_numLines(fp)));

  //read each line of the file by grabbing new word and scanning
  char* fileLine;
  while ((fileLine = file_readWord(fp)) != NULL) {
    int docID;
    int count;
    //should stop at newline
    while (fscanf(fp, " %d %d", &docID, &count) == 2) {
      //add appropriate number of counts to the counterset
      for (int i=0; i < count; i++) {
        index_add(newIndex, fileLine, docID);
      }
    }

    free(fileLine);
  }

  fclose(fp);
  return newIndex;
}

//index_delete: see index.h for documentation

void index_delete(index_t* index){
  if (index != NULL){
    hashtable_delete(index->hashtable, remove_count);
    free(index);
  }
}

//index_find: see index.h for documentation

void* index_find(index_t* index, const char* key){
  return hashtable_find(index->hashtable, key);
}
