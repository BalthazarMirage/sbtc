//
// Created by balthazar on 11/04/18.
//

#include <ctype.h>
#include "dimacs.h"

static
void skip_spaces (FILE * f) {
    int c;
    while (isspace((c=fgetc(f))));
    ungetc(c, f);
}

static
void parse_comment (FILE * f) {
    int c;
    while ((c=fgetc(f)) != '\n' && c != EOF);
    ungetc(c, f);
}

static
void parse_problem (FILE * f, size_t * num_vars, size_t * num_clauses) {
    fscanf(f, "p cnf %ld %ld", num_vars, num_clauses);
}

static
void parse_clause (FILE * f, size_t * size_clause, int * vars) {
    (*size_clause) = 0;

    int var; fscanf(f, "%d", &var);
    while (var != 0) {
        vars[*size_clause] = var;
        (*size_clause)++;
        fscanf(f, "%d", &var);
    }
}

int dimacs_parse_header (FILE * src, size_t * num_vars, size_t * num_clauses) {
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

int dimacs_parse_next_clause (FILE * src, size_t * num_vars, int * vars) {
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

