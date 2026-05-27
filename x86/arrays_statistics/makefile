OBJS	= statistics.o
ASM		= yasm -g dwarf2 -f elf64
LD		= ld -g

all: statistics

statistics.o: statistics.asm 
	$(ASM) statistics.asm -l statistics.lst

statistics: statistics.o
	$(LD) -o statistics $(OBJS)

# -----
# clean by removing object file.

clean:
	rm	$(OBJS)
