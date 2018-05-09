#ifndef ANALYSE_H
#define ANALYSE_H

#include "../cnf/cnf.h"
#include <stddef.h>

typedef struct {
	int num_doms, num_cons;
	int * vars;
	int * doms, * cons;
} Analyse;


Analyse * analyse (Cnf * cnf);

#endif
