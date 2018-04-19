#ifndef BUILDER_H
#define BUILDER_H

#include "filter.h"
#include "clauses.h"

typedef struct {
	Filter  * f;
	Clauses * c;
	
	int num_doms, num_ndoms;
	int  * dependant;
} Builder;

Builder * builder_create (Filter * f, Clauses * c);
void builder_destroy (Builder * b);

void builder_sort (Builder * b);

int builder_is_domain (Builder * b, int idx);
int builder_bin_is_clause (Builder * b, int idx);
#endif
