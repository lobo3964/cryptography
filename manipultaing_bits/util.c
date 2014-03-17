#include <stdio.h>
#include <stdlib.h>
#include "hw1.h"
/*
 * read from stdin(filepath==NULL) or filepath
 * return file descriptor or exit with error
 */
FILE* fileopen ( char *filepath )
{
	FILE *fp;
	if ( filepath==NULL ) {
		fp=stdin;
	} else {
		fp=fopen ( filepath, "rb" );
		if ( fp==NULL ) {
			fprintf ( stderr, "ERR: file open failed\n" );
			exit ( 1 );
		}
	}
	return fp;
}
