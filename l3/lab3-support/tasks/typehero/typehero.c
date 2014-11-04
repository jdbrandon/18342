#define TIMER_BASE_ADDR 0x40a00000
#define OSCR (TIMER_BASE_ADDR + 0x10) /* OS Timer counter register */
#define STEP 1000

#include <unistd.h>
#include <stdio.h>

typedef volatile unsigned * mmio_t;

int main(int argc, char** argv){
	mmio_t oscr = (mmio_t) OSCR;
	char check[2];
	printf("Ready your fingers on home keys (asdf jkl;)\n");
	sleep(1000);
	printf("Be ready to type what you see...\n If you get the type the shown letter within a second, you get a point...\n If you are not fast enough YOU LOSE POINTS!\n  The slower you are, the more points you lose...\n");
	sleep(3000);
	printf("Press any key when ready...\n");
	read(STDIN_FILENO, &check, 1); 
	printf("\b \b");
	printf("Ready...\n");
	sleep(2000);
	printf("GO!\n");
	unsigned random = 0;
	unsigned start_time = 0;
	int diff = 0;
	int score = 0;
	int hit = 0;
	while(1){
		random = *oscr % 8;
		start_time = time();
		if(random == 0){
			printf("a--- ----");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='a'){
				hit = 1;
			}
		}else if(random == 1){
			printf("-s-- ----");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='s'){
				hit = 1;
			}
		}else if(random == 2){
			printf("--d- ----");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='d'){
				hit = 1;
			}
		}else if(random == 3){
			printf("---f ----");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='f'){
				hit = 1;
			}
		}else if(random == 4){
			printf("---- j---");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='j'){
				hit = 1;
			}
		}else if(random == 5){
			printf("---- -k--");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='k'){
				hit = 1;
			}
		}else if(random == 6){
			printf("---- --l-");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='l'){
				hit = 1;
			}
		}else if(random == 7){
			printf("---- ---;");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]==';'){
				hit = 1;
			}
		}
		if(diff < STEP && hit){
			printf("   HIT!    Score: %d +1\n", score);
			score++;
		}else{
			score -= (diff/STEP + 1);
			printf("   miss... Score: %d -%d\n", score, diff/STEP+1);
		}
		hit = 0;
	}
}
