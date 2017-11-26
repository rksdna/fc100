# Project files

SRC = $(wildcard *_bench.v)
OBJ = $(SRC:.v=.o)
VCD = $(SRC:.v=.vcd)

# Tools and flags

CC = iverilog -Wall
VM = vvp
RM = rm

.PHONY: all clean install

all: $(OBJ)
	@echo "Execute $<..."
	$(VM) $<

%.o: %.v
	@ echo "Compiling $<..."
	$(CC) -o $@ $<

clean:
	@echo "Cleaning..."
	$(RM) -f $(OBJ) $(VCD)

