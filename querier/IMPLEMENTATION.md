# Querier Implementation Spec

## Data structures

Our data structures include the index read from indexFilename, as well as several counters that help score the documents counted in the querier.

## Control flow

The querier is implemented in one file `querier.c`, with four main functions and two sub-functions.

### main
This function handles the parsing of arguments, as well as the loop that untangles the query and scores documents. See the design spec for an overview of the main function's logic tree.

### intersect / intersect_helper
This function bases itself on the counters_iterate function; thus, it requires a helper function (see `counters.h` in the `libcs50` library for more information). It finds the intersection of two counters. In essence:

    Using the parameters sent to the helper function
        compare the counts of a word occurrence in a given document of the intersected words
        set the count in the and statment to be the lower of the two

### merge / merge_helper
This function also leverages the counters_iterate function.

    Using the parameters send to the helper function
        add the count of one word occurrence to another in a given document

### count_docs
This function also leverages the counters_iterate function.

    Simply, when called, increment the value at an int pointer that holds the number of documents counted.

### countersort
This function compares the values of two documents in order to score them
    
    if a document's score is greater than the one compared
        set the key and count for that document as the top document


## Modules

The querier leverages the `common` and `libcs50` libraries. Specifically, it mostly uses the `counters.h` and `word.h` modules. All other modules used are better described in the `crawler` or `indexer` implementations.

## Error Handling

All out-of-memory errors are handled as needed by comparisons to NULL. Many of the errors relating to invalid arguments that are handled in other modules are handled there rather than handled in the driver file, since those modules are used elsewhere for identical purposes. See the `common` directory for information.

## Testing plan

We run a bash file that attempts several example queries based on those described in the design specification. Please see the design specification for more information.
