CC = gcc
CFLAGS = Wall

all: parent.o child.o
	$(CC) -$(CFLAGS) parent.o child.o -o all

parent.o: parent.c
	$(CC) -$(CFLAGS) -c parent.c

child.o: child.c
	$(CC) -$(CFLAGS) -c child.c

clean:
	rm *.o all