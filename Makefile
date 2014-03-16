CC=cc -m32
CFLAGS=-O3 -Wall -Wextra -Wno-parentheses # -O2 triggers compiler bug in acc
RM=rm -f

SRC=adler32.c bsd.c cksum.c main.c crc32.c md5.c sha1.c sha2.c finalize.c output.c
OBJ=adler32.o bsd.o cksum.o main.o crc32.o md5.o sha1.o sha2.o finalize.o output.o
DIGESTBIN=adler32sum bsdsum crc32sum md5sum sha1sum sha256sum sha224sum
BINARY=digest
DEST=/usr

all: $(BINARY)

include dep.mf

dep:
	$(CC) -MM -MG $(SRC) >dep.mf

$(BINARY): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

clean:
	$(RM) $(BINARY) *.o

install: $(BINARY)
	mkdir -p $(DEST)/bin
	cp $(BINARY) $(DEST)/bin
	for i in $(DIGESTBIN) ; do ln -f $(DEST)/bin/$(BINARY) $(DEST)/bin/$$i ; done
	cp $(BINARY).1 $(DEST)/man/man1
