/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
 */

#include <unistd.h>

int main(int argc, char** argv)
{
	while(1){
		write(STDOUT_FILENO, "|", 1);
		sleep(400);
		write(STDOUT_FILENO, "\b \b", 3);
		write(STDOUT_FILENO, "/", 1);
		sleep(400);
		write(STDOUT_FILENO, "\b \b", 3);
		write(STDOUT_FILENO, "-", 1);
		sleep(400);
		write(STDOUT_FILENO, "\b \b", 3);
		write(STDOUT_FILENO, "\\", 1);
		sleep(400);
		write(STDOUT_FILENO, "\b \b", 3);
	}
	return 0;
}
