CFLAGS=-Wall
OBJECTS1=example1.o ../src/argparse.o

all: example1

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

example1: $(OBJECTS1)
	gcc -o $@ $(OBJECTS1)
