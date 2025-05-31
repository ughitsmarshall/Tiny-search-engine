# Tiny Search Engine Common Modules

## Created by Marshall Carey-Matthews

## Structures

The modules `pagedir` and `index` help support the funcitonality of the crawler and indexer. There are currently on known limitations for their use on UNIX systems. 

### pagedir

See pagedir.h for documentation on its functions. For information on its applications, see the `crawler.c` and `indexer.c` functions in `../crawler` and `../indexer` directories, respectively.

### index

See index.h for documentation on structure. For information on its application, see the README in the `../indexer` directory.

### word

See word.h for documentation on structure. For information on its application, see the README in the `../querier` directory.

## Assumptions
Both `pagedir` and `index` are capable of dynamically allocating memory such that only hardware limitations would prevent the usage of the modules on a large scale. Furthermore, `pagedir` allows for flexible input styles for directories, so entering directories ending in "/" as arguments (or not doing so) will not cause an error. The `word` functionality always allocates memory as large as the original query for each word it parses, so it doesn't have limitations on its memory.
