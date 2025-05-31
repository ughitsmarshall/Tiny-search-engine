/*
 * index.h - common module for Tiny Search Enging
 *
 * An index is comprised of a hashtable that holds a string (word) key,
 * counters (document list) pair. Within the document list counters are any
 * number of document IDs with a count of the number of occurrences of the
 * word. See hashtable.h and counters.h in the libcs50/ directory for more
 * information on the base data structures.
 *
 * Marshall Carey-Matthews - 24 October 2024
 */

#include <stdio.h>
#include <stdbool.h>


/*===================== global types =========================*/
typedef struct index index_t; //opaque to users of the module

/*===================== functions ============================*/

/********** index_new **********/
/* Create a new (empty) index structure.
 *
 * Caller provides:
 *  number of desired slots
 * We return:
 *  pointer to a new index, NULL if error (out of memory).
 * We guarantee:
 *  the index is initialized empty.
 * Caller is responsible for:
 *  later calling index_delete();
 */
index_t* index_new(int slots);

/********** index_add **********/
/* Increment the count for the counter associated with the word and
 * document ID.
 *
 * Caller provides:
 *  valid pointer to the index, a word, and document ID
 * We return:
 *  the new value of the word count for the document
 *  0 on error (if the index or counterset is null, key is invalid,
 *  or out of memory)
 * We guarantee:
 *  the count for the word and document ID will be >= 1 on successful return
 * We do:
 *  ignore if the index is NULL or key is negative/NaN
 *  if the word is not yet documented, insert a new counters object
 *  if the document is not yet in the counters object, initialize to 1
 *  if the word and document are in the index, increment the counter by 1
 */
int index_add(index_t* index, const char* word, const int docID);

/********** index_save **********/
/* Write the given index to a new file according to format described in
 * indexer Requirements documentation.
 *
 * Caller provides:
 *  valid pointer to index and valid filepath to write index
 * We return:
 *  true if the file was written successfully, false if it was not
 * We guarantee:
 *  if the file opening is successful, the index will be written in format
 * We do:
 *  nothing, if file open fails or index is NULL
 *  otherwise, open/overwrite a file and copy the index data into it
 * Note:
 *  words and counts are unordered
 *
 * IMPORTANT: INDEX FILES SHOULD ONLY BE HELD IN DIRECTORIES WITH .crawler
 *  TAG! This is to ensure that the file being written is a TSE index.
 */
bool index_save(index_t* index, char* filepath);

/********** index_read **********/
/* Read an index file and construct an index from it.
 *
 * Assumptions:
 *  the index file being read MUST be in a crawler directory with a
 *  .crawler tag. This is to ensure we are ACTUALLY reading a valid
 *  index file.
 *
 * Caller provides:
 *  valid filepath to an index file and valid filepath to a new file
 * We return:
 *  the constructed index if successful, NULL if there is an error
 * We guarantee:
 *  if the file opening is successful, the index will be constructed
 * We do:
 *  nothing, if file opening fails or memory error
 *  make an index matching the form of the file, if successful
 * Note:
 *  words and counts are unordered
 */
index_t* index_read(char* filepath);

/********** index_delete **********/
/* Delete the whole index.
 *
 * Caller provides:
 *  a valid pointer to the index
 * We do:
 *  we ignore NULL indexes
 *  we free all memory we allocate for this index
 */
void index_delete(index_t* index);

/********** index_find **********/
/* Find and item in the index.
 *
 * Caller provides:
 *  a valid pointer to the index, valid pointer to a key
 * We return:
 *  a pointer to a counter, if found
 *  NULL, if not found
 */
void* index_find(index_t*, const char* key);
