/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <const.h>
#include <exports.h>
#include <kernel.h>

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f


/* doread performs the read system call
   Parameters:
	args - arguments passed to the function. args[0] is the file descriptor
		of the file to read from, args[1] is the address of the buffer
		to fill, and args[2] is the number of bytes to read.
	ret - output value set by doread on completion.
   Returns:
	On completion ret will be the number of bytes read if
        read is successful.
        EBADF is returned when the input file descriptor isn't stdin.
        EFAULT is returned when the buffer read into is outside valid bounds.
*/
void doread(unsigned* args, unsigned* ret){
	int fileno, c;
	char* buf;
	size_t count;
	size_t pcount = 0; //processed char count
	fileno = (int) *args++;
	buf = (char*) *args++;
	count = *args;
	if(fileno != STDIN_FILENO){
		*ret = -EBADF;
		return;
	}
	if(!(buf >= (char*)SDRAM_BASE && (buf+count) <= (char*)SDRAM_LIMIT)){
		*ret = -EFAULT;
		return;
	}
	char* bufstart = buf;
	while(pcount++ < count){
		c = getc();
		if(c == EOT){
			*ret = pcount - 1;
			break;
		}
		if(c == BACK_SPACE || c == DELETE){
			if(buf > bufstart)
				buf--;
			if(pcount-- > 1)
				pcount--; //don't count input character and the one it deleted
			/* make terminal display accurate */
			if(buf >= bufstart)
				printf("\b \b");
			continue;
		}
		if(c == NEW_LINE || c == RETURN){
			*buf++ = '\n';
			*buf = 0;
			/* map \r to \n */
			printf("\n");
			*ret = pcount;
			break;
		}
		/* echo input character */
		printf("%c", c);
		*buf++ = c;
	}
}

/* dowrite performs the write system call

   Parameters:
	args - arguments to the function. arg[0] is the file descriptor to write to,
		args[1] is the address of the buffer to write from, and args[2] is 
		the number of bytes that should be written.
	ret - output parameter set on completion of dowrite.
   Returns:
	On completion ret will contain the number of bytes written on success.
	EBADF will be returned if the file descriptor to write to isn't stdout.
	EFAULT will be returned if the buffer isn't within valid bounds.
*/
void dowrite(unsigned* args, unsigned* ret){
	int fileno;
	char* buf;
	size_t count;
	size_t pcount = 0;
	fileno = (int) *args++;
	buf = (char*) *args++;
	count = *args;
	if(fileno != STDOUT_FILENO){
		*ret = -EBADF;
		return;
	}
	if(!((buf >= (char*) SDRAM_BASE && (buf+count) <= (char*) SDRAM_LIMIT) ||
	     (buf >= (char*) SFROM_BASE && (buf+count) <= (char*) SFROM_LIMIT))){
		*ret = -EFAULT;
		return;
	}
	while(*buf){
		printf("%c",*buf++);
		pcount++;
	}
	*ret = pcount;
}

