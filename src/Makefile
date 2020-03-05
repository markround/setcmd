CC=gcc
CFLAGS=
OBJS=main.o utility.o usage.o init.o list.o show.o add_cmd.o


all: setcmd stub

setcmd: $(OBJS)
	$(CC) $(CFLAGS) -o setcmd $(OBJS) -Iinclude -lauto

stub: stub.o utility.o
	$(CC) $(CFLAGS) -o stub stub.o utility.o -Iinclude

.c.o:
	$(CC) $(CFLAGS) -c -g $< -Iinclude

clean:
	-rm *.o sdktest setcmd stub

debug: CFLAGS=-DDEBUG
debug: setcmd

