OBJS		= mazesearch.o maze.o
ASM		= yasm -g dwarf2 -f elf64
LD		= ld -g

all: mazesearch

maze.o: maze.asm
	$(ASM) maze.asm -l maze.lst

mazesearch.o: mazesearch.asm 
	$(ASM) mazesearch.asm -l mazesearch.lst

mazesearch: mazesearch.o maze.o
	$(LD) -o mazesearch $(OBJS)

# -----
# clean by removing object file.

clean:
	rm	$(OBJS)
