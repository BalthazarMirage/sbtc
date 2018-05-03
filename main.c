#include "parse/cnf_parse.h"
#include "deduce/trans_deduce.h"
#include "deduce/trans_method.h"

#include <stdlib.h>
#include <stdio.h>




int main (int argc, char * argv[]) {
	FILE * src = fopen (argv[1], "r");
	FILE * dst = fopen (argv[2], "w");	

	if (src == NULL) {
		return -1;
	}
	
	Cnf problem;
	cnf_import (&problem, src);
	
	printf ("%d\n", problem.clauses[problem.num_clauses]);
	
	cnf_export (&problem, dst);

	fclose (src);
	fclose (dst);
/*	
	Translation translation;
	translation_deduce (&problem, &translation);
	cnf_empty (&problem);

	printf ("p trans %ld %ld %ld\n\n", 
		translation_get_num_vars(&translation),
		translation_get_num_domains(&translation),
		translation_get_num_constraints(&translation));

	for (size_t i = 0; i < translation_get_num_domains (&translation); i++) {
		int * beg = translation_get_domain_begin(&translation, i);
		int * end = translation_get_domain_end(&translation, i);
		
		for (int * v = beg; v != end; v++) {
			printf ("%d ", *v);
		}	
		printf ("0\n");
	}
	printf ("\n");
*/	
/*
	for (size_t i = 0; i < translation_get_num_constraints(&translation); i++) {
		int * beg = translation_get_constraint_begin(&translation, i);
		int * end = translation_get_constraint_end(&translation, i);
		
		for (int * v = beg; v != end; v++) {
			printf ("%d ", *v);
		}
		printf ("0\n");
	}
	printf("\n");
	
	switch (translation_get_method (&translation)) {
		case TRANS_IMPOSSIBLE : printf ("not a translation\n"); break;
		case TRANS_DIRECT : printf ("direct translation\n"); break;
		case TRANS_SUPPORT : printf ("support translation\n"); break;
		case TRANS_KAD : printf ("k-ad translation\n"); break;
		case TRANS_UNKNOW : printf ("insufficient datas\n"); break;
		default : printf ("something was not right lel\n"); break;
	}

	translation_empty (&translation);
*/
	
	return 0;
}
