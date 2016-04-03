PROGRAM = break
OBJECTS =
CFLAGS = -Wall --std=c11 -g -O3
LDLIBS =
CC = gcc

all: $(PROGRAM) sample

$(PROGRAM): $(OBJECTS)

sample: sample.o
	ld sample.o -o sample

sample.o: sample.asm
	nasm -f elf64 sample.asm

clean:
	rm -rf *.o $(PROGRAM) sample
