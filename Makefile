CC=cc -O3
CFLAGS=-Wall -Wextra -Wno-parentheses # -O2 triggers compiler bug in acc
RM=rm -f

SRC=adler32.c bsd.c cksum.c main.c crc32.c md5.c sha1.c sha2.c finalize.c output.c sysv.c
OBJ=adler32.o bsd.o cksum.o main.o crc32.o md5.o sha1.o sha2.o finalize.o output.o sysv.o
# Remove binaries if you don't want digest to replace these
DIGESTBIN=adler32sum bsdsum cksum crc32sum md5sum sha1sum sha256sum sha224sum sum
BINARY=digest
# Change these to the appropriate directories of your operating system
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
MANDIR=$(PREFIX)/share/man


all: $(BINARY)

include dep.mf

dep:
	$(CC) -MM -MG $(SRC) >dep.mf

$(BINARY): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

clean:
	$(RM) $(BINARY) *.o

install: $(BINARY)
	mkdir -p $(BINDIR)
	mkdir -p $(MANDIR)/man1
	cp $(BINARY) $(BINDIR)
	for i in $(DIGESTBIN) ; do ln -f $(BINDIR)/$(BINARY) $(BINDIR)/$$i ; done
	cp $(BINARY).1 $(MANDIR)/man1
