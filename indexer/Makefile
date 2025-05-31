# Makefile for 'indexer' module
#
# Marshall Carey-Matthews 29 October 2024

LIBS = ../common/common.a ../libcs50/libcs50-given.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: valgrind clean test all

all: indexer indextest

indextest: indextest.o $(LIBS)
	$(CC) $(CFLAGS) indextest.o $(LIBS) -lm -o $@

indexer: indexer.o $(LIBS)
	$(CC) $(CFLAGS) indexer.o $(LIBS) -lm -o $@

indexer.o: $(LIBS)

valgrind: indexer
	$(VALGRIND) ./indexer

test: indexer indextest
	bash testing.sh &> testing.out

clean:
	rm -f *~ *.o
	rm -f core
	rm -f indexer
	rm -f indextest
