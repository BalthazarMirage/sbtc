#include "builder.h"

#include <stdlib.h>
#include <string.h>

Builder * builder_create (Filter * f, Clauses * c) {
	Builder * b = malloc (
		sizeof (Builder) +
		sizeof (int) * (c->num_nary + c->num_2ary)
	);
	
	b->num_doms = 0;
	b->num_ndoms = c->num_nary + c->num_2ary;
	
	b->c = c;
	b->f = f;
	b->dependant = (int *) (b+1);
	memset (b->dependant, 0, b->num_ndoms * sizeof(int));
	
	return b;
}

void builder_destroy (Builder * b) {
	free (b);
}

void builder_sort (Builder * b) {
	int n_proofs;
	int proofs [ b->c->num_2ary ];
	
	int * cur = clauses_first (b->c);
	for (int i = 0; i < b->c->num_nary; i++) {
		int size  = clauses_size (cur);
		int *vars = clauses_vars (cur);
		
		if ( (n_proofs = filter_are_exclusives (b->f, size, vars, proofs)) != 0 ) {
			b->num_ndoms--; b->num_doms++;
			b->dependant[i] = 1;
			for (int j = 0; j < n_proofs; j++) {
				if (b->dependant[b->c->num_nary + proofs[j]] == 0) {
					b->dependant[b->c->num_nary + proofs[j]] = 1;
					b->num_ndoms--;
				}
			}
			
		}
		
		cur = clauses_next (cur);
	}
	
}

int builder_is_domain (Builder * b, int idx) {
	return b->dependant[idx];
}

int builder_bin_is_clause (Builder * b, int idx) {
	return (! b->dependant[b->c->num_nary + idx] );
}
