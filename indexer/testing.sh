#!/bin/bash

# test file for indexer.c and indextest.c
# indexer pageDirectory indexFilename
# indextest oldFilename newFilename
# Marshall Carey-Matthews 30 October 2024

echo "Testing invalid args...\n"

./indexer doogle boogle
./indexer ../blonk ../blonk/blonk.index

echo "Testing no args...\n"

./indexer

echo "Testing one arg...\n"

./indexer ../crawler/fourthtest

echo "Testing three args...\n"

./indexer ../crawler/fourthtest ../crawler/fourthtest/fourthtest.index 5

echo "Testing invalid directory path...\n"

./indexer ../crawler/fakepath fake.index

echo "Testing invalid directory path...\n"

./indexer ../crawler/fourthtest ../crawler/fakepath/invalid.index

echo "Testing read-only directory...\n"

./indexer ../crawler/fourthtest ../crawler/readonlydir/good.index

echo "Testing read-only file...\n"

./indexer ../crawler/fourthtest ../crawler/fourthtest/readonlyindex.index

echo "Testing several valid tests...\n"

./indexer ../crawler/fourthtest ../crawler/fourthtest/fourthtest.index

./indexer ../crawler/fifthtest ../crawler/fifthtest/fifthtest.index

./indexer ../crawler/sixthtest ../crawler/sixthtest/sixthtest.index

echo "Testing indextest...\n"

./indextest ../crawler/fourthtest/fourthtest.index ../crawler/fourthtest/fourthtest.indexcopy
