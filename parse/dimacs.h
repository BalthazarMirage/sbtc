//
// Created by balthazar on 11/04/18.
//

#ifndef DIMACS_H
#define DIMACS_H

#include <stdio.h>
#include <stddef.h>

/**
 * dimacs_parse_header parse the problem line, skipping all comments in the process
 * @param f the source file
 * @param num_vars an address to stock the number of variables
 * @param num_clauses an address to stock the number of clauses
 * @return 0 if successful, any negative value otherwise.
 */
int dimacs_parse_header (FILE * f, size_t * num_vars, size_t * num_clauses);

/**
 * dimacs_parse_next_clause parse the next encountered clause, skipping all comments in the process
 * @param f the source file
 * @param num_vars an address to stock the size of the clause
 * @param vars an address to stock the variables of the clause
 * @return 0 if successful, any negative value otherwise.
 */
int dimacs_parse_next_clause (FILE * f, size_t * num_vars, int * vars);


#endif //SBTC_DIMACS_H

