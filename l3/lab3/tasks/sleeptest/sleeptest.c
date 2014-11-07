/* sleeptest.c

   Description - test the sleep function by calling edge case values.

   Author: Jeff Brandon <jdbrando@andrew.cmu.edu>
   Author: Keane Lucas  <kjlucas@anrew.cmu.edu>

   Note: the final call to sleep where -1 is passed as an argument will
      cause the program to sleep for a long time and it will also overflow
      the sleep value.

   Date: Thu Nov  6 15:30:44 EST 2014
*/
#include <stdio.h>
#include <unistd.h>
int main(int argc, char** argv){
	int i;
	if(argc > 1)
		printf("argv[1]: %s\n", argv[1]);
	for(i = 0; i < 10; i++){
		printf("sleep %d\n", i);
		sleep(i);
	}
	printf("sleep %d\n", -1);
	sleep(-1);
	return 0;
}
