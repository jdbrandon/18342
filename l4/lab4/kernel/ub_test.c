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
 * @brief Method to determine schedulability (using UB test) and sorts tasks by period
 *
 * @param tasks  List of task structs
 * @param count  Size of list

 * @return 1 if schedulable, 0 if not
 */

int assign_schedule(task_t** tasks, size_t count){
	sorttasks(*tasks, count);
	size_t i;
	size_t j;
	double util;
	double kroottwo[64] = {2.0, 1.4142135623730951, 1.2599210498948732, 1.189207115002721, 1.148698354997035, 1.122462048309373, 1.1040895136738123, 1.0905077326652577, 1.080059738892306, 1.0717734625362931, 1.0650410894399627, 1.0594630943592953, 1.0547660764816467, 1.0507566386532194, 1.0472941228206267, 1.0442737824274138, 1.0416160106505838, 1.0392592260318434, 1.0371550444461919, 1.0352649238413776, 1.0335577830070277, 1.0320082797342096, 1.0305955447520094, 1.029302236643492, 1.0281138266560665, 1.0270180507087725, 1.0260044847070386, 1.0250642119658746, 1.0241895602489972, 1.023373891996775, 1.0226114356012683, 1.0218971486541166, 1.021226606315364, 1.0205959095795867, 1.020001609421199, 1.0194406437021448, 1.0189102844052507, 1.018408093274102, 1.0179318843373906, 1.0174796921026863, 1.0170497444436268, 1.0166404393919355, 1.016250325193552, 1.0158780831055514, 1.015522512504275, 1.0151825179503484, 1.0148570979170202, 1.0145453349375237, 1.0142463869673273, 1.013959479790029, 1.0136839003226856, 1.0134189906987003, 1.0131641430249148, 1.0129187947249467, 1.0126824243937012, 1.012454548098765, 1.0122347160734744, 1.012022509754105, 1.0118175391201212, 1.0116194403019225, 1.0114278734252204, 1.0112425206651807, 1.0110630844868955, 1.0108892860517005};
	for(i=0; i<count; i++){
		for(j=0; j<i; j++){
			util += (double)tasks[j]->C/(double)tasks[j]->T;
		}
		util += ((double)tasks[i]->C + (double)/tasks[i]->B)/(double)tasks[i]->T;
		if(util > ((double)i+1.0)*((double)kroottwo[i]-1.0)){
			return 0;
		}
	}
	return 1;
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
