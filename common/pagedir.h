/* 
 * pagedir.h - common module for Tiny Search Engine
 *
 * This module defines the use of the pageDirectory functions to be used
 * by the Tiny Search Engine. The module allows us to mark a directory as
 * a "crawler" directory, as well as save webpage objects to the directory
 * in files to be handled by the indexer.
 *
 * Marshall Carey-Matthews - 19 October 2024
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include "webpage.h"

/*=======================================================================*/

/* pagedir_init: this is the function that "marks" a directory as a crawler
 * directory.
 *
 * Caller provides: the path for the directory to be marked
 *
 * We return:
 *  true if we could successfully mark the directory
 *  false if we could not successfully mark the directory
 *
 * Caller is responsible for: being smart and providing a directory they
 *  don't want overwritten. Caller is also responsible for providing a
 *  directory name that actually exists.
 *
 * FILE FORMAT: pageDirectory/.crawler
 */

bool pagedir_init(const char* pageDirectory);

/* pagedir_save: this is the function that saves the webpage features/
 * constants to a new file within the marked crawler directory.
 *
 * We assume: 
 *  docID does not go above 9 digits
 *  webpage depth does not go above 9 digits
 *
 * Caller provides: the correct page directory to be written in.
 *
 * We return:
 *  Nothing if the file write is successful
 * 
 * Caller is responsible for: providing a directory that exists and has
 *  already been marked with a crawler file.
 *
 * FILE FORMAT: pageDirectory/docID
 *
 * IMPORTANT: This function will exit nonzero if pagedir_save is called on
 * an invalid directory or if fopen() did not work.
 */

void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/* pagedir_validate: this is the function that validates whether the
 * directory provided is a crawler directory.
 *
 * Caller provides: a valid page directory
 *
 * We return:
 *  True if the directory is a valid crawler directory (has .crawler tag)
 * 
 * Caller is responsible for: providing a directory
 */

bool pagedir_validate(const char* pageDirectory);
