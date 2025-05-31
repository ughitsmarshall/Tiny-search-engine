# Makefile for common directory for Tiny Search Engine
#
# Marshall Carey-Matthews - 19 October 2024

OBJS = pagedir.o index.o word.o
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
LIB = common.a
CC = gcc

.PHONY: valgrind clean

common.a: $(OBJS)
	ar cr -lm $(LIB) $(OBJS)

pagedir.o: pagedir.h ../libcs50/libcs50-given.a

index.o: index.h ../libcs50/libcs50-given.a

word.o: word.h ../libcs50/libcs50-given.a

clean:
	rm -f *~ *.o
	rm -f core
