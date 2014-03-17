#include "hw4.h"
/*
 * read from stdin(filepath==NULL) or filepath
 * return file descriptor or exit with error
 */
FILE* fileopen ( char *filepath )
{
	FILE *fp;
	if ( filepath == NULL ) {
		fp = stdin;
	} else {
		fp = fopen ( filepath, "rb" );
		if ( fp == NULL ) {
			printerror ( "file open failed" );
			exit ( 1 );
		}
	}
	return fp;
}

void printerror ( char msg[] )
{
	fprintf ( stderr, "ERR: " );
	fprintf ( stderr, "%s", msg );
	fprintf ( stderr, "\n" );
	exit ( 1 );
}
