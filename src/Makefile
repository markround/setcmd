CC=gcc

setcmd: main.o utility.o usage.o
	$(CC) -o setcmd main.o utility.o usage.o -Iinclude -lauto

sdktest: sdktest.o utility.o
	$(CC) -o sdktest sdktest.o utility.o -Iinclude

.c.o:
	$(CC) -c -g $< -Iinclude

clean:
	-rm *.o sdktest setcmd
