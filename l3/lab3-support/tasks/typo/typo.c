/** @file typo.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
 */

#include <unistd.h>

int main(int argc, char** argv)
{
	char rawinput[100];
	unsigned starttime = 0;
	unsigned length = 0;
	while(1){
		write(STDOUT_FILENO, ">>>", 3);
		starttime = time();
		length = read(STDIN_FILENO, rawinput, 100);
		write(STDOUT_FILENO, rawinput, length);
		write(STDOUT_FILENO, ((time() - starttime)/1000), 3);
		write(STDOUT_FILENO, "s", 3);
	}
	return 0
}
