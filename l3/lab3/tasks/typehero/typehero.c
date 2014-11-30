/* typehero.c

   Description: A game to test functionality of our kernel.

   Author: Keane Lucas  <kjlucas@andrew.cmu.edu>
   Author: Jeff Brandon <jdbrando@andrew.cmu.edu>

   Date Thu Nov  6 15:33:58 EST 2014
*/
#define TIMER_BASE_ADDR 0x40a00000
#define OSCR (TIMER_BASE_ADDR + 0x10) /* OS Timer counter register */

#include <unistd.h>
#include <stdio.h>
#include <bits/swi.h>

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
	unsigned total_steps = 0;
	unsigned start_time = 0;
	int step = 1000;
	int diff = 0;
	int score = 0;
	int hit = 0;
	int highscore = 0;
	int streak = 0;
	while(1){
		random = *oscr % 8;
		start_time = time();
		if(random == 0){
			printf("a--- ----");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='a' || check[0]=='A'){
				hit = 1;
			}
		}else if(random == 1){
			printf("-s-- ----");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='s' || check[0]=='S'){
				hit = 1;
			}
		}else if(random == 2){
			printf("--d- ----");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='d' || check[0]=='D'){
				hit = 1;
			}
		}else if(random == 3){
			printf("---f ----");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='f' || check[0]=='F'){
				hit = 1;
			}
		}else if(random == 4){
			printf("---- j---");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='j' || check[0]=='J'){
				hit = 1;
			}
		}else if(random == 5){
			printf("---- -k--");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='k' || check[0]=='K'){
				hit = 1;
			}
		}else if(random == 6){
			printf("---- --l-");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]=='l' || check[0]=='L'){
				hit = 1;
			}
		}else if(random == 7){
			printf("---- ---;");
			read(STDIN_FILENO, &check, 1);
			printf("\b \b");
			diff = time() - start_time;
			if(check[0]==';' || check[0]==':'){
				hit = 1;
			}
		}
		if(diff < step && hit){
			printf("   HIT!    Score: %d +1\n", score);
			score++;
			streak++;
			if(score > highscore){
				highscore = score;
			}
			if(streak > 9){
				printf("WHOA! 10 HIT streak! +5 Points! Making this harder... better get faster...\n");
				streak = 0;
				score += 5;
				step -= 100;
			}
		}else{
			score -= (diff/step + 1);
			printf("   miss... Score: %d -%d\n", score, diff/step+1);
			streak = 0;
			if(score < -49){
				printf("Nice try... Better luck next time. Highest Score: %d Steps Survived: %d steps\n",highscore, (int)total_steps);
				break;
			}
		}
		total_steps++;
		hit = 0;
	}
	return 0;
}
