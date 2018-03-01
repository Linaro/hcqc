CC = gcc
CFLAGS = -std=c99 -g -DERROR_RATIO=10

.PHONY: all clean

all: libHCQC.a

libHCQC.o: ../libHCQC.h ../libHCQC.c
	$(CC) -I .. $(CFLAGS) -c ../libHCQC.c

libHCQC.a: libHCQC.o
	ar r libHCQC.a libHCQC.o

clean:
	-rm -f libHCQC.a libHCQC.o
