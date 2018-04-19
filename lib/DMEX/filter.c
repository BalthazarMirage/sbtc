#include "filter.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

Filter * filter_create (int cap_vars, int cap_clauses) {
	Filter * f = malloc ( 
		sizeof (Filter) +
		sizeof (int) * cap_vars +
		sizeof (int) * cap_clauses +
		sizeof (Node) * (2 * cap_clauses)
	);
	
	f->num_vars = 0;
	f->num_clauses = 0;
	
	f->vars = (int *) (f+1);
	f->clauses = (int *) (f->vars + cap_vars);
	f->nodes = (Node *) (f->clauses + cap_clauses);
	
	memset (f->vars, 0, cap_vars * sizeof(int));
	memset (f->clauses, 0, cap_clauses * sizeof(int));
	
	return f;
}

void filter_destroy (Filter * f) {
	free (f);
}

void filter_add_exclusion (Filter * f, int v1, int v2) {
	Node * nodes = f->nodes + ( 2 * f->num_clauses );
	
	nodes[0].clause = (v1 > 0) ? f->num_clauses+1 : -(f->num_clauses+1);
	nodes[1].clause = (v2 > 0) ? f->num_clauses+1 : -(f->num_clauses+1); 
	
	int hsh0 = abs(v1)-1;
	int hsh1 = abs(v2)-1;
	
	nodes[0].next = f->vars[hsh0];
	nodes[1].next = f->vars[hsh1];
	
	f->vars[hsh0] = 2 * f->num_clauses + 1;
	f->vars[hsh1] = 2 * f->num_clauses + 2;
	
	f->num_clauses++;
}


Node * nodes_of (Filter * f, int var) {
	int hsh = abs(var)-1;
	int idx = f->vars[hsh];
	
	if (idx == 0) return NULL;
	else return f->nodes + idx - 1;
}

Node * nodes_next (Filter * f, Node * n) {
	if (n->next == 0) return NULL;
	else return f->nodes + n->next - 1;
}


int filter_are_exclusives (Filter * f, int num_vars, int * vars, int * proof) {
	if (num_vars == 1) return 0;	// cannot be exclusives with yourself.
	
	int cpt = 0;
	memset (f->clauses, 0, f->num_clauses * sizeof(int));
	for (int i = 0; i < num_vars; i++) {
		for (Node * n = nodes_of (f, vars[i]); n != NULL; n = nodes_next (f, n)) {
			if (! ( (n->clause >= 0) ^ (vars[i] < 0) ) ) continue;	// verify same sign
			
			if (f->clauses[abs(n->clause)-1] == 1) {
				proof[cpt] = abs(n->clause)-1;
				cpt++;
			} else {
				f->clauses[abs(n->clause)-1] = 1;	
			}
		}
	}
	if (cpt >= (num_vars)*(num_vars-1) / 2) return cpt;
	else return 0;
	
}
