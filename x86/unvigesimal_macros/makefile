# CS 218 Assignment #1
# Simple make file for asst #1

OBJS	= macros.o
ASM		= yasm -g dwarf2 -f elf64
LD		= ld -g

all: macros

macros.o: macros.asm 
	$(ASM) macros.asm -l macros.lst

macros: macros.o 
	$(LD) -o macros $(OBJS)

# -----
# clean by removing object file.

clean:
	rm	$(OBJS)
