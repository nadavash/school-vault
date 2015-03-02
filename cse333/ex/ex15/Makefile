# Copyright 2015 Nadav Ashkenazi
# nadava@uw.edu - 1230523
# CSE 333 15wi
# Exercise 11 - Makefile
CPP = g++

CPPFLAGS = -Wall -std=c++11 -g

all: ex15

ex15: ex15.o
	$(CPP) $(CPPFLAGS) -o ex15 ex15.o

%.o: %.cc
	$(CPP) $(CPPFLAGS) -c $<

clean: FORCE
	rm -f *.o *~ ex15

FORCE:
