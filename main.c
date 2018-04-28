#include "parse/cnf_parse.h"
#include "deduce/trans_deduce.h"

#include <stdlib.h>
#include <stdio.h>




int main (int argc, char * argv[]) {
	FILE * src = fopen (argv[1], "r");
	
	if (src == NULL) {
		fclose (src);
		return -1;
	}
	
	Cnf problem;
	cnf_import (&problem, src);
	fclose (src);
	
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
	
	for (size_t i = 0; i < translation_get_num_constraints(&translation); i++) {
		int * beg = translation_get_constraint_begin(&translation, i);
		int * end = translation_get_constraint_end(&translation, i);
		
		for (int * v = beg; v != end; v++) {
			printf ("%d ", *v);
		}
		printf ("0\n");
	}
	printf("\n");

	translation_empty (&translation);

	
	return 0;
}
