#define TIMER_BASE_ADDR 0x40a00000
#define OSCR (TIMER_BASE_ADDR + 0x10) /* OS Timer counter register */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>

typedef volatile unsigned * mmio_t;

int main(int argc, char** argv){
	mmio_t oscr = (mmio_t) OSCR;
	*oscr = 0xffffff00;
	while(1){
		printf("time:%ld\n", time());
		sleep(200);
	}
}
