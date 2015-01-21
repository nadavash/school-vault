# Copyright 2015 Nadav Ashkenazi
# nadava@uw.edu - 1230523
# CSE 333 15wi
# Exercise 5 - Makefile
CC = gcc

CFLAGS = -Wall -std=c11 -g

all: ex5

ex5: ex5.o vector.o vector.h FORCE
	$(CC) $(CFLAGS) -o ex5 ex5.o vector.o

%.o: %.c vector.h FORCE
	$(CC) $(CFLAGS) -c $<

clean: FORCE
	rm -f *.o *~ ex5

FORCE: