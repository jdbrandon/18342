/** @file typo.c
 *
 * Prints the time it takes for a user to enter some text
 *
 * @Author Keane Lucas  <kjlucas@andrew.cmu.edu>
 * @Author Jeff Brandon <jdbrando@andrew.cmu.edu>
 * @Date Thu Nov  6 15:33:08 EST 2014
 */

#include <unistd.h>
#include <string.h>

void convert(unsigned, char*);

/* Prompt user for unput. Then get starting time. When read completes,
   calculate time to enter the input using ending time. Present user 
   with timing info after echoing the input. 
*/
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

/* Converts and unsigned number into a string representation.
   if the input number is xyzw the returned string will be 
   xyz.w 
   @param t - the unsigned number to convert to a string
   @param out - pointer to the string result on completion.
*/
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
