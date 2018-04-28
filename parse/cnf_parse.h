#ifndef CNF_PARSE_H
#define CNF_PARSE_H

#include "../types/cnf.h"
#include <stdio.h>

void cnf_import (Cnf * cnf, FILE * src);
void cnf_export (Cnf * cnf, FILE * dst);
void cnf_empty (Cnf * cnf);


#endif
