CC = gcc
CFLAGS = Wall

CFILES = parent.c child.c
objects = parent child

all: $(objects)

parent : parent.c
	$(CC) -$(CFLAGS) $^ -o $@

child : child.c
	$(CC) -$(CFLAGS) $^ -o $@

clean:
	rm -f $(objects)