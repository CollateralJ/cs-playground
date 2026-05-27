# CS 218 Assignment #11
# Simple make file for asst #11

OBJS	= main.o fileIO.o
ASM	= yasm -g dwarf2 -f elf64
CC	= g++ -g -std=c++11 -z noexecstack

all: main

main.o: main.cpp
	$(CC) -c main.cpp

fileIO.o: fileIO.asm
	$(ASM) fileIO.asm -l fileIO.lst

main: $(OBJS)
	$(CC) -no-pie -o main $(OBJS)

# -----
# clean by removing object files.

clean:
	rm  $(OBJS)
	rm  fileIO.lst

