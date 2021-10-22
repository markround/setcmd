CC=vc
CFLAGS=-lamiga -lauto +aos68k -I/opt/sdk/NDK_3.9/Include/include_h -Iinclude
OBJS=main.o utility.o

all: setcmd test stub

setcmd: $(OBJS)
	$(CC) -o setcmd $(OBJS) $(CFLAGS)

stub: stub.o utility.o
	$(CC) $(CFLAGS) -o stub stub.o utility.o -Iinclude
	
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