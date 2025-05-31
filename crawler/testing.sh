#!/bin/bash

# test file for crawler.c
# seedURL pageDir maxDepth
# Marshall Carey-Matthews 21 October 2024

echo "Testing wrong number of args...\n"

./crawler 1 2 3 4 5 6 7 8

echo "Testing bad directory...\n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html notADir 3

echo "Testing bad website...\n"

./crawler yum.com firsttest 3

echo "Testing bad maxDepth (too big)...\n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html firsttest 99

echo "Testing bad maxDepth (not an integer)...\n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html firsttest 3.1

echo "Testing letters at depth 0...\n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html firsttest 0

echo "Testing toscrape at depth 0...\n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html secondtest 0

echo "Testing wikipedia at depth 0...\n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html thirdtest 0

echo "Testing letters at depth 10...\n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html fourthtest 10

echo "Testing toscrape at depth 1...\n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html fifthtest 1

echo "Testing wikipedia at depth 1...\n"

./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html sixthtest 1

echo "Testing complete!\n"


