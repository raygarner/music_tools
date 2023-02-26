.POSIX:

DEST = /usr/bin
SRC = md.c crd.c cf.c fb.c int.c
EXES = $(SRC:.c=)
COMMON = common

all: $(EXES)

options:
	@echo "DEST   = $(DEST)"
	@echo "SRC    = $(SRC)"
	@echo "EXES   = $(EXES)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "CC     = $(CC)"

clean:
	rm -f $(EXES) test music_tools.tar.gz

install: all
	cp -f $(EXES) $(DEST)

uninstall:
	@$(foreach EXE,$(EXES), rm -f $(DEST)/$(EXE))

dist: clean
	mkdir -p music_tools-dist
	cp -R TODO Makefile $(COMMON).* $(SRC) music_tools-dist
	tar -cf - music_tools-dist | gzip > music_tools.tar.gz
	rm -rf music_tools-dist

md:
	$(CC) $(CFLAGS) $@.c $(COMMON).c -o $@

crd:
	$(CC) $(CFLAGS) $@.c $(COMMON).c -o $@

cf:
	$(CC) $(CFLAGS) $@.c $(COMMON).c -o $@

fb:
	$(CC) $(CFLAGS) $@.c $(COMMON).c -o $@

int:
	$(CC) $(CFLAGS) $@.c $(COMMON).c -o $@

test:
	$(CC) $(CFLAGS) $@.c $(COMMON).c -o $@

.PHONY: all options clean install uninstall dist
