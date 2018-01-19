CC = gcc
CFLAGS = -std=c99 -g
#CHECK_CFLAGS = -DERROR_RATIO=9

.PHONY: all test clean

all: libHCQC.a

libHCQC.o: libHCQC.c
	$(CC) $(CFLAGS) -c libHCQC.c

libHCQC.a: libHCQC.o
	ar r libHCQC.a libHCQC.o

savetest: io-main.c io-kernel.c
	$(CC) -DINIT_PHASE_P io-main.c io-kernel.c -o savetest libHCQC.a

loadtest: io-main.c io-kernel.c
	$(CC) io-main.c io-kernel.c -o loadtest libHCQC.a

test: savetest loadtest
	savetest && loadtest 

clean:
	-rm -f libHCQC.a libHCQC.o savetest loadtest check.dat
