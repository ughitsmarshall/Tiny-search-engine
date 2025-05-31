# CS50 TSE Indexer

## Implementation Spec

In this document we reference the Requirements Specification and Design Specification and focus on the implementation-specific decisions.

## Data Structures

Our main data structure is the index data structure, whose documentation is outlined in the index.h file in the common directory.

## Control Flow

The indexer itself if implemented in one file `indexer.c`, with three functions.

### main

The main function validates the arguments (using dunctions from `pagedir.h` ), calls `indexBuild` , then calls `index_save` from the index module, then exits zero.

### indexBuild

indexBuild is the function that instantiates and returns an index. If there is an error or invalid input, then the function will return NULL.

Pseudocode:

```
Create a new index
loop through docIDs starting from 1
    make path to file in directory
    if it cannot be read
        break
    read file to generate webpage, return if NULL
    call indexPage
    delete the generated webpage
    increment docID
return index
```

### indexPage

indexPage is the function that takes words from the generated webpage and adds them to the instantiated index.

```
loop through all words in the webpage
    if the word is < 3 characters, don't add it
    normalize the word
    add the word to the index
```

## Other Modules

### pagedir

We create a re-usable module `pagdir.c` to handle the validation of directory names to check if they contain a crawler tag. This module is in `../common` .
Pseudocode:

```
create copied path of page directory
concatenate with ".crawler"
attempt to read file
if successful,
    close and return true
else
    return false
```

### libcs50

We leverage the modules of libcs50, most notably `hashtable` and `webpage` . See that directory for module interfaces. The `index` module uses hashtable, and the indexer must generate webpage_t* objects to add words to the index.

## Function Prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `indexer.c` and is not repeated here.

```
int main(const int argc, char* argv[]);
static index_t* indexBuild(const char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, int docID);
```

### pagedir

Detailed descriptions of the function's interface is provided as a paragraph comment prior to the functions declaration in `pagedir.h` and is not repeated here.

```
bool pagedir_validate(const char* pageDirectory);
```

## Error Handling and Recovery

All of the command-line parameters are checked before any data structures are allocated or work begins; problems result in a message printed to stderr and non-zero exit status.

All data structures that require memory allocation are checked for NULL results, which result in a NULL return for any index objects.

## Testing Plan

We test indexer through two methods. Firstly, integration testing is used to test invalid arguments, as well as valid arguments for multiple tests. Then, index duplication is tested with a small driver file called `indextest.c` which attempts to duplicate one index to another.
