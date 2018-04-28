#include "translation.h"

size_t translation_get_num_vars (Translation * tls) {
	return tls->num_vars;	
}

size_t translation_get_num_domains (Translation * tls) {
	return tls->num_doms;
}

size_t translation_get_num_constraints (Translation * tls) {
	return tls->num_cons;
}


size_t translation_get_domain_size (Translation * tls, size_t dom) {
	return tls->doms[dom+1] - tls->doms[dom];
}

int * translation_get_domain_begin (Translation * tls, size_t dom) {
	return tls->datas + tls->doms[dom];
}

int * translation_get_domain_end (Translation * tls, size_t dom) {
	return tls->datas + tls->doms[dom+1];
}

size_t translation_get_constraint_size (Translation * tls, size_t con) {
	return tls->cons[con+1] - tls->cons[con];
}

int * translation_get_constraint_begin (Translation * tls, size_t con) {
	return tls->datas + tls->cons[con];
}

int * translation_get_constrain_end (Translation * tls, size_t con) {
	return tls->datas + tls->cons[con+1];
}
