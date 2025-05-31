# Makefile for 'crawler' module
#
# Marshall Carey-Matthews - 19 October 2024

LIBS = ../libcs50/libcs50-given.a ../common/common.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: valgrind clean test

crawler: crawler.o $(LIBS)
	$(CC) $(CFLAGS) crawler.o $(LIBS) -lm -o $@

crawler.o: $(LIBS)

valgrind: crawler
	$(VALGRIND) ./crawler

test: crawler
	bash testing.sh &> testing.out

clean:
	rm -f *~ *.o
	rm -f bagtest
	rm -f core
	rm -f crawler
