CC=gcc
CFLAGS=
OBJS=main.o utility.o usage.o init.o list.o show.o add_cmd.o delete_cmd.o add_version.o delete_version.o set_version.o


all: setcmd stub

setcmd: $(OBJS)
	$(CC) $(CFLAGS) -o setcmd $(OBJS) -Iinclude -lauto
	copy setcmd /

stub: stub.o utility.o
	$(CC) $(CFLAGS) -o stub stub.o utility.o -Iinclude
	copy setcmd /

.c.o:
	$(CC) $(CFLAGS) -c -g $< -Iinclude

clean:
	-rm *.o setcmd stub

debug: CFLAGS=-DDEBUG
debug: setcmd

