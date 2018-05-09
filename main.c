#include "cnf/cnf.h"
#include "analyse/analyse.h"
#include <stdlib.h>
#include <stdio.h>




int main (int argc, char * argv[]) {
	Cnf * cnf = cnf_load (argv[1]);
	if (cnf == NULL) return 0;
	
	Analyse * ana = analyse (cnf);
	if (ana == NULL) {
		cnf_destroy (cnf);
	}
	
	Vct * vct = vct_create (cnf, ana->num_doms, ana->doms);
	
	cnf_destroy(cnf);
	vct_destroy(vct);
	free (ana);	// do ana_destroy...
}
