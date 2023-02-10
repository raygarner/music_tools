.POSIX:

DEST = /usr/bin
EXES = md crd cf fb
COMMON = common.c

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
	$(CC) $(CFLAGS) $@.c $(COMMON) -o $@

crd:
	$(CC) $(CFLAGS) $@.c $(COMMON) -o $@

cf:
	$(CC) $(CFLAGS) $@.c -o $@

fb:
	$(CC) $(CFLAGS) $@.c $(COMMON) -o $@
