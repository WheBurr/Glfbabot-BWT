#ifndef ITERATE_H_
#define ITERATE_H_

#include "backward_search.h"
#include "bit_vector.h"

typedef struct substring_struct
{
	Interval* normal;
	Interval* reverse;
	int length;
} substring;

/**
* @brief creates vector runs from given string
*
* @param string normal string (rbwt is generated from it)
*
* @return pointer to runs vector
*
* @author Topi Paavilainen
*
* @bug no known bugs
**/
bit_vector* create_runs_vector(char* string);

/**
* @brief function that determines if given interval in given reverse-bwt is right-maximal or not.
*
* @param runs bit_vector runs
*
* @param interval interval to be searched
*
* @return 1 if given interval is right-maximal, 0 if not. If interval is not correct function returns 0
*
* @author Topi Paavilainen
*
* @bug no known bugs
**/
int is_reverse_interval_right_maximal(bit_vector* runs, Interval* interval);

#endif