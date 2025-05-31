# Querier Design Spec

## User interface

After inputting the valid arguments (pageDirectory and indexFilename) to initialize the querier, the user interface comprises of a prompt at the terminal that asks for a query.

## Inputs and outputs

The user inputs the words that they want to search for in the given index. They may use `and` and `or` to modify the search parameters. The output result is a debrief of the number of results found

## Modules

We anticipate the following modules or functions

1. main, which parses arguments
2. parses, which parses the queries
3. merger, which merges counters for scoring
4. intersecter, which intersects counters for scoring

## Pseudocode

The querier will run as follows:

    validate arguments, initialize index
    while the user inputs queries
        parse the query
        for each word in the query
            if it's an and operator, continue
            if it's an or operator, merge and continue
            search for the regular word in the index
            intersect what is found with the existing and statement

    for all of the documents found
        selection sort the documents by score

    print out the results of the document scores by collecting URL from crawler directory

## Data Structures

The major data structure used is the index data structure which is described in greater detail in the `index.h` and `indexer.c` files.

## Testing plan

The querier will be tested using a series of arguments that are known to cause errors or be invalid. Furthermore, several varieties and orders of the same query will be planted to verify that the "order of operations" works as intended.
