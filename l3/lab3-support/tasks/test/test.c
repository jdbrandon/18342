#define TIMER_BASE_ADDR 0x40a00000
#define OSCR (TIMER_BASE_ADDR + 0x10) /* OS Timer counter register */
#define OSMR_1 (TIMER_BASE_ADDR + 0x4)

#include <unistd.h>
#include <stdio.h>

typedef volatile unsigned * mmio_t;

int main(int argc, char** argv){
	mmio_t oscr = (mmio_t) OSCR;
	mmio_t osmr = (mmio_t) OSMR_1;
	*osmr = 18432000;
	*oscr = 0x0;
	while(1){
		printf("time:%ld, \n", time());
//		sleep(200);
	}
}
