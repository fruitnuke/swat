PROGRAM = count-instructions
OBJECTS =
CFLAGS = -g -Wall -O3 --std=c11
LDLIBS =
CC = gcc

$(PROGRAM): $(OBJECTS)

samples: sample-01 sample-02

sample-01:

sample-02: sample-02.o
	ld sample-02.o -o sample-02

sample-02.o: sample-02.asm
	nasm -f elf64 sample-02.asm

clean:
	rm -rf *.o sample-01 sample-02 $(PROGRAM)

