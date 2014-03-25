/*
 * mapper.c
 *
 *  Created on: 13.3.2014
 *      Author: lvapaaka
 */

#include "../../include/maximal_repeats.h"
#include "../../include/mum.h"
#include "triplet_sorter.h"
#include <stdlib.h>
#include <string.h>

void map_maximal_repeats_to_string(max_repeat_node* nodes, char* bwt, int count) {
	int i = 0;
	long n = strlen(bwt);
	int k;
	Interval* interval = malloc(sizeof(Interval));
	interval->i = 0;
	interval->j = 0;
	compare_quick_sort(nodes, count, sizeof(max_repeat_node), &compare_max_repeat_nodes);
	for (k = 0; k < n; k++) {
		if (nodes[i].normal->i == interval->i) {
			nodes[i].normal->i = (n - k) - 1;
			i++;
			if (i == count) break;
		}
		interval = backward_search_interval(bwt, interval, bwt[interval->i]);
	}
	free(interval);

}

void update_position_in_triplets(triplet* nodes, char* bwt, int count,
		int position) {
	int i = 0;
	long n = strlen(bwt);
	int k;
	Interval* interval = malloc(sizeof(Interval));
	interval->i = 0;
	interval->j = 0;
	
	if (position == 1) {
		compare_quick_sort(nodes, count, sizeof(triplet), &compare_triplets_pos1);
	} else if (position == 2) {
		compare_quick_sort(nodes, count, sizeof(triplet), &compare_triplets_pos2);
	}
		
	for (k = 0; k < n; k++) {
		if (position == 1) {
			if (nodes[i].pos1 == interval->i) {
				nodes[i].pos1 = (n - k) - 1;
				i++;
				if (i == count) break;
			}
		}
		if (position == 2) {
			if (nodes[i].pos2 == interval->i) {
				nodes[i].pos2 = (n - k) - 1;
				i++;
				if (i == count) break;
			}
		}
		interval = backward_search_interval(bwt, interval, bwt[interval->i]);
	}
	free(interval);

}

void map_mum_triplets_to_string(triplet* nodes, char* bwt1, char* bwt2,
		int count) {
	update_position_in_triplets(nodes, bwt1, count, 1);
	update_position_in_triplets(nodes, bwt2, count, 2);
}
