# Make file for pointers assignment

OBJS	= main.o matryoshka.o sweepminer.o
CC		= g++
CCFLAGS	= -Wall -Wextra -pedantic -std=c++11 -g

all: recursion 

recursion: $(OBJS)
	@echo "Compiling recursion program"
	$(CC) $(CCFLAGS) -o recursion $(OBJS)
	@echo "Done! Run with ./recursion"
	
main.o:
	@echo "Compiling main.cpp"
	$(CC) $(CCFLAGS) -c main.cpp

matryoshka.o:
	@echo "Compiling matryoshka.cpp"
	$(CC) $(CCFLAGS) -c matryoshka.cpp
	
sweepminer.o:
	@echo "Compiling sweepminer.cpp"
	$(CC) $(CCFLAGS) -c sweepminer.cpp

# -----
# clean by removing object files.

clean:
	rm $(OBJS)
	rm recursion
