CC=gcc

sdktest: sdktest.o utility.o
	$(CC) -o sdktest sdktest.o utility.o -Iinclude

.c.o:
	$(CC) -c -g $< -Iinclude

clean:
	rm *.o sdktest
