#include "trans_method.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int translation_get_method (Translation * tls) {
	
	// cannot be translated if no domain found...
	if (tls->num_doms == 0) return TRANS_IMPOSSIBLE;
	
	int conflict = 0;
	
	size_t to_cover = tls->num_vars;
	long covering[ tls->num_vars ];
	
	memset (covering, 0, sizeof(size_t) * tls->num_vars);
	
	for (size_t dom = 0; dom < tls->num_doms; dom++) {
		int * beg = translation_get_domain_begin (tls, dom);
		int * end = translation_get_domain_end (tls, dom);
		
		for (int * v = beg; v != end; v++) {
			if (covering[abs(*v)-1] == 0) {
				to_cover--;
				covering[abs(*v)-1] = (*v > 0) ? dom+1:-dom-1;
			} else {
				conflict = 1;
				return 0;
			}
		}
	}
	
	if (! conflict) {
		if (! to_cover) {
			return TRANS_DIRECT;	// OR SUPPORT we don't know yet...
		} else {
			return TRANS_KAD;
		}
	} else {
		return TRANS_UNKNOW;
	}
}

int constraint_type (Translation * tls, size_t cn, long * covert) {
		
}
