CC = gcc
CFLAGS = -std=c99 -g
#CHECK_CFLAGS = -DERROR_RATIO=9
ARCH = x86_64
#ARCH = aarch64

.PHONY: test clean

savetest: io-main.c io-kernel.c
	$(CC) -D GEN_CHECK_DATA_P io-main.c io-kernel.c -o savetest $(ARCH)/libHCQC.a

loadtest: io-main.c io-kernel.c
	$(CC) io-main.c io-kernel.c -o loadtest $(ARCH)/libHCQC.a

test: savetest loadtest
	savetest && loadtest 

clean:
	-rm -f savetest loadtest check.dat
