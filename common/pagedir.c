/*
 * pagedir.c - utility functions for intializing a pageDirectory for
 * crawler and saving webpages to the directory.
 *
 * Marshall Carey-Matthews - 19 October 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "pagedir.h"
#include "webpage.h"


/*=============================Local Methods==============================*/
/* check_slash: This function checks to see if the input for directories
 * contains "/" at the end of the function or not. This is important for
 * functions related to writing/validating the crawler dotfile.
 *
 * Caller provides:
 *  char* for directory name
 * We return:
 *  true, if the directory name has "/" as the last character
 */
static bool check_slash(const char* pageDirectory) {
  int length = strlen(pageDirectory);
  if (length == 0) { //empty string
    return false;
  }
  
  char finalChar = pageDirectory[length-1];
  return finalChar == '/';
}

/*=============================Public Methods=============================*/

//pagedir_init: see pagedir.h for documentation

bool pagedir_init(const char* pageDirectory){

  //create path string and allocate room for /.crawler(\0)
  char* path = malloc(sizeof(char) * (strlen(pageDirectory) + 10));

  //if path could not allocate memory, return false immediately
  if (path == NULL) {
    fprintf(stderr, "Failed to allocate memory!");
    return false;
  }

  //copy the directory and /.crawler to the path
  strcpy(path, pageDirectory);
  if(!check_slash(pageDirectory)) { //in the case that caller did not add '/'
    strcat(path, "/"); //add slash to enter directory
  }
  strcat(path, ".crawler");

  //create new file, should work if directory exists
  FILE *fp = fopen(path, "w");

  //free path
  free(path);

  //if it doesn't work, fp will be null and we return false
  if (fp) {
    fclose(fp);
    return true;
  } else {
    return false;
  }
}

//pagedir_save: see pagedir.h for documentation

void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
  //calculate the number of digits in the document ID, then add 2 for "/\0"
  int numDigits = floor(1 + log10(docID)) + 2;

  //create path string and allocate room for filename
  char* path = malloc(sizeof(char) * (strlen(pageDirectory) + numDigits));

  //if path is NULL, return immediately
  if (path == NULL) {
    fprintf(stderr, "Failed to allocate memory!");
    return;
  }

  //create ID and copy to path
  char IDString[numDigits-1];
  sprintf(IDString, "%d", docID);
  strcpy(path, pageDirectory);
  if (!check_slash(pageDirectory)) {
    strcat(path, "/"); //add slash to enter directory
  }
  strcat(path, IDString);

  //open file for writing
  FILE *fp = fopen(path, "w");

  //free path
  free(path);

  //if fp is NULL, return immediately
  if (fp == NULL) {
    return;
  }

  //print the URL of the webpage
  fprintf(fp, "%s\n", webpage_getURL(page));

  //print depth
  int webDepth = webpage_getDepth(page);
  int depthDigits = floor(2+log10(webDepth+1));
  char depthString[depthDigits];
  sprintf(depthString, "%d", webDepth);
  fprintf(fp, "%s\n", depthString);
  
  //print contents of the webpage
  fprintf(fp, "%s", webpage_getHTML(page));

  fclose(fp);
}

//pagedir_validate: see pagedir.h for documentation

bool pagedir_validate(const char* pageDirectory) {
  //create path string and allocate room for filename
  char* path = malloc(sizeof(char) * (strlen(pageDirectory) + 10));

  //if path is NULL, return false immediately
  if (path == NULL) {
    fprintf(stderr, "Failed to allocate memory!");
    return false;
  }

  //copy directory and crawler basename to path
  strcpy(path, pageDirectory);
  if (!check_slash(pageDirectory)) {
    strcat(path, "/");
  }
  strcat(path, ".crawler");

  //attempt to open file
  FILE* fp = fopen(path, "r");

  //free path
  free(path);

  //return whether the file could be opened
  if (fp) {
    fclose(fp);
    return true;
  } else {
    return false;
  }
}
