/** @file splat.c
 *
 * @Displays a spinning cursor forever.
 *
 * Links to libc.
 * @Author Keane Lucal <kjlucas@andrew.cmu.edu>
 * @Author Jeff Brandon <jdbrando@andrew.cmu.edu>
 */

#include <unistd.h>

int main(int argc, char** argv)
{
	while(1){
		write(STDOUT_FILENO, "|", 1);
		sleep(200);
		write(STDOUT_FILENO, "\b \b", 3);
		write(STDOUT_FILENO, "/", 1);
		sleep(200);
		write(STDOUT_FILENO, "\b \b", 3);
		write(STDOUT_FILENO, "-", 1);
		sleep(200);
		write(STDOUT_FILENO, "\b \b", 3);
		write(STDOUT_FILENO, "\\", 1);
		sleep(200);
		write(STDOUT_FILENO, "\b \b", 3);
	}
	return 0;
}
