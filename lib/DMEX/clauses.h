#ifndef CLAUSES_H
#define CLAUSES_H

typedef struct {
	int num_nary, num_2ary;	// number of clauses (size != 2 and size 2)
	int * vars_beg;	// begin of variables area
	int * vars_end;	// end of variables area

	int * cursor_2;	// current cursor for pushing 2ary clauses.
	int * cursor_n;	// current cursor for pushing nary clauses.
} Clauses;

Clauses * clauses_create (int num_vars, int num_clauses);
void clauses_destroy (Clauses * clauses);

int * clauses_prepare_size (Clauses * clauses);
int * clauses_prepare_vars (Clauses * clauses);
void clauses_stock (Clauses * clauses);

int * clauses_first (Clauses * clauses);

int  clauses_size (int * clause);
int * clauses_vars (int * clause);
int * clauses_next (int * clause);

int * clauses_bin (Clauses * clauses, int idx);

#endif
