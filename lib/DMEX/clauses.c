#include "clauses.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

Clauses * clauses_create (int num_vars, int num_clauses) {
	Clauses * cls = malloc (
		sizeof (Clauses) +
		sizeof (int) * (num_vars * num_clauses + num_clauses)
	);
	
	cls->num_nary = 0;
	cls->num_2ary = 0;
	
	cls->vars_beg = (int *) (cls + 1);
	cls->vars_end = (int *) (cls->vars_beg + num_vars * num_clauses + num_clauses);
	cls->cursor_n = cls->vars_beg;
	cls->cursor_2 = cls->vars_end;
	
	return cls;
}

void clauses_destroy (Clauses * clauses) {
	free (clauses);
}


int * clauses_prepare_size (Clauses * clauses) {
	return clauses->cursor_n;
}

int * clauses_prepare_vars (Clauses * clauses) {
	return clauses->cursor_n + 1;
}

void clauses_stock (Clauses * clauses) {
	if (*(clauses->cursor_n) == 2) {
		clauses->num_2ary++;
		clauses->cursor_2 -= 3;
			
		memcpy (clauses->cursor_2, clauses->cursor_n, 3 * sizeof (int));
	} else {
		clauses->num_nary++;
		clauses->cursor_n += (*clauses->cursor_n + 1);
	}
}

int * clauses_first (Clauses * clauses) {
	return clauses->vars_beg;
}

int clauses_size (int * clause) {
	return (*clause);
}

int * clauses_vars (int * clause) {
	return clause + 1;
}

int * clauses_next (int * clause) {
	return clause + (*clause) + 1;
}

int * clauses_bin (Clauses * clauses, int idx) {
	return clauses->vars_end - (3 * (idx + 1));
}
