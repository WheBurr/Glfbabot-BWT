/*
 * check_maximal_repeats.c
 *
 *  Created on: 13.3.2014
 *      Author: lvapaaka
 */

#include "../utils_for_tests.h"
#include "../../src/core/iterate.h"
#include "../../src/core/s_to_bwt.h"
#include "../../src/applications/maximal_repeats.h"
#include "../../src/applications/mapper.h"
#include "../../src/applications/map_bwt_to_s.h"
#include "../../src/utils/wavelet_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <check.h>

START_TEST(test_maximal_repeat2)
	{
		parameter_struct* params = initialize_for_max_repeats("abracadabra",1);
		iterator_state* state = iterate(params);
		max_repeat_results* results = (max_repeat_results*) params->ret_data;
		ck_assert_int_eq(2, results->length);
		ck_assert_int_eq(1, results->data[0].normal.i);
		ck_assert_int_eq(5, results->data[0].normal.j);
		ck_assert_int_eq(1, results->data[0].length);
		ck_assert_int_eq(2, results->data[1].normal.i);
		ck_assert_int_eq(3, results->data[1].normal.j);
		ck_assert_int_eq(4, results->data[1].length);

		free(results->data);
		free_parameter_struct(params);
		free_iterator_state(state);
	}END_TEST

START_TEST(test_maximal_repeat2_mapped)
	{
		parameter_struct* params = initialize_for_max_repeats("abracadabra",1);
		iterator_state* state = iterate(params);
		max_repeat_results* results = (max_repeat_results*) params->ret_data;
		max_repeat_node* nodes = results->data;
		max_repeat_with_indexes* mapped_nodes = map_maximal_repeats_to_string(
				nodes, s_to_bwt("abracadabra"), results->length,
				max_repeat_make_bit_vector(results, state));
		ck_assert_int_eq(2, results->length);
		ck_assert_int_eq(0, mapped_nodes[0].indexes[0]);
		ck_assert_int_eq(1, mapped_nodes[0].length);
		ck_assert_int_eq(0, mapped_nodes[1].indexes[0]);
		ck_assert_int_eq(4, mapped_nodes[1].length);

		free(results->data);
		free_parameter_struct(params);
		free_iterator_state(state);
	}END_TEST

START_TEST(test_maximal_repeat2b)
	{
		parameter_struct* params = initialize_for_max_repeats("hattivatti",1);
		iterator_state* state = iterate(params);
		max_repeat_results* results = (max_repeat_results*) params->ret_data;
		max_repeat_node* nodes = results->data;
		ck_assert_int_eq(2, results->length);
		ck_assert_int_eq(6, nodes[0].normal.i);
		ck_assert_int_eq(9, nodes[0].normal.j);
		ck_assert_int_eq(1, nodes[0].length);
		ck_assert_int_eq(1, nodes[1].normal.i);
		ck_assert_int_eq(2, nodes[1].normal.j);
		ck_assert_int_eq(4, nodes[1].length);

		free(results->data);
		free_parameter_struct(params);
		free_iterator_state(state);
	}END_TEST

START_TEST(test_maximal_repeat2b_mapped)
	{
		parameter_struct* params = initialize_for_max_repeats("hattivatti",1);
		iterator_state* state = iterate(params);
		max_repeat_results* results = (max_repeat_results*) params->ret_data;
		max_repeat_with_indexes* nodes = map_maximal_repeats_to_string(
				results->data, s_to_bwt("hattivatti"), results->length,
				max_repeat_make_bit_vector(results, state));
		ck_assert_int_eq(2, results->length);
		ck_assert_int_eq(1, nodes[0].indexes[0]);
		ck_assert_int_eq(4, nodes[0].length);
		ck_assert_int_eq(2, nodes[1].indexes[0]);
		ck_assert_int_eq(1, nodes[1].length);

		free(results->data);
		free_parameter_struct(params);
		free_iterator_state(state);
	}END_TEST

START_TEST(test_maximal_repeat3)
	{
		parameter_struct* params = initialize_for_max_repeats("balalaikka",1);
		iterator_state* state = iterate(params);
		max_repeat_results* results = (max_repeat_results*) params->ret_data;
		max_repeat_node* nodes = results->data;
		ck_assert_int_eq(3, results->length);
		ck_assert_int_eq(1, nodes[0].normal.i);
		ck_assert_int_eq(4, nodes[0].normal.j);
		ck_assert_int_eq(1, nodes[0].length);
		ck_assert_int_eq(7, nodes[1].normal.i);
		ck_assert_int_eq(8, nodes[1].normal.j);
		ck_assert_int_eq(1, nodes[1].length);
		ck_assert_int_eq(3, nodes[2].normal.i);
		ck_assert_int_eq(4, nodes[2].normal.j);
		ck_assert_int_eq(3, nodes[2].length);

		free(results->data);
		free_parameter_struct(params);
		free_iterator_state(state);
	}END_TEST

START_TEST(test_maximal_repeat3_mapped)
	{
		parameter_struct* params = initialize_for_max_repeats("balalaikka",1);
		iterator_state* state = iterate(params);
		max_repeat_results* results = (max_repeat_results*) params->ret_data;
		max_repeat_with_indexes* nodes = map_maximal_repeats_to_string(
				results->data, s_to_bwt("balalaikka"), results->length,
				max_repeat_make_bit_vector(results, state));
		ck_assert_int_eq(3, results->length);
		ck_assert_int_eq(1, nodes[0].indexes[0]);
		ck_assert_int_eq(1, nodes[0].length);
		ck_assert_int_eq(1, nodes[1].indexes[0]);
		ck_assert_int_eq(3, nodes[1].length);
		ck_assert_int_eq(7, nodes[2].indexes[0]);
		ck_assert_int_eq(1, nodes[2].length);

		free(results->data);
		free_parameter_struct(params);
		free_iterator_state(state);
	}END_TEST

START_TEST(test_maximal_repeat_empty)
	{
		parameter_struct* params = initialize_for_max_repeats("abc",1);
		iterator_state* state = iterate(params);
		max_repeat_results* results = (max_repeat_results*) params->ret_data;
		ck_assert_int_eq(0, results->length);

		free(results->data);
		free_parameter_struct(params);
		free_iterator_state(state);
	}END_TEST

START_TEST(test_maximal_repeat1)
	{
		parameter_struct* params = initialize_for_max_repeats("ilotalo",1);
		iterator_state* state = iterate(params);
		max_repeat_results* results = (max_repeat_results*) params->ret_data;
		max_repeat_node* nodes = results->data;
		ck_assert_int_eq(1, results->length);
		ck_assert_int_eq(3, nodes[0].normal.i);
		ck_assert_int_eq(4, nodes[0].normal.j);
		ck_assert_int_eq(2, nodes[0].length);

		free(results->data);
		free_parameter_struct(params);
		free_iterator_state(state);
	}END_TEST

START_TEST(test_maximal_repeat1_mapped)
	{
		parameter_struct* params = initialize_for_max_repeats("ilotalo",1);
		iterator_state* state = iterate(params);
		max_repeat_results* results = (max_repeat_results*) params->ret_data;
		max_repeat_with_indexes* nodes = map_maximal_repeats_to_string(
				results->data, s_to_bwt("ilotalo"), results->length,
				max_repeat_make_bit_vector(results, state));
		ck_assert_int_eq(1, results->length);
		ck_assert_int_eq(1, nodes[0].indexes[0]);
		ck_assert_int_eq(2, nodes[0].length);

		free(results->data);
		free_parameter_struct(params);
		free_iterator_state(state);
	}END_TEST

//returns 0 if the searched substring is not found
int find_and_remove_test_substr(test_substr* head, int index, int length) {
	test_substr* previous = head;
	test_substr* current = head->next;

	while (current != NULL) {

		if (current->length == length && current->start_index == index) {
			previous->next = current->next;
			free(current);
			return 1;
		}
		previous = current;
		current = current->next;
	}
	return 0;
}

START_TEST(test_max_repeats_randomized)
	{
		srand(time(NULL));
		char* test;
		wavelet_tree* bwt;
		int* suffix_array;
		int length;
		char* alphabet = "abcgdf";

		for (int i = 0; i < 100; i++) {

			test = generate_random_string(alphabet, rand() % 100 + 1);
			bwt = s_to_bwt(test);
			suffix_array = map_create_suffix_array_from_bwt(bwt);


			test_substr* maxrep_naive = find_maximal_repeat_substrings(test,1);
			parameter_struct* params = initialize_for_max_repeats(test,1);
			iterator_state* state = iterate(params);
			max_repeat_results* results = (max_repeat_results*) params->ret_data;
			max_repeat_node* maxrep_fast = results->data;
			int max_number_nodes = results->length;

			for (int j = 0; j < max_number_nodes; j++) {
				for (int k = 0;
						k + maxrep_fast[j].normal.i <= maxrep_fast[j].normal.j;
						k++) {

					fail_unless(
							find_and_remove_test_substr(maxrep_naive,
									suffix_array[maxrep_fast[j].normal.i + k],
									maxrep_fast[j].length));
				}
			}
			print_substring_list(test, maxrep_naive);
			fail_unless(maxrep_naive->next == NULL);

			free_wavelet_tree(bwt);
			free(suffix_array);
			free(maxrep_naive);
			free(results->data);
			free_parameter_struct(params);
			free_iterator_state(state);
		}
	}END_TEST

START_TEST(test_max_repeats_randomized2)
	{
		srand(time(NULL));
		char* test;
		wavelet_tree* bwt;
		int* suffix_array;
		int length;
		char* alphabet = "aoskfdhebs";

		for (int i = 0; i < 10; i++) {

			test = generate_random_string(alphabet, rand() % 1000 + 100);
			bwt = s_to_bwt(test);
			suffix_array = map_create_suffix_array_from_bwt(bwt);
			int threshold = rand()%5+1;

			test_substr* maxrep_naive = find_maximal_repeat_substrings(test,threshold);
			parameter_struct* params = initialize_for_max_repeats(test,threshold);
			iterator_state* state = iterate(params);
			max_repeat_results* results = (max_repeat_results*) params->ret_data;
			max_repeat_node* maxrep_fast = results->data;
			int max_number_nodes = results->length;

			for (int j = 0; j < max_number_nodes; j++) {
				for (int k = 0;
						k + maxrep_fast[j].normal.i <= maxrep_fast[j].normal.j;
						k++) {

					fail_unless(
							find_and_remove_test_substr(maxrep_naive,
									suffix_array[maxrep_fast[j].normal.i + k],
									maxrep_fast[j].length));
				}
			}
			print_substring_list(test, maxrep_naive);
			fail_unless(maxrep_naive->next == NULL);

			free_wavelet_tree(bwt);
			free(suffix_array);
			free(maxrep_naive);
			free(results->data);
			free_parameter_struct(params);
			free_iterator_state(state);
		}
	}END_TEST

TCase * create_max_repeats_randomized_test_case(void) {
	TCase * tc_randrep = tcase_create("max_repeats_randomized");
	tcase_add_test(tc_randrep, test_max_repeats_randomized2);
	tcase_add_test(tc_randrep, test_max_repeats_randomized);
	tcase_set_timeout(tc_randrep, 100);
	return tc_randrep;
}

TCase * create_maximal_repeats_test_case(void) {
	TCase * tc_stack = tcase_create("maximal_repeat_test");
	tcase_add_test(tc_stack, test_maximal_repeat2);
	//tcase_add_test(tc_stack, test_maximal_repeat2_mapped);
	tcase_add_test(tc_stack, test_maximal_repeat2b);
	//tcase_add_test(tc_stack, test_maximal_repeat2b_mapped);
	tcase_add_test(tc_stack, test_maximal_repeat3);
	//tcase_add_test(tc_stack, test_maximal_repeat3_mapped);
	tcase_add_test(tc_stack, test_maximal_repeat_empty);
	tcase_add_test(tc_stack, test_maximal_repeat1);
	//tcase_add_test(tc_stack, test_maximal_repeat1_mapped);
	return tc_stack;
}

Suite * test_suite(void) {
	Suite *s = suite_create("Maximal repeats tests");
	TCase *tc_stack = create_maximal_repeats_test_case();
	TCase *tc_randrep = create_max_repeats_randomized_test_case();
	suite_add_tcase(s, tc_stack);
	suite_add_tcase(s, tc_randrep);

	return s;
}

int main() {
	int number_failed;
	Suite *s = test_suite();
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	if (number_failed == 0) {
		return 0;
	} else {
		return 1;
	}
}

