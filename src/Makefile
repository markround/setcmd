CC=gcc
CFLAGS=


setcmd: main.o utility.o usage.o init.o list.o show.o
	$(CC) $(CFLAGS) -o setcmd main.o utility.o usage.o init.o list.o show.o -Iinclude -lauto

stub: stub.o utility.o
	$(CC) $(CFLAGS) -o stub stub.o utility.o -Iinclude

sdktest: sdktest.o utility.o
	$(CC) $(CFLAGS) -o sdktest sdktest.o utility.o -Iinclude

.c.o:
	$(CC) $(CFLAGS) -c -g $< -Iinclude

clean:
	-rm *.o sdktest setcmd stub

debug: CFLAGS=-DDEBUG
debug: main.o utility.o usage.o init.o list.o show.o
	$(CC) $(CFLAGS) -o setcmd main.o utility.o usage.o init.o list.o show.o -Iinclude -lauto

all: setcmd stub
