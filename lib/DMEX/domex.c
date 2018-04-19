#include "domex.h"
#include "dimacs.h"
#include "filter.h"
#include "clauses.h"
#include "builder.h"

#include <stdio.h>
#include <string.h>


void print (FILE * f, int size, int * vars) {
	for (int i = 0; i < size; i++) {
		fprintf (f, "%d ", vars[i]);
	}
	fprintf (f, "0\n");
}


void domex_explicit (FILE * cnf, FILE * dmex) {
	int num_vars, num_clauses;
	
	dimacs_parse_header (cnf, &num_vars, &num_clauses);
	
	Clauses * c = clauses_create (num_vars, num_clauses);
	Filter * f = filter_create (num_vars, num_clauses);
	
	for (int i = 0; i < num_clauses; i++) {
		int * size = clauses_prepare_size (c);
		int * vars = clauses_prepare_vars (c);
		
		dimacs_parse_next_clause (cnf, clauses_prepare_size(c), clauses_prepare_vars(c));
		
		if (*size == 2)
			filter_add_exclusion (f, -vars[0], -vars[1]);
			
		clauses_stock(c);
	}
	
	Builder * b = builder_create (f, c);
	builder_sort (b);
	
	
	fprintf (dmex, "p dmex %d %d %d\n", num_vars, b->num_doms, b->num_ndoms);
	
	int * cur = clauses_first (c);
	for (int i = 0; i < c->num_nary; i++) {
		if (builder_is_domain (b, i)) {
			print (dmex, clauses_size (cur), clauses_vars(cur));
		}
		cur = clauses_next (cur);
	}
	
	cur = clauses_first (c);
	for (int i = 0; i < c->num_nary; i++) {
		if (! builder_is_domain (b, i)) {
			print (dmex, clauses_size(cur), clauses_vars(cur));
		}
	}
	
	for (int i = 0; i < c->num_2ary; i++) {
		cur = clauses_bin (c, i);
		if ( builder_bin_is_clause (b, i) ) {
			print (dmex, clauses_size(cur), clauses_vars(cur));
		}
	}
	
#if 0
	int num_proofs;
	int proofs[ c->num_2ary ];
	int dep [ num_clauses ];
	memset (dep, 0, num_clauses * sizeof (int));
	
	int num_dom = 0, num_ndom = num_clauses;
	
	int * curr = clauses_first (c);
	for (int i = 0; i < c->num_nary; i++) {
		int  size  = clauses_size (curr);
		int * vars = clauses_vars (curr);
		
		if( (num_proofs = filter_are_exclusives(f, size, vars, proofs)) != 0) {
			num_dom++; num_ndom--;
			dep[i] = 1;
			
			for (int j = 0; j < num_proofs; j++) {
				if ( ! dep[c->num_nary + proofs[j]] ) num_ndom--;
				dep[ c->num_nary + proofs[j] ] = 1;
			}
		}
		
		curr = clauses_next (curr);
	}
	
	
	
	fprintf (dmex, "p dmex %d %d %d\n", num_vars, num_dom, num_ndom);
	
	curr = clauses_first (c);
	for (int i = 0; i < c->num_nary; i++) {
		int  size  = clauses_size (curr);
		int * vars = clauses_vars (curr);
		
		if (dep[i]) {
			for (int j = 0; j < size; j++) {
				fprintf (dmex, "%d ", vars[j]);
			}
			fprintf (dmex, "0\n");
		}
		curr = clauses_next (curr);
	}
	
	curr = clauses_first (c);
	for (int i = 0; i < c->num_nary; i++) {
		int  size  = clauses_size (curr);
		int * vars = clauses_vars (curr);
		
		if (! dep[i]) {
			for (int j = 0; j < size; j++) {
				fprintf (dmex,"%d ", vars[j]);
			}
			fprintf (dmex,"0\n");
		}
		curr = clauses_next (curr);
	}
	
	for (int i = 0; i < c->num_2ary; i++) {
		curr = clauses_bin (c, i);
		int  size  = clauses_size (curr);
		int * vars = clauses_vars (curr);
		
		if (! dep[c->num_nary + i]) {
			for (int j = 0; j < size; j++) {
				fprintf (dmex, "%d ", vars[j]);
			}
			fprintf (dmex,"0\n");
		}
	}
#endif
}
