CC=gcc
# warnings all and debugging
CFLAGS=-c -Wall -g
CURRENT_DIR := $(shell basename `pwd`)
THIS_FILE := $(lastword $(MAKEFILE_LIST))

all: hw3

hw3: shell.o hw3.o
	$(CC) -g shell.o hw3.o -o hw3

shell.o: shell.c
	$(CC) $(CFLAGS) shell.c

hw3.o:	hw3.c
	$(CC) $(CFLAGS) hw3.c

clean:
	/bin/rm -f hw3 *.o *~

run:
	./hw3

tarball:
	@$(MAKE) -f $(THIS_FILE) clean
	tar -cvzf ../$(CURRENT_DIR).tgz -C.. $(CURRENT_DIR)
