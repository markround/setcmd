CC=gcc

setcmd: main.o utility.o usage.o init.o list.o show.o
	$(CC) -o setcmd main.o utility.o usage.o init.o list.o show.o -Iinclude -lauto

stub: stub.o utility.o
	$(CC) -o stub stub.o utility.o -Iinclude

sdktest: sdktest.o utility.o
	$(CC) -o sdktest sdktest.o utility.o -Iinclude

.c.o:
	$(CC) -c -g $< -Iinclude

clean:
	-rm *.o sdktest setcmd stub

all: setcmd stub
