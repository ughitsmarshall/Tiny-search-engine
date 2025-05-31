/* querier.c - driverfile for indexer functionality
 *
 * Marshall Carey-Matthews, 3 Novermber 2024
 *
 */

#include <stdlib.h>
#include <math.h>
#include "pagedir.h"
#include "index.h"
#include "word.h"
#include "counters.h"
#include "hashtable.h"
#include "file.h"

//Querier functions
static void intersect(counters_t* c1, counters_t* c2);
static void intersect_helper(void* arg, const int key, const int count);
static void  merge(counters_t* c1, counters_t* c2);
static void merge_helper(void* arg, const int key, const int count);
static void count_docs(void* arg, const int key, const int count);
static void countersort(void* arg, const int key, const int count);


/* Handle command inputs and initiate the querier
 * Input paradigm is `./querier pageDirectory indexFilename`
 * In theory the pageDirectory should have the same content
 * as the indexFilename, but the requirements do not outline checks
 * to verify that this is the case.
 */
int main(const int argc, char* argv[]) {
  
  //validate arguments
  if (argc != 3) {
    fprintf(stderr, "Please provide pageDirectory and indexFilename only!");
    return 1;
  }

  if(!pagedir_validate(argv[1])) {
    fprintf(stderr, "pageDirectory is not a crawler directory!");
    return 1;
  }

  //attempt to read file to index
  index_t* qIndex = index_read(argv[2]);
  if (qIndex == NULL) {
    fprintf(stderr, "Could not build document index!");
    return 1;
  }

  /* we use a length size_t and char* with standard i/o library's
   * getline() function to grab a copy of the input line
   */
  char input[1000];
  char* buffer;
  char** query;
  fprintf(stdout, "Query?\n");
  while((buffer = fgets(input, sizeof(input), stdin)) != NULL){
    if (strlen(input) == 1) {
      break;
    }
    //now we use the word functions to parse the query
    query = word_parse(input);
    if (query == NULL) {
      continue;
    }

    //return query
    word_print(query);

    //iterate through the valid query to score the docs
    counters_t* orCount = counters_new();
    if (orCount == NULL) {
      fprintf(stderr, "Memory error.");
      return 2;
    }
    counters_t* andCount = NULL;
    for (int i = 0; query[i]; i++) {
      //handle and/or operators
      if (strcmp(query[i], "or") == 0){
        //merge orCount and andCount
        merge(orCount, andCount);
        counters_delete(andCount);
        andCount = NULL;
        continue;
      } else if (strcmp(query[i], "and") == 0){
        //because 'and' is natural, you can just continue
        continue;
      }

      counters_t* found = index_find(qIndex, query[i]);
      if (andCount == NULL) { //move found to and statement
        andCount = counters_new();
        merge(andCount, found);
        continue;
      } else {
        intersect(andCount, found);
      }
    }
    //final merge for query
      merge(orCount, andCount);
      counters_delete(andCount);

    //check the number of found documents. if 0, say no docs found
    int foundDocs = 0;
    counters_iterate(orCount, &foundDocs, count_docs);
    if(!foundDocs) {
      fprintf(stdout, "No documents found with this query!\n");
    } else {
      fprintf(stdout, "Matches %d documents (ranked):\n", foundDocs);
    }
    for (int i = 0; i < foundDocs; i++) {
      //selection sort and print the top document
      int topDoc[2];
      topDoc[0] = 0;
      topDoc[1] = 0;
      counters_iterate(orCount, topDoc, countersort);
      counters_set(orCount, topDoc[0], 0);
        
      //open file for the top document
      int IDSize = floor(3+log10(topDoc[0]));
      char IDString[IDSize];
      const char* dir = argv[1];
      char* path = malloc(sizeof(char) * (strlen(dir) + IDSize));

      strcpy(path, dir);
      if (dir[strlen(dir)-1] != '/') { //add slash
        strcat(path, "/");
      }
      sprintf(IDString, "%d", topDoc[0]);
      strcat(path, IDString);

      FILE* fp = fopen(path, "r");
      if (fp == NULL) {
        fprintf(stderr, "Could not read file at path: %s\n", path);
        free(path);
        continue;
      }
      char* URL = file_readLine(fp);
      free(path);
      fclose(fp);

      //print out the URL read from document
      fprintf(stdout, "Score\t%d Doc\t%d: %s\n", topDoc[1], topDoc[0], URL);
      free(URL);
    }
    counters_delete(orCount);
    for (int i = 0; query[i]; i++) {
      free(query[i]);
    }
    free(query);
    fprintf(stdout, "Query?\n");
  }
  index_delete(qIndex);
  return 0;
}

/* countersort: change the "topDoc" argument to be the id of the
 * webpage with the highest score in the counterset
 */
static void countersort(void* arg, const int key, const int count) {
  int* topArray = arg;
  if(topArray[1] < count){
    topArray[0] = key;
    topArray[1] = count;
  }
}

/* count_docs: count the number of documents given a counterset
 */
static void count_docs(void* arg, const int key, const int count) {
  int* found = arg;
  if(count){
    (*found)++;
  }
}

/* merge: finds the union of two countersets
 */
static void merge(counters_t* c1, counters_t* c2){
  counters_iterate(c2, c1, merge_helper);
}

/* helper function for merge. adds counts of the two counters to each other.
 */
static void merge_helper(void* arg, const int key, const int count) {
  counters_t* c1 = arg;
  counters_set(c1, key, count + counters_get(c1, key));
}

/* intersect: finds the intersection of two countersets
 */
static void intersect(counters_t* c1, counters_t* c2){
  counters_t* countersets[2];
  countersets[0] = c1;
  countersets[1] = c2;
  counters_iterate(c1, countersets, intersect_helper);
}


/* helper function for intersect. compares the counters in two
 * countersets. if both have counters, set the resulting counter to
 * contain the lower of the two counts
 */
static void intersect_helper(void* arg, const int key, const int count) {
  counters_t** countersets = arg;

  //if you can find the item in c2 then set counter to lower of the two
  int num = counters_get(countersets[1], key);
  if (num > count) {
    counters_set(countersets[0], key, count);
  } else {
    counters_set(countersets[0], key, num);
  }
}
