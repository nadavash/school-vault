# Copyright 2015 Nadav Ashkenazi
# nadava@uw.edu - 1230523
# CSE 333 15wi
# Exercise 5 - Makefile
CPP = g++

CPPFLAGS = -Wall -std=c++11 -g

all: ex10

ex10: ex10.o Vector.o Vector.h
	$(CPP) $(CPPFLAGS) -o ex10 ex10.o Vector.o

%.o: %.cc Vector.h
	$(CPP) $(CPPFLAGS) -c $<

clean: FORCE
	rm -f *.o *~ ex10

FORCE:
