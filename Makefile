CFLAGS = -g -Wall

all: needed mkprog

needed: needed.o
	$(CC) $(CFLAGS) -o needed needed.o

mkprog: mkprog.o
	$(CC) $(CFLAGS) -o mkprog mkprog.o

clean:
	rm -f *.o needed mkprog
