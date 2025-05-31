#!/bin/bash

# test file for querier.c
# querier pageDirectory indexFilename
# Marshall Carey-Matthews 2 November 2024


echo "Testing invalid args...\n"

./querier sdgsdlsd sdlkjsdfljk
./querier ~/cs50-dev/shared/tse/output/wikipedia-2/ notAnIndex
./querier notaDir ~/cs50-dev/shared/tse/output/wikipedia-2.index

echo "Testing no args...\n"

./querier

echo "Testing one arg...\n"

./querier ~/cs50-dev/shared/tse/output/wikipedia-2/

echo "Testing three args...\n"

./querier ~/cs50-dev/shared/tse/output/wikipedia-2/ ~/cs50-dev/shared/tse/output/wikipedia-2.index 342902309423049

echo "Testing valid tests...\n"

./querier ~/cs50-dev/shared/tse/output/wikipedia-2/ ~/cs50-dev/shared/tse/output/wikipedia-2.index < validqueries.txt

echo "\n\nTesting invalid syntax...\n"

./querier ~/cs50-dev/shared/tse/output/wikipedia-2/ ~/cs50-dev/shared/tse/output/wikipedia-2.index < invalidqueries.txt
