/** @file dagger.c
 *
 * @brief Creates two simple periodic tasks.
 *
 * @note  This is like dagger -- but smaller :)
 *
 */
#include <stdio.h>
#include <task.h>
#include <unistd.h>

struct obj{
	int m;
	char c;
};
typedef struct obj obj;


void panic(const char* str)
{
	puts(str);
	while(1);
}

void fun1(void* args)
{
	obj* tmp = (obj*) args;
	while(1)
	{
		mutex_lock(tmp->m);
		putchar(tmp->c);
		sleep(200);
		mutex_unlock(tmp->m);
		if (event_wait(0) < 0)
			panic("Dev 0 failed");
	}
}

void fun2(void* args)
{
	obj* tmp = (obj*) args;
	while(1)
	{	
		mutex_lock(tmp->m);
		putchar(tmp->c);
		sleep(1000);
		mutex_unlock(tmp->m);
		if (event_wait(1) < 0)
			panic("Dev 1 failed");
	}
}

int main(int argc, char** argv)
{
	obj tmp, tmp2;
	tmp.m = mutex_create();
	tmp2.m = tmp.m;
	tmp.c = '@';
	tmp2.c = '<';
	task_t tasks[2];
	tasks[0].lambda = fun1;
	tasks[0].data = (void*)&tmp;
	tasks[0].stack_pos = (void*)0xa2000000;
	tasks[0].C = 1;
	tasks[0].T = PERIOD_DEV0;
	tasks[1].lambda = fun2;
	tasks[1].data = (void*)&tmp2;
	tasks[1].stack_pos = (void*)0xa1000000;
	tasks[1].C = 1;
	tasks[1].T = PERIOD_DEV1;

	puts("launching create tasks..\n");
	task_create(tasks, 2);
	argc=argc; /* remove compiler warning */
	argv[0]=argv[0]; /* remove compiler warning */

	puts("Why did YOUR code get here!\n");
	return 0;
}
