/** @file ub_test.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Jeff Brandon <jdbrando@andrew.cmu.edu>
 * @author Keane Lucas <kjlucas@andrew.cmu.edu>
 *
 * @date 2014-12-02
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

extern tcb_t system_tcb[OS_MAX_TASKS];
void sorttasks(task_t*, size_t);
void swap(task_t*, task_t*);

/**
 * @brief Method to determine schedulability and sorts tasks by period
 *
 * @param tasks  List of task structs
 * @param count  Size of list

 * @return 1 if schedulable, 0 if not
 */

int assign_schedule(task_t** tasks, size_t count){
	
}

/**
 * @brief Method to sort tasks by their frequency T
 *
 * @param tasks  List of task structs
 * @param count  Size of list
 */

void sorttasks(task_t* tasks, size_t count){
	size_t i, j, min = 0, minpos = 0;
	for(i = 0; i < (count-1); i++){
		min = tasks[i].T;
		minpos = i;
		for(j = i+1; j < count; j++){
			if(tasks[j].T < min){
				min = tasks[j].T;
				minpos = j;
			}
		}
		if(i != minpos)
			swap(&tasks[i], &tasks[minpos]);
	}
}

/**
 * @brief Method used to swap two tasks
 *
 * param a  Task 1
 * param b  Task 2
 */
void swap(task_t* a, task_t* b){
	task_t tmp;
	tmp.lambda = a->lambda;
	tmp.data = a->data;
	tmp.stack_pos = a->stack_pos;
	tmp.C = a->C;
	tmp.T = a->T;
	
	a->lambda = b->lambda;
	a->data = b->data;
	a->stack_pos = b->stack_pos;
	a->C = b->C;
	a->T = b->T;

	b->lambda = tmp.lambda;
	b->data = tmp.data;
	b->stack_pos = tmp.stack_pos;
	b->C = tmp.C;
	b->T = tmp.T;
}
