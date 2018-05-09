#include "analyse.h"
#include "vct.h"
#include <stdlib.h>

enum {
	CONSTRAINT,
	AT_LEAST_ONE,
	AT_MOST_ONE
};

typedef struct {
	int mark;
	unsigned char role;
} Status;


int * separate_binaries (Cnf * cnf, int * clauses) {
	int * nary = clauses;
	int * binary = clauses + cnf_num_clauses(cnf);
	
	for (int i = 0; i < cnf_num_clauses(cnf); i++) {
		if (cnf_size(cnf, i) == 2) {
			--binary;
			(*binary) = i;
		} else {
			(*nary) = i;
			++nary;
		}
	}
	return binary;
}

void determine_status (Vct * vct, Cnf * cnf, int cl, Status * status, int * buff) {
	if (cnf_size(cnf, cl) == 1) return; 	// nothing to do...
	
	int num_supports = 0;
	for (int * lit = cnf_begin(cnf, cl); lit != cnf_end(cnf, cl); lit++) {
		for (int * bin = vct_begin(vct, *lit); bin != vct_end(vct, *lit); bin++) {
			if ((*lit >= 0) ^ (*bin < 0)) continue;	// if same sign
			
			if (status[abs(*bin)-1].mark == cl) {
				buff[num_supports] = abs(*bin)-1;
				num_supports++;
			} else {
				status[abs(*bin)-1].mark = cl;
			}	
		}	
	}
	
	if (num_supports >= (cnf_size(cnf, cl) * (cnf_size(cnf, cl)-1) / 2)) {
		for (int i = 0; i < num_supports; i++) {
			status[buff[i]].role = AT_MOST_ONE;
		}	
		status[cl].role = AT_LEAST_ONE;
	}
}

void find_domains (Cnf * cnf, Analyse * ana) {	
	int * nary = malloc (sizeof(int) * cnf_num_clauses(cnf));
	int * binary = separate_binaries(cnf, nary);
	
	int num_nary = binary - nary;
	int num_binary = cnf_num_clauses(cnf) - num_nary;
	
	Vct * vct = vct_create (cnf, num_binary, binary);
	Status * status = malloc (sizeof(Status) * cnf_num_clauses(cnf));
	int * buff = malloc (sizeof(int) * num_binary);
	
	for (int i = 0; i < cnf_num_clauses(cnf); i++) {
		status[i].mark = i;
		status[i].role = CONSTRAINT;
	}
	
	int * doms = ana->doms;
	int * cons = ana->doms + cnf_num_clauses(cnf);
	
	for (int * cl = nary; cl != binary; cl++) {
		determine_status(vct, cnf, *cl, status, buff);
		if (status[*cl].role == AT_LEAST_ONE) {
			(*doms) = *cl;
			doms++;
			ana->num_doms++;
		} else {
			cons--;
			(*cons) = *cl;
			ana->num_cons++;
		}
	}
	
	for (int * cl = binary; cl != nary + cnf_num_clauses(cnf); cl++) {
		if (status[*cl].role == CONSTRAINT) {
			cons--;
			(*cons) = *cl;
			ana->num_cons++;
		}
	}
	
	ana->cons = cons;
	
	vct_destroy(vct);
	free (status);
	free (buff);
	free (nary);
}


Analyse * analyse (Cnf * cnf) {
	Analyse * result = malloc (
		sizeof (Analyse) +
		sizeof (int) * cnf_num_vars(cnf) +
		sizeof (int) * cnf_num_clauses(cnf)
	);
	
	result->num_doms = 0;
	result->num_cons = 0;
	result->vars = (int *) (result + 1);
	result->doms = result->vars + cnf_num_vars(cnf);
	result->cons = result->doms;
	
	
	find_domains (cnf, result);
	
	// make analysis of covering peraps...
	
	return result;
}
