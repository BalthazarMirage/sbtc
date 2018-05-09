#include "cnf.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/***********************************************************************
 * HELPERS
 **********************************************************************/
 
void skip_spaces (FILE * f) {
    int c;
    while (isspace((c=fgetc(f))));
    ungetc(c, f);
}

void parse_comment (FILE * f) {
    int c;
    while ((c=fgetc(f)) != '\n' && c != EOF);
    ungetc(c, f);
}

void parse_problem (FILE * f, int * num_vars, int * num_clauses) {
    fscanf(f, "p cnf %d %d", num_vars, num_clauses);
}

void parse_clause (FILE * f, int * size_clause, int * vars) {
    (*size_clause) = 0;

    int var; fscanf(f, "%d", &var);
    while (var != 0) {
        vars[*size_clause] = var;
        (*size_clause)++;
        fscanf(f, "%d", &var);
    }
}

int dimacs_parse_header (FILE * src, int * num_vars, int * num_clauses) {
    while (1) {
        skip_spaces(src);
        int c = fgetc(src);
        ungetc(c,src);

        switch (c) {
            case 'p' : parse_problem(src, num_vars, num_clauses); return 0;
            case 'c' : parse_comment(src); break;
            case EOF : return -1;
            default : return -1;
        }
    }
}

int dimacs_parse_next_clause (FILE * src, int * num_vars, int * vars) {
    while (1) {
        skip_spaces(src);
        int c = fgetc(src);
        ungetc(c,src);

        switch (c) {
            case 'p' : return -1;
            case 'c' : parse_comment(src); break;
            case EOF : return -1;
            default : parse_clause(src, num_vars, vars); return 0;
        }
    }
}



size_t file_size (FILE * f) {
	long int cur = ftell (f);
	fseek (f, 0L, SEEK_END);
	long int en = ftell (f);
	fseek (f, cur, SEEK_SET);
	return en - cur;
}



/***********************************************************************
 * INTERFACES
 **********************************************************************/

Cnf * cnf_load (const char * filename) {
	FILE * src = fopen (filename, "r");
	if (src == NULL) return NULL;
	
	int num_vars, num_clauses;
	dimacs_parse_header (src, &num_vars, &num_clauses);
	
	/* maximum number of litterals contained in this file */
	int cap_litterals = file_size(src) / 2 - num_clauses;
	
	Cnf * cnf = malloc (
		sizeof (Cnf) * (1) +
		sizeof (int) * (num_clauses+1) + 	// clauses
		sizeof (int) * (cap_litterals)		// litterals
	);
	
	if (cnf == NULL) {
		fclose (src);
		return NULL;
	}
	
	cnf->num_clauses = num_clauses;
	cnf->num_vars = num_vars;
	cnf->clauses = (int *) (cnf + 1);
	cnf->datas = (int *) (cnf->clauses + num_clauses + 1);
	cnf->clauses[0] = 0;
	
	for (int i = 0; i < cnf->num_clauses; i++) {
		dimacs_parse_next_clause (src, cnf->clauses + i + 1, cnf->datas + cnf->clauses[i]);
		cnf->clauses[i + 1] += cnf->clauses[i];
	}
	
	cnf = realloc (cnf,
		sizeof(Cnf) +
		sizeof(int) * (cnf->num_clauses + 1) +
		sizeof(int) * cnf->clauses[cnf->num_clauses]
	);
	
	fclose (src);
	return cnf;
}

void cnf_save (Cnf * cnf, const char * filename) {
	FILE * dst = fopen (filename, "w");
	if (dst == NULL) return;
	
	fprintf (dst, "p cnf %d %d\n", cnf_num_vars(cnf), cnf_num_clauses(cnf));
	
	for (int i = 0; i < cnf_num_clauses(cnf); i++) {
		for (int * l = cnf_begin (cnf, i); l != cnf_end (cnf, i); l++)
			fprintf (dst, "%d ", *l);
		fprintf (dst, "0\n");
	}
	fclose (dst);
}

void cnf_destroy (Cnf * cnf) {
	free (cnf);
}

int cnf_num_vars (Cnf * cnf) { return cnf->num_vars; }
int cnf_num_clauses (Cnf * cnf) { return cnf->num_clauses; }
int cnf_num_litterals (Cnf * cnf) { return cnf->clauses[cnf->num_clauses]; }


int cnf_size (Cnf * cnf, int clause) { return cnf->clauses[clause+1] - cnf->clauses[clause]; }
int * cnf_begin (Cnf * cnf, int clause) { return cnf->datas + cnf->clauses[clause]; }
int * cnf_end (Cnf * cnf, int clause) { return cnf->datas + cnf->clauses[clause+1]; }

int cnf_litteral (Cnf * cnf, int clause, int lit) { return cnf_begin(cnf, clause)[lit]; }
