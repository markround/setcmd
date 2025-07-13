#include <proto/dos.h>
#include <dos/dos.h>
#include <stdio.h>
#include "utility.h"

int main(int argc, char const *argv[])
{
  

  printf("Testing...\n");
	BOOL test;
	test = path_is_directory("Work:Projects");
	if (test) {
		printf("Is a directory\n");
	} else {
		printf("Is not a directory\n");
	}

	test = path_is_directory("Work:Disk.info");
	if (test) {
		printf("Is a directory\n");
	} else {
		printf("Is not a directory\n");
	}
}
