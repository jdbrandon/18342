/** @file typo.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
 */

#include <unistd.h>
#include <string.h>

void convert(unsigned, char*);

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
		convert((unsigned)(time()-starttime)/100, rawinput);
		write(STDOUT_FILENO, rawinput, strlen(rawinput));
		write(STDOUT_FILENO, "s\n", 2);
	}
	return 0;
}

void convert(unsigned t, char* out){
	unsigned r, tmp, len;
	int first = 1;
	tmp = t;
	len = 0;
	while(tmp){
		tmp /= 10;
		len++;
	}
	len++;
	out[len--] = 0;
	while(t){
		r = t % 10;
		out[len--] = r + '0';
		if(first){
			first = 0;
			out[len--] = '.';
		}
		t /= 10;
	}
}
