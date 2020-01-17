CC=gcc

sdktest: sdktest.o utility.o
	$(CC) -o sdktest main.o utility.o -Iinclude -lauto

.c.o:
	$(CC) -c -g $< -Iinclude -lauto

clean:
	rm *.o test
