CC=cc
CFLAGS=-O3 -Wall -Wextra -Wno-parentheses # -O2 triggers compiler bug in acc
RM=rm -f

SRC=adler32.c bsd.c main.c crc32.c md5.c sha1.c sha2.c finalize.c
OBJ=adler32.o bsd.o main.o crc32.o md5.o sha1.o sha2.o finalize.o
DIGESTBIN=adler32sum bsdsum crc32sum md5sum sha1sum sha256sum sha224sum
BINARY=digest
AUXBIN=mkmd5tab mksha2tab mkcrc32tab
#AUXBIN=mkmd5tab mkcrc32tab
AUXHDR=crc32tab.h md5tab.h sha2tab.h
#AUXHDR=crc32tab.h md5tab.h
DEST=/usr

all: $(BINARY)

include dep.mf

dep:
	$(CC) -MM -MG $(SRC) mkmd5tab.c >dep.mf

$(BINARY): $(OBJ)
	$(CC) -o $@ $(OBJ)

md5tab.h: mkmd5tab
	./mkmd5tab >$@

mkmd5tab: mkmd5tab.c
	$(CC) $(CFLAGS) -o $@ mkmd5tab.c -lm

sha2tab.h: mksha2tab
	./mksha2tab >$@

mksha2tab: mksha2tab.c
	$(CC) $(CFLAGS) -o $@ mksha2tab.c -lm

crc32tab.h: mkcrc32tab
	./mkcrc32tab >$@

mkcrc32tab: mkcrc32tab.c
	$(CC) $(CFLAGS) -o $@ mkcrc32tab.c

clean:
	$(RM) $(BINARY) $(AUXBIN) $(AUXHDR) *.o

install: $(BINARY)
	mkdir -p $(DEST)/bin
	cp $(BINARY) $(DEST)/bin
	for i in $(DIGESTBIN) ; do ln -f $(DEST)/bin/$(BINARY) $(DEST)/bin/$$i ; done
	cp $(BINARY).1 $(DEST)/man/man1
