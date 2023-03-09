# Copyright 2021 Nastase Maria

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=Galactic_War

build: $(TARGETS)

Galactic_War: Galactic_War.c
	$(CC) $(CFLAGS) Galactic_War.c -o main

clean:
	rm -f $(TARGETS) main