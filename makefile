CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

clean:
	rm *.o test1 test2 test3 test4

all: hash.o

hash.o: hash.c hash.h
	$(CC) $(CFLAGS) -c hash.c -o hash.o

test1.o: htest1.c hash.h
	$(CC) $(CFLAGS) -c htest1.c -o test1.o

test1: test1.o hash.o
	$(CC) $(CFLAGS) -o test1 test1.o hash.o

test2.o: htest2.c hash.h
	$(CC) $(CFLAGS) -c htest2.c -o test2.o

test2: test2.o hash.o
	$(CC) $(CFLAGS) -o test2 test2.o hash.o

test3.o: htest3.c hash.h
	$(CC) $(CFLAGS) -c htest3.c -o test3.o

test3: test3.o hash.o
	$(CC) $(CFLAGS) -o test3 test3.o hash.o

test4.o: htest4.c hash.h
	$(CC) $(CFLAGS) -c htest4.c -o test4.o

test4: test4.o hash.o
	$(CC) $(CFLAGS) -o test4 test4.o hash.o