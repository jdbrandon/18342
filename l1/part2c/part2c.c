/* part2c.c
   Jeff Brandon
   jdbrando@andrew.cmu.edu
   9.20.2014
   18-342: Fundamentals of Embedded Systems
*/
#include <stdlib.h>
#include <stdio.h>

/**
 * Function: oddball
 * Description: Baseline implementation for finding the number that
 *   occurs only once in the array.
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   The number that occurs only once
 */
#ifdef DEFAULT
int oddball(int *arr, int len) {
	int i, j;
	int foundInner;
	int result = 0;

	for (i = 0; i < len; i++) {
		foundInner = 0;
		for (j = 0; j < len; j++) {
			if (i == j) {
				continue;
			}
			if (arr[i] == arr[j]) {
				foundInner = 1;
			}
		}
		if (foundInner != 1) {
			result = arr[i];
		}
	}

	return result;
}
#endif

#ifdef OPTIMIZE1
/**
 * Function: oddball
 * Description: Greedy implementation of oddball. Allocate memory for 
 * n integers in int array (data), traverse arr and keep count of how 
 * many times a number has occured at data[<number>]. After arr has 
 * been traversed find the index in data where data[index] == 1 and 
 * the oddball has been found. Executes in O(n) time because it traverses
 * arr once, and for data n/2 elements on average are traversed. This 
 * implementation is better than the default because the default runs
 * in O(n^2) time.
 * It's biggest drawback is that it is very memory intensive.
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   The number that occurs only once
 */
int oddball(int *arr, int len) {
	int num, i, n, *data;
	n = (len+1)>>1;
	data = calloc(n, sizeof(int));
	for(i=0; i<len; i++)
		data[arr[i]-1]++;
        for(num=0; data[num] != 1; num++);
        free(data);
	return num + 1;
}
#endif

#ifdef OPTIMIZE2
/**
 * Function: oddball
 * Description: Oddball implementation 2 takes advantage of quicksort,
 * which operates in O(n log n) time to simplify the problem. Once arr
 * is sorted, traverse arr with stride 2. When an element is found whos
 * next adjacent element is not equal to that element, the oddball has 
 * been found. This search operates in O(n/2) time on average and is at
 * worst O(n). This solution operates in O(n log n) time, which is better
 * than the default implementation that operates in O(n^2) time.
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   The number that occurs only once
 */
int oddball2(int *arr, int len) {
	void quicksort(int*, int, int);
	int i;
	quicksort(arr, 0, len-1);
	for(i = 0; i<len; i+=2)
		if(arr[i]!=arr[i+1]) 
			break;
	return (i>>1)+1;
}

/* standard quicksort implementation based off
   of pseudocode found at http://en.wikipedia.org/wiki/Quicksort
   Arguments:
     arr - pointer to start of integer array
     base - left bound on the sub-array to be sorted
     limit - right bound on the sub-array to be sorted
*/
void quicksort(int *arr, int base, int limit){
	int part(int*, int, int);
	if(base >= limit) 
		return;
	int p = part(arr, base, limit);
        quicksort(arr, base, p-1);
        quicksort(arr, p+1, limit);
}

/* Partition function used for quicksort, this simple
   implementation chooses arr[right] as the pivot.
   Also based off of pseudocode on Wikipedia (link above)
   Arguments:
      arr - pointer to the start of an array
      left - left bound on sub-array being sorted
      right - right bound on sub-array being sorted
   Returns: The index of the pivot
*/
int part(int *arr, int left, int right){
	void swap(int*, int*);
	int i, store = left, p = arr[right];
        for(i=left; i<right; i++){
		if(arr[i] < p)
			swap(&arr[i], &arr[store++]);
	}
	swap(&arr[store], &arr[right]);
	return store;
}

/* Simple swap function that swaps the values stored at two pointers
   using a temporary variable.
*/
void swap(int* x, int* y){
	int tmp = *x;
	*x = *y;
	*y = tmp;
}
#endif

/**
 * Function: randGenerator
 * Description: Generate a random array that is in compliance with
 *   lab specification
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   Unused, but feel free to utilize it.
 */

int randGenerator(int *arr, int len) {
	int i, j, r, rcount;
	for (i = 0; i < len; i++) {
		do {
			rcount = 0;
			r = rand()%(len/2 + 1) + 1;
			for (j = 0; j < i && rcount < 2; j++) {
				if (arr[j] == r) {
					rcount++;
				}
			}
		} while(rcount >= 2);
		arr[i] = r;
		printf("%d ", r);
	}
	printf("\nDone generating\n");
	return 0;
}




