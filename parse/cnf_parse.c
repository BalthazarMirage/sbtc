#include "cnf_parse.h"
#include "dimacs.h"

#include <stdlib.h>

size_t file_size (FILE * f) {
	long int cur = ftell (f);
	fseek (f, 0L, SEEK_END);
	long int en = ftell (f);
	fseek (f, cur, SEEK_SET);
	return en - cur;
}

void cnf_import (Cnf * cnf, FILE * src) {
	dimacs_parse_header (src, &cnf->num_vars, &cnf->num_clauses);
	
	size_t cap  = file_size(src) / 2;		// a upper bound of the size of the problem (in number of litterals)

	cnf->clauses = malloc (
		sizeof(size_t) * (cnf->num_clauses + 1) +
		sizeof(int) * cap
	);
	cnf->datas = (int *) (cnf->clauses + cnf->num_clauses + 1);
	cnf->clauses[cnf->num_clauses] = 0;
	
	size_t cl_size = 0;
	for (size_t i = 0; i < cnf->num_clauses; i++) {
		dimacs_parse_next_clause (src, &cl_size, cnf->datas + cnf_get_size(cnf));
		cnf->clauses[i] = cnf_get_size (cnf);
		cnf->clauses[cnf->num_clauses] += cl_size;
	}
	
	cnf->clauses = realloc (cnf->clauses,
		sizeof(size_t) * (cnf->num_clauses + 1) +
		sizeof(int) * cnf->clauses[cnf->num_clauses]
	);
	
}

void cnf_export (Cnf * cnf, FILE * dst) {
	fprintf (dst, "p cnf %ld %ld\n", cnf_get_num_vars(cnf), cnf_get_num_clauses(cnf));
	
	for (size_t i = 0; i < cnf_get_num_clauses(cnf); i++) {
		for (int * v = cnf_get_clause_begin (cnf, i); v != cnf_get_clause_end(cnf, i); v++) {
			fprintf (dst, "%d ", (*v));
		}
		fprintf (dst, "0\n");
	}
}

void cnf_empty (Cnf * cnf) {
	free (cnf->clauses);
}
