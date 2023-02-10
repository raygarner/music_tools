.POSIX:

DEST = /usr/bin
EXES = md crd

all: $(EXES)

options:
	@echo "CFLAGS = $(CFLAGS)"
	@echo "CC     = $(CC)"

clean:
	rm -f $(EXES)

install: all
	cp -f $(EXES) $(DEST)

uninstall:
	@$(foreach EXE,$(EXES), rm -f $(DEST)/$(EXE))

md:
	$(CC) $(CFLAGS) $@.c -o $@

crd:
	$(CC) $(CFLAGS) $@.c -o $@

