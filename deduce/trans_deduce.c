#include "trans_deduce.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
	size_t nexts[2];
	size_t mark;
} Node;

typedef struct {
	size_t num_clauses;
	size_t * vars;
	Node * nodes;
} Filter;

enum {
	CONSTRAINT,
	DOMAIN,
	EXCLUSION
};


void domfil_init (Filter * fl, Cnf * src) {
	fl->vars = malloc ( 
		sizeof (size_t) * src->num_vars +
		sizeof (Node) * src->num_clauses
	);
	
	fl->nodes = (Node *) (fl->vars + src->num_vars);
	fl->num_clauses = src->num_clauses;
	
	for (size_t i = 0; i < src->num_vars; i++) {
		fl->vars[i] = 2 * fl->num_clauses;
	}
}


void domfil_add (Filter * fl, Cnf * src, size_t cl) {
	int v0 = cnf_get_clause_begin(src, cl)[0];
	int v1 = cnf_get_clause_begin(src, cl)[1];
	
	fl->nodes[cl].nexts[0] = fl->vars[abs(v0)-1];
	fl->nodes[cl].nexts[1] = fl->vars[abs(v1)-1];
	fl->nodes[cl].mark = (size_t)(-1);
	
	fl->vars[abs(v0)-1] = 2 * cl;
	fl->vars[abs(v1)-1] = 2 * cl + 1;
}

void domfil_test (Filter * fl, Cnf * src, size_t cl, unsigned char * cls_role) {
	int * beg = cnf_get_clause_begin(src, cl);
	int * end = cnf_get_clause_end(src, cl);
	
	size_t num_exc = 0;
	size_t exclus[fl->num_clauses];
	size_t num_ok = (cnf_get_clause_size(src, cl) * (cnf_get_clause_size(src, cl)-1))/2;
	
	for (int * v = beg; v != end; v++) {
		size_t idx = fl->vars[abs(*v)-1];
		size_t li;
		
		while (1) {
			if (idx == 2 * fl->num_clauses) break;
			li = idx%2;
			idx = idx/2;
			
			if (fl->nodes[idx].mark == cl) {
				exclus[num_exc] = idx;
				num_exc++;
			} else {
				fl->nodes[idx].mark = cl;
			}
			
			idx = fl->nodes[idx].nexts[li];
		}
	}
	
	if (num_exc >= num_ok) {
		cls_role[cl] = DOMAIN;
		for (size_t i = 0; i < num_exc; i++)
			cls_role[exclus[i]] = EXCLUSION;
	}
}


void domfil_empty (Filter * fl) {
	free (fl->vars);
}




void translation_deduce (Cnf * src, Translation * dst) {
	dst->num_vars = src->num_vars;
	dst->num_doms = 0;
	dst->num_cons = 0;
	
	dst->doms = malloc (sizeof(size_t) * src->num_clauses);
	dst->cons = dst->doms;
	dst->doms[0] = 0;
	
	dst->datas = malloc (sizeof(int) * src->clauses[src->num_clauses]);
	
	Filter filter;
	domfil_init (&filter, src);
	
	for (size_t cl = 0; cl < src->num_clauses; cl++) {
		if (cnf_get_clause_size(src, cl) != 2) continue;
		
		domfil_add (&filter, src, cl);
	}
	
	unsigned char cls_role[ src->num_clauses ];
	
	for (size_t cl = 0; cl < src->num_clauses; cl++) {
		cls_role[cl] = CONSTRAINT;
		
		if (cnf_get_clause_size(src, cl) == 2) continue;
		
		domfil_test (&filter, src, cl, cls_role);
		
		if (cls_role[cl] == DOMAIN) {
			memcpy (
				dst->datas + dst->doms[dst->num_doms], 
				src->datas + src->clauses[cl], 
				cnf_get_clause_size(src, cl) * sizeof (int)
			);
			dst->num_doms++;
			dst->doms[dst->num_doms] = dst->doms[dst->num_doms-1] + cnf_get_clause_size(src, cl);
		}
	}
	
	dst->cons = dst->doms + dst->num_doms;
	
	for (size_t cl = 0; cl < src->num_clauses; cl++) {
		if (cls_role[cl] == CONSTRAINT) {
			memcpy (
				dst->datas + dst->cons[dst->num_cons],
				src->datas + src->clauses[cl],
				cnf_get_clause_size(src, cl) * sizeof (int)
			);
			dst->num_cons++;
			dst->cons[dst->num_cons] = dst->cons[dst->num_cons-1] + cnf_get_clause_size(src, cl);
		}
	}
	
	domfil_empty (&filter);
	
	dst->doms = realloc (dst->doms, sizeof (size_t) * dst->num_doms + dst->num_cons);
	dst->datas = realloc (dst->datas, sizeof(int) * dst->cons[dst->num_cons]);  
}

void translation_empty (Translation * tls) {
	free (tls->doms);
	free (tls->datas);
}
