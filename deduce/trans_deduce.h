#ifndef TRANSLATION_DEDUCE_H
#define TRANSLATION_DEDUCE_H

#include "../types/cnf.h"
#include "../types/translation.h"

void translation_deduce (Cnf * src, Translation * dst);
void translation_empty (Translation * tls);

#endif
