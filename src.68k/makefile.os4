CC=vc
CFLAGS=-lamiga -lauto +aos68k -INDK:include_h -Iinclude -c99
OBJS=main.o utility.o

all: setcmd test stub

setcmd: $(OBJS)
	$(CC) -o setcmd $(OBJS) $(CFLAGS)

stub: stub.o utility.o
	$(CC) -o stub stub.o utility.o $(CFLAGS)

test: test.o utility.o
	$(CC) -o test test.o utility.o $(CFLAGS)

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

clean:
	rm *.o test setcmd

debug: CFLAGS += -DDEBUG
debug: test
debug: setcmd
debug: stub
