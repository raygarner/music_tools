DEST = /usr/bin
SRC = md.c crd.c cf.c fb.c int.c ks.c mld.c hrm.c mxml.c
EXES = $(SRC:.c=)
COMMON = common
MYCFLAGS = -std=c99 -Wall -pedantic

all: $(EXES)

full: clean all 
	sudo make install

options:
	@echo "DEST     = $(DEST)"
	@echo "SRC      = $(SRC)"
	@echo "EXES     = $(EXES)"
	@echo "MYCFLAGS = $(MYCFLAGS)"
	@echo "CC       = $(CC)"

clean:
	rm -rf $(EXES) *.musicxml music_tools.tar.gz music_tools.zip dist

install: all
	cp -f $(EXES) $(DEST)

uninstall:
	@$(foreach EXE,$(EXES), rm -f $(DEST)/$(EXE))

dist:
	mkdir -p dist
	cp -R README TODO Makefile $(COMMON).* $(SRC) dist

tar: dist
	tar -cf - dist | gzip > music_tools.tar.gz

zip: dist
	zip -r music_tools.zip dist

md:
	$(CC) $(MYCFLAGS) $@.c $(COMMON).c -o $@

crd:
	$(CC) $(MYCFLAGS) $@.c $(COMMON).c -o $@

cf:
	$(CC) $(MYCFLAGS) $@.c $(COMMON).c -o $@

fb:
	$(CC) $(MYCFLAGS) $@.c $(COMMON).c -o $@

int:
	$(CC) $(MYCFLAGS) $@.c $(COMMON).c -o $@

ks:
	$(CC) $(MYCFLAGS) $@.c $(COMMON).c -o $@

mld:
	$(CC) $(MYCFLAGS) $@.c $(COMMON).c -o $@

hrm:
	$(CC) $(MYCFLAGS) $@.c $(COMMON).c -o $@

mxml:
	$(CC) $(MYCFLAGS) $@.c $(COMMON).c -o $@
	
.PHONY: all full options clean install uninstall tar zip
