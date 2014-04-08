/**
 * @file	iterate.c
 * @brief	Implementation of the core of the library.
 * @author	Lassi Vapaakallio, Topi Paavilainen, Max Sandberg (REXiator),
 * 			Paula Lehtola
 * @bug		No known bugs.
 */

#include "iterate.h"
#include "backward_search.h"
#include "rbwt.h"
#include "c_array.h"
#include "s_to_bwt.h"
#include "substring_stack.h"
#include "../../include/utils.h"
#include "../applications/maximal_repeats.h"
#include "../applications/mum.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define END_STRING '$'

/**
 * @brief 	A function that determines if given interval in given reverse BWT is right maximal.
 * @param 	runs	Runs-vector of a reverse BWT.
 * @param 	inter 	The interval that is to be checked.
 * @return 			<code>1</code> if the interval is right maximal, <code>0</code> otherwise.
 * @author 	Topi Paavilainen, Max Sandberg (REXiator)
 * @bug 	No known bugs.
 */
int is_reverse_interval_right_maximal(bit_vector* runs,
		interval* interval) {
	return runs->rank(runs, (interval->i) + 1, interval->j) > 0 ? 1 : 0;
}

/**
 * @brief	Updates the interval in the BWT of the reverse of the string
 * @param	inter			The current interval in the reverse BWT.
 * @param	normal			The current interval in the normal BWT.
 * @param	alphabet		The alphabet used by both BWT's.
 * @param	alphabet_length	The length of the alphabet.
 * @param	c_array			The c-array used in the interval of the BWT's.
 * @param	c				The current extension character.
 * @param	target			The structure in which the results should be stored.
 * 							If <code>NULL</code>, a new structure will be allocated
 * 							and returned.
 * @return 					An updated interval-struct of the BWT of the
 * 							reverse of the string.
 * @author	Topi Paavilainen, Max Sandberg (REXiator)
 * @bug		No known bugs.
 */
interval* update_reverse_interval(interval* inter, interval* normal,
		const char* alphabet, unsigned int alphabet_length, const int* c_array,
		const char c, interval* target) {

	if (target == 0)
		target = malloc(sizeof(interval));

	int index_in_c_array = binary_search(alphabet, &c, sizeof(char),
			alphabet_length, 0);
	int char_index = c_array[index_in_c_array];
	target->i = inter->i + char_index;

	//length of the reverse interval is same as length of the normal interval
	int normal_j = normal->j;
	int normal_i = normal->i;

	int new_j = target->i + (normal_j - normal_i);
	target->j = new_j;
	return target;
}

/**
 * @brief 	Creates a substring-struct from the parameters.
 * @param 	normal 	The interval of the substring in the normal BWT.
 * @param 	reverse The interval of the substring in the BWT of the reverse
 * 					of the string.
 * @param 	length 	Length of the substring.
 * @param	target	The structure in which the data should be stored. If
 * 					<code>NULL</code>, a new struct will be allocated and
 * 					returned.
 * @return			A pointer to an initialized substring-struct.
 * @author	Lassi Vapaakallio, Max Sandberg (REXiator)
 * @bug		No known bugs.
 */
substring* create_substring(interval* normal, interval* reverse, int length,
		substring* target) {
	if (target == 0)
		target = malloc(sizeof(substring));

	int normal_i = normal->i;
	int normal_j = normal->j;

	target->normal.i = normal_i;
	target->normal.j = normal_j;

	target->reverse.i = reverse->i;
	target->reverse.j = reverse->j;

	target->length = length;

	return target;
}

void iterate(char* string, void (*callback)(substring* substr)) {
	wavelet_tree* bwt = s_to_BWT(string), *rev_bwt = reverse_bwt(string);
	bit_vector* reverse_runs = create_runs_vector(rev_bwt, 0);
	substring_stack* stack = create_stack(10);
	int bwt_length = bwt->get_num_bits(bwt), i, alphabet_length =
			bwt->get_alphabet_length(bwt);
	unsigned int* c_array = malloc(
			(alphabet_length + 1) * sizeof(unsigned int));
	alphabet_data* alpha_data = malloc(sizeof(alphabet_data));
	alpha_data->alphabet = malloc((alphabet_length + 1) * sizeof(char));

	// WARNING WARNING, NOT GOOD
	max_repeats_initialize_bwt(bwt);

	//Initialize first intervals. In the start both intervals are the whole bwt
	interval* normal = &((interval ) { .i = 0, .j = bwt_length - 1 } );
	interval* reverse = &((interval ) { .i = 0, .j = bwt_length - 1 } );

	//create starting substring
	substring* new_substring = 0, *substr = create_substring(normal, reverse, 0,
			0);

	substring* temp;

	while (1) {
		if (substr == NULL)
			break;

		//if size of the interval is 1, it cannot be a right-maximal string
		//if(substr->normal->i == substring->normal->j) continue;

		// Determine characters that precede the interval
		alpha_data = create_alphabet_interval(&substr->normal, bwt, alpha_data);

		c_array = create_c_array(bwt, &substr->normal, 0, 0, c_array);

		alphabet_length = alpha_data->length;

		for (i = 0; i < alphabet_length; i++) {
			normal = backward_search_interval(bwt, &substr->normal,
					alpha_data->alphabet[i], normal);

			reverse = update_reverse_interval(&substr->reverse, normal,
					alpha_data->alphabet, alphabet_length, c_array,
					alpha_data->alphabet[i], reverse);

			if (is_reverse_interval_right_maximal(reverse_runs, reverse)) {
				new_substring = create_substring(normal, reverse,
						substr->length + 1, new_substring);
				// callback function pointers
				callback(new_substring);
				push(stack, new_substring);
			}
		}

		temp = pop(stack);

		if (temp == NULL)
			break;

		substr = create_substring(&temp->normal, &temp->reverse, temp->length,
				substr);
	}

	free(c_array);
	free(alpha_data->alphabet);
	free(alpha_data);
	free_stack(stack);
}

void iterate_for_tree_drawing(char* string,
		void (*callback)(substring* substr, substring* prev_substr, char c)) {

	wavelet_tree* bwt = s_to_BWT(string), *rev_bwt = reverse_bwt(string);
	bit_vector* reverse_runs = create_runs_vector(rev_bwt, 0);
	substring_stack* stack = create_stack(10);
	int bwt_length = bwt->get_num_bits(bwt), i, alphabet_length =
			bwt->get_alphabet_length(bwt);
	unsigned int* c_array = malloc(
			(alphabet_length + 1) * sizeof(unsigned int));
	alphabet_data* alpha_data = malloc(sizeof(alphabet_data));
	alpha_data->alphabet = malloc((alphabet_length + 1) * sizeof(char));

	// WARNING WARNING, NOT GOOD
	max_repeats_initialize_bwt(bwt);

	//Initialize first intervals. In the start both intervals are the whole bwt
	interval* normal = &((interval ) { .i = 0, .j = bwt_length - 1 } );
	interval* reverse = &((interval ) { .i = 0, .j = bwt_length - 1 } );

	//create starting substring
	substring* new_substring = 0, *substr = create_substring(normal, reverse, 0,
			0);

	substring* temp;

	while (1) {
		if (substr == NULL)
			break;

		//if size of the interval is 1, it cannot be a right-maximal string
		//if(substr->normal->i == substring->normal->j) continue;

		// Determine characters that precede the interval
		alpha_data = create_alphabet_interval(&substr->normal, bwt, alpha_data);

		c_array = create_c_array(bwt, &substr->normal, 0, 0, c_array);

		alphabet_length = alpha_data->length;

		for (i = 0; i < alphabet_length; i++) {
			normal = backward_search_interval(bwt, &substr->normal,
					alpha_data->alphabet[i], normal);

			if (normal == NULL)
				continue;

			reverse = update_reverse_interval(&substr->reverse, normal,
					alpha_data->alphabet, alphabet_length, c_array,
					alpha_data->alphabet[i], reverse);

			if (is_reverse_interval_right_maximal(reverse_runs, reverse)) {
				new_substring = create_substring(normal, reverse,
						substr->length + 1, new_substring);
				// callback function pointers
				callback(new_substring, substr, alpha_data->alphabet[i]);
				push(stack, new_substring);
			}
		}

		temp = pop(stack);

		if (temp == NULL)
			break;

		substr = create_substring(&temp->normal, &temp->reverse, temp->length,
				substr);
	}

	free(c_array);
	free(alpha_data->alphabet);
	free(alpha_data);
	free_stack(stack);
}

/**
 * @brief	Combines the intersection of the two alphabets into one common alphabet.
 * @param	alpha_data1		The data for the first alphabet.
 * @param	alpha_data2		The data for the second alphabet.
 * @param	common_alphabet	The c-string where the result should be stored.
 * @return					The intersection of the combined alphabets.
 * @author	Lassi Vapaakallio, Topi Paavilainen, Max Sandberg (REXiator)
 * @bug		No known bugs.
 */
char* combine_alphabets_intersection(alphabet_data* alpha_data1,
		alphabet_data* alpha_data2, char* common_alphabet) {
	char* alphabet1 = alpha_data1->alphabet;
	char* alphabet2 = alpha_data2->alphabet;
	int index1 = 0;
	int index2 = 0;
	int common_index = 0;
	if (alphabet1[index1] == END_STRING) {
		index1++;
	}
	if (alphabet2[index2] == END_STRING) {
		index2++;
	}
	while (index1 < alpha_data1->length && index2 < alpha_data2->length) {
		if (alphabet1[index1] == alphabet2[index2]) {
			common_alphabet[common_index] = alphabet1[index1];
			common_index++;
			index1++;
			index2++;
		} else if (alphabet1[index1] > alphabet2[index2]) {
			index2++;
		} else if (alphabet1[index1] < alphabet2[index2]) {
			index1++;
		} else {
			printf("THE IMPOSSIBLR HAPPENED");
		}
	}
	common_alphabet[common_index] = '\0';
	return common_alphabet;
}

void double_iterate(char* string1, char* string2,
		void (*callback)(substring* substr1, substring* substr2)) {

	wavelet_tree* bwt1 = s_to_BWT(string1), *rev_bwt1 = reverse_bwt(string1);
	wavelet_tree* bwt2 = s_to_BWT(string2), *rev_bwt2 = reverse_bwt(string2);
	bit_vector* reverse_runs1 = create_runs_vector(rev_bwt1, 0);
	bit_vector* reverse_runs2 = create_runs_vector(rev_bwt2, 0);
	substring_stack* stack1 = create_stack(10);
	substring_stack* stack2 = create_stack(10);
	int bwt_length1 = bwt1->get_num_bits(bwt1), i, alphabet_length1 =
			bwt1->get_alphabet_length(bwt1);
	int bwt_length2 = bwt2->get_num_bits(bwt2), alphabet_length2 =
			bwt2->get_alphabet_length(bwt2);
	unsigned int* c_array1 = malloc(
			(alphabet_length1 + 1) * sizeof(unsigned int));
	unsigned int* c_array2 = malloc(
			(alphabet_length2 + 1) * sizeof(unsigned int));
	alphabet_data* alpha_data1 = malloc(sizeof(alphabet_data));
	alpha_data1->alphabet = malloc((alphabet_length1 + 1) * sizeof(char));

	alphabet_data* alpha_data2 = malloc(sizeof(alphabet_data));
	alpha_data2->alphabet = malloc((alphabet_length2 + 1) * sizeof(char));

	char* common_alphabet = malloc(
			(alphabet_length2 + alphabet_length1 + 1) * sizeof(char));

	// WARNING WARNING, NOT GOOD
	mum_initialize_bwts(bwt1, bwt2, rev_bwt1, rev_bwt2);

	//Initialize first intervals. In the start both intervals are the whole bwt
	interval* normal1 = &((interval ) { .i = 0, .j = bwt_length1 - 1 } );
	interval* reverse1 = &((interval ) { .i = 0, .j = bwt_length1 - 1 } );

	interval* normal2 = &((interval ) { .i = 0, .j = bwt_length2 - 1 } );
	interval* reverse2 = &((interval ) { .i = 0, .j = bwt_length2 - 1 } );

	//create starting substring
	substring* new_substring1 = 0, *substring1 = create_substring(normal1,
			reverse1, 0, 0);

	substring* new_substring2 = 0, *substring2 = create_substring(normal2,
			reverse2, 0, 0);

	substring* temp;

	while (1) {
		//if size of the interval is 1, it cannot be a right-maximal string
		//if(substring->normal->i == substring->normal->j) continue;

		// Determine characters that precede the interval
		alpha_data1 = create_alphabet_interval(&substring1->normal, bwt1,
				alpha_data1);
		alpha_data2 = create_alphabet_interval(&substring2->normal, bwt2,
				alpha_data2);
		c_array1 = create_c_array(bwt1, &substring1->normal, 0, 0, c_array1);
		c_array2 = create_c_array(bwt2, &substring2->normal, 0, 0, c_array2);

		common_alphabet = combine_alphabets_intersection(alpha_data1,
				alpha_data2, common_alphabet);

		//common_alphabet = determine_alphabet(alphabets);

		int common_alphabet_length = strlen(common_alphabet);

		for (i = 0; i < common_alphabet_length; i++) {
			//print_node(substring1->normal);
			//printf("letter added to the left: %c \n", common_alphabet[i]);
			normal1 = backward_search_interval(bwt1, &substring1->normal,
					common_alphabet[i], normal1);
			if (normal1 == NULL) {
				continue;
			}
			//print_node(substring2->normal);
			normal2 = backward_search_interval(bwt2, &substring2->normal,
					common_alphabet[i], normal2);
			if (normal2 == NULL) {
				continue;
			}
			reverse1 = update_reverse_interval(&substring1->reverse, normal1,
					alpha_data1->alphabet, alpha_data1->length, c_array1,
					common_alphabet[i], reverse1);
			reverse2 = update_reverse_interval(&substring2->reverse, normal2,
					alpha_data2->alphabet, alpha_data2->length, c_array2,
					common_alphabet[i], reverse2);

			new_substring1 = create_substring(normal1, reverse1,
					substring1->length + 1, new_substring1);
			new_substring2 = create_substring(normal2, reverse2,
					substring2->length + 1, new_substring2);

			// callback function pointers
			callback(new_substring1, new_substring2);
			push(stack1, new_substring1);
			push(stack2, new_substring2);
		}

		temp = pop(stack1);
		if (temp == NULL) {
			break;
		}
		substring1 = create_substring(&temp->normal, &temp->reverse,
				temp->length, substring1);
		temp = pop(stack2);
		if (temp == NULL) {
			break;
		}
		substring2 = create_substring(&temp->normal, &temp->reverse,
				temp->length, substring2);
	}

	free(c_array1);
	free(c_array2);
	free(alpha_data1->alphabet);
	free(alpha_data1);
	free_stack(stack1);
	free(alpha_data2->alphabet);
	free(alpha_data2);
	free_stack(stack2);
	free(common_alphabet);
}