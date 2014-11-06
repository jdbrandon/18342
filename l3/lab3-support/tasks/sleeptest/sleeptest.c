#include <stdio.h>
#include <unistd.h>
int main(int argc, char** argv){
	int i;
	for(i = 0; i < 10; i++){
		printf("sleep %d\n", i);
		sleep(i);
	}
	printf("sleep %d\n", -1);
	sleep(-1);
	return 0;
}
