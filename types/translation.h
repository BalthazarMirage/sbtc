#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <stddef.h>

enum {
	TRANS_UNKNOW,		// the method of translation is not know
	TRANS_IMPOSSIBLE,	// this translation is impossible to obtain
	TRANS_DIRECT,		// translation used the direct method
	TRANS_SUPPORT,		// translation used the support method
	TRANS_KAD			// translation used the k-ad method
};

typedef struct {
	size_t num_vars, num_doms, num_cons;
	
	size_t * doms;
	size_t * cons;
	
	int * datas;
} Translation;

size_t translation_get_num_vars (Translation * tls);
size_t translation_get_num_domains (Translation * tls);
size_t translation_get_num_constraints (Translation * tls);

size_t translation_get_domain_size (Translation * tls, size_t dom);
int * translation_get_domain_begin (Translation * tls, size_t dom);
int * translation_get_domain_end (Translation * tls, size_t dom);

size_t translation_get_constraint_size (Translation * tls, size_t con);
int * translation_get_constraint_begin (Translation * tls, size_t con);
int * translation_get_constraint_end (Translation * tls, size_t con);

#endif
