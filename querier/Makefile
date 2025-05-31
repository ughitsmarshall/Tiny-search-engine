# Makefile for 'querier' module
#
# Marshall Carey-Matthews 4 November 2024

LIBS = ../common/common.a ../libcs50/libcs50-given.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: valgrind clean test all

all: querier

querier: querier.o $(LIBS)
	$(CC) $(CFLAGS) querier.o $(LIBS) -lm -o $@

valgrind: querier
	$(VALGRIND) ./querier

test: querier
	bash -v testing.sh &> testing.out

clean:
	rm -f *~ *.o
	rm -f core
	rm -f querier
