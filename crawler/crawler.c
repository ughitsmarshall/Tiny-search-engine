/* crawler.c - driverfile for crawler functionality
 *
 * Marshall Carey-Matthews, 15 October 2024
 *
 */

#include <ctype.h>
#include <string.h>
#include "webpage.h"
#include "pagedir.h"
#include "hashtable.h"
#include "bag.h"

//Global variable for number of hashtable slots
static const int HASHTABLESIZE = 200;

//Crawler functions
static void parseArgs(const int argc, char* argv[], char** seedURL,
          char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/*
 * Handle command inputs and initiate the crawler
 * Input paradigm is `./crawler seedURL pageDirectory maxDepth`
 * Because argv[3] is a char* and not int*, we have to make a dummy
 * variable in order to call the function
 */
int main(const int argc, char* argv[]) {

  int maxDepth = -1;
  parseArgs(argc, argv, &argv[1], &argv[2], &maxDepth);

  crawl(argv[1], argv[2], atoi(argv[3]));

  exit(0);
}

/* Function that crawls the webpages.
 * Uses hashtable;bag as storage for searched;toSearch webpages
 * Adds to bag every new URL it finds, adds to hashtable every URL fetched
 */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) {
  //Intialize hashtable and bag for the crawler
  hashtable_t* URLTable = hashtable_new(HASHTABLESIZE);
  bag_t* toCrawl = bag_new();

  //Add seedURL to URLTable as it is already validated by parseArgs
  char* normSeed = normalizeURL(seedURL);
  hashtable_insert(URLTable, normSeed, "");

  //Convert seedURL to webpage, add toCrawl as a base for the crawler
  webpage_t* seedPage = webpage_new(normSeed, 0, NULL);
  bag_insert(toCrawl, seedPage);

  //numDocs becomes the docID when the document file is saved to pageDirectory
  int numDocs = 0;
  webpage_t* currPage;

  while ((currPage = bag_extract(toCrawl)) != NULL) {//true when bag is not empty
    numDocs++;

    //true when the webpage successfully fetches the HTML for the page
    if (webpage_fetch(currPage)) {

      //print to stdout to see progress
      fprintf(stdout, "Fetched: %s\n", webpage_getURL(currPage));

      //save the HTML to the pageDirectory
      const int docID = numDocs;
      pagedir_save(currPage, pageDirectory, docID);

      //only scan page if you haven't reached maximum depth
      if (webpage_getDepth(currPage) < maxDepth) {
        pageScan(currPage, toCrawl, URLTable);
      }
    }

    //delete the current webpage (CAUSES MEMORY ERROR)
    webpage_delete(currPage);
  }

  //free memory from the bag and hashtable
  bag_delete(toCrawl, webpage_delete);
  hashtable_delete(URLTable, NULL);
}

/* Function that scans the webpages to place unseen webpages in the bag
 * and corroborate all URLS against the hashtable. Only allows internal
 * links to be added.
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {

  //print to stdout to see progress
  fprintf(stdout, "Scanning: %s\n", webpage_getURL(page));

  //instantiate the seedURL
  int URLPos = 0;
  char* nextURL;
  
  //Check all URLs while there are URLs to check
  while ((nextURL = webpage_getNextURL(page, &URLPos)) != NULL) {

    char* nextNorm = normalizeURL(nextURL);
    if (isInternalURL(nextNorm)) { //only add if internal

      //this will be true if the key does not already exist in the table
      if (hashtable_insert(pagesSeen, nextNorm, "")) {

        int newDepth = webpage_getDepth(page) + 1;
        webpage_t* newPage = webpage_new(nextNorm, newDepth, NULL);

        //print to stdout to see progress
        fprintf(stdout, "Added: %s\n", webpage_getURL(newPage));
        bag_insert(pagesToCrawl, newPage);
      } else { //Free unused URL
        free(nextNorm);
      }
    } else { //print that this is not an internal URL and free it
      fprintf(stdout, "External, not added: %s\n", nextNorm);
      free(nextNorm);
    }
    //free URLs and instantiate the next URL
    free(nextURL);
  }
}

/* Function that parses the arguments entered and makes sure that they are
 * valid. If there is an error or the arguments aren't valid, exit non-zero.
 * exit(1) refers to incorrect number of arguments.
 * exit(2) refers to invalid argument.
 */
static void parseArgs(const int argc, char* argv[], char** seedURL, 
    char** pageDirectory, int* maxDepth) {

  //check argc to make sure the proper amount of arguments are there
  if (argc != 4) {
    fprintf(stderr, "Make sure you provide seedURL, pageDirectory, and maxDepth!\n");
    exit(1);
  }

  //check seedURL to see if it is internal
  const char* confSeed = *seedURL;
  char* normSeed = normalizeURL(confSeed);
  if (!isInternalURL(normSeed)){
    fprintf(stderr, "Make sure seedURL is internal!\n");
    fprintf(stderr, "Use http://, not https://\n");
    exit(2);
  }
  free(normSeed);

  //call pagedir_init() to make sure directory exists
  char* confDirectory = *pageDirectory;
  if(!pagedir_init(confDirectory)) {
    fprintf(stderr, "pageDirectory failed to initialize!\n");
    exit(2);
  }

  //ensure maxDepth is in range by reassigning maxDepth dummy var
  //in order to reassign, argv[3] must look like an int
  char* depthCheck;
  for (depthCheck = argv[3]; *depthCheck != '\0'; depthCheck++) {
    if (!isdigit(*depthCheck)) {
      fprintf(stderr, "maxDepth must be an integer!\n");
      exit(2);
    }
  }

  //now we have verified that argv[3] is an integer
  *maxDepth = atoi(argv[3]);

  if(*maxDepth < 0 || *maxDepth > 10){
    fprintf(stderr, "maxDepth must be greater than 0!\n");
    exit(2);
  }
}
