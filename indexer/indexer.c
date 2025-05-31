/* index.c - driverfile for indexer functionality
 *
 * Marshall Carey-Matthews, 28 October 2024
 *
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "pagedir.h"
#include "index.h"
#include "file.h"
#include "hashtable.h"
#include "counters.h"

//Indexer functions
static index_t* indexBuild(const char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, int docID);

/* Handle command inputs and initiate the indexer
 * Input paradigm is `./indexer pageDirectory indexFilename
 */
int main(const int argc, char* argv[]) {

  //validate arguments
  if (argc != 3) {
    fprintf(stderr, "Please provide pageDirectory and indexFilename only!");
    return 1;
  }

  if (!pagedir_validate(argv[1])) {
    fprintf(stderr, "pageDirectory is not a crawler directory!");
    return 1;
  }

  //build the index
  index_t* theIndex = indexBuild(argv[1]);
  if (theIndex == NULL) {
    return 1; 
  }

  //save the index to a file
  index_save(theIndex, argv[2]);

  index_delete(theIndex);

  return 0;
}

/* indexBuild: create an index using webpages from the pre-validated
 * crawler directory.
 */
static index_t* indexBuild(const char* pageDirectory) {
  //create the index
  index_t* theIndex = index_new(200);
  if (theIndex == NULL) {
    fprintf(stderr, "Error allocating memory!");
    return NULL;
  }

  int docID = 1;
  while (true) {
    //construct path to open file
    int IDSize = floor(3+log10(docID));
    char IDString[IDSize];
    char* path = malloc(sizeof(char) * (strlen(pageDirectory) + IDSize));

    strcpy(path, pageDirectory);
    if (pageDirectory[strlen(pageDirectory)-1] != '/') { //add slash
      strcat(path, "/");
    }
    sprintf(IDString, "%d", docID);
    strcat(path, IDString);

    FILE* fp = fopen(path, "r");
    if (fp == NULL) { //no such file, exit loop
      free(path);
      break;
    }

    //get information to generate webpage
    char* URL = file_readLine(fp);
    char* depthString = file_readLine(fp);
    int depth = atoi(depthString);
    char* HTML = file_readFile(fp);

    webpage_t* passPage = webpage_new(URL, depth, HTML);

    if(passPage == NULL) { //memory failure, exit
      if(URL){free(URL);}
      if(depthString){free(depthString);}
      if(HTML){free(HTML);}
      return NULL;
    }

    //pass successful webpage to handle word counts
    indexPage(theIndex, passPage, docID);

    //free allocations and increment the document ID
    webpage_delete(passPage);
    free(path);
    free(depthString);
    fclose(fp);
    docID++;
  }

  return theIndex;
}

/* indexPage: grabs words from webpage and adds them to index
 */
static void indexPage(index_t* index, webpage_t* page, int docID) {
  char* word;
  int pos = 0;

  while((word = webpage_getNextWord(page, &pos)) != NULL) {
    //ignore small words
    if (strlen(word) < 3) {
      free(word);
      continue;
    }
    //normalize the word
    for (int i = 0; word[i]; i++) {
      word[i] = tolower(word[i]);
    }
    //add the word to the index
    index_add(index, word, docID);
    //free the word
    free(word);
  }
}
