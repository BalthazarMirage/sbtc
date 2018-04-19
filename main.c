#include "lib/domex.h"

#include <stdlib.h>
#include <stdio.h>




int main (int argc, char * argv[]) {
	FILE * src = fopen (argv[1], "r");
	FILE * dst = fopen (argv[2], "w");
	
	if (src == NULL || dst == NULL) {
		fclose (src);
		fclose (dst);
		return -1;
	}
	
	domex_explicit (src, dst);
	
	fclose (src);
	fclose (dst);
	
	return 0;
}
