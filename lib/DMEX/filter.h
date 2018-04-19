#ifndef EXCLUSION_FILTER_H
#define EXCLUSION_FILTER_H

typedef struct {
	int clause;
	int next;
} Node;

typedef struct {
	int num_vars, num_clauses;
	int * vars;
	int * clauses;
	Node * nodes;
} Filter;

Filter * filter_create (int cap_vars, int cap_clauses);
void filter_destroy (Filter * f);

void filter_add_exclusion (Filter * f, int v1, int v2);

/**
 * proof is here to indicate the binary clause used to find
 * the exclusivity.
 * that way, we can exclude them from play.
 * 
 * return 0 if not exclusives, number of proofs otherwise.
 **/
int filter_are_exclusives (Filter * f, int num_vars, int * vars, int * proof);

#endif
