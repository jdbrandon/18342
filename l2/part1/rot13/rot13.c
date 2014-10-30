/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Jeff Brandon jdbrando@andrew.cmu.edu
 * Date: Sun Oct 12 19:45:22 UTC 2014
 */

#include <bits/errno.h>
#include <bits/fileno.h>
#include <bits/types.h>
#include <stdlib.h>
#include <unistd.h>

size_t strlen(char*);
inline void chkres(ssize_t);
int errno;

/* Main
	If provided performs rot13 on cmd line args, then
	reads from stdin, performing rot13 on all data read.
	Writes result of rot13 to stdout.
	If no bytes are read the program exits with status 0.
	If an error occurs during a read or write call the program 
	exits with status errno.
	exit never returns but in order to compile successfully
	the return 1; statement was left at the end of main.
*/
int main(int argc, char** argv) {
	void rot13(char*, size_t);
	ssize_t res;
	char buf[512];
	int i = 1;
	while(i < argc){
		rot13(argv[i], strlen(argv[i]));
		res = write(STDOUT_FILENO, argv[i], strlen(argv[i]));
		chkres(res);
		res = write(STDOUT_FILENO, " ", 2);
		chkres(res);
		i++;
	}
	res = write(STDOUT_FILENO, "\n", 2);
	chkres(res);
	while((res = read(STDIN_FILENO, buf, 512)) > 0){
		if(buf[0] == '\n')
			exit(0); /* no input -> exit successfully */
		chkres(res);
		rot13(buf, (size_t) res);
		res = write(STDOUT_FILENO, buf, res);
		chkres(res);
	}
	chkres(res);
	exit(0); /* exit successfully */
	return 1; /* exit didn't work */
}

/* Given a buffer and its length, performs a rotation
   of 13 on all letters in the buffer.
	buf - the buffer
	size - the length of the buffer in bytes
	returns: void, buf contains rotated version of
		its former value on completion.
*/
void rot13(char* buf, size_t size){
	int i;
	char c;
	for(i=0; i<size; i++){
		c = *(buf+i);	/* store character in local variable to reduce memory access */
		if((c >= 'A' && c < ('A' + 13)) || 
		   (c >= 'a' && c < ('a' + 13)))
			c += 13; /* rotate up */
		else if((c >= ('A' + 13) && c <= 'Z') ||
			(c >= ('a' + 13) && c <= 'z'))
			c -= 13; /* rotate down */
		*(buf+i) = c;	/* write c back to buffer */
	}
}

/* strlen iterates through the string s until a null byte is found
   to count the number of bytes/characters that make up the string.
	s - the string to calculate the length of.
   	returns: The number of characters that make up s not including
		the terminating null byte.
*/
size_t strlen(char* s){
	int c = 0;
	while(*s++){
		c++;
	}
	return c;
}

/* Helper function checks the result of a read or write syscall.
   If an error occured, it calls exit(errno).
	res - result to test
*/
inline void chkres(ssize_t res){
	if(res < 0){
		exit(errno);
	}
}
