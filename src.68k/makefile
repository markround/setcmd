CC=vc
CFLAGS=-lamiga -lauto +aos68k -ISoftware:NDK_3.9/Include/include_h -Iinclude -c99

all: test

test: test.o utility.o
	$(CC) -o test test.o utility.o $(CFLAGS)

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

clean:
	rm *.o test

debug: CFLAGS += -DDEBUG
debug: test