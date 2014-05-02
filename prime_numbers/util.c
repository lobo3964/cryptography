#include "hw7.h"
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

//convert input char into hex value instead of ASCII value
int charToHex ( char c )
{
	if ( c >= '0' && c <= '9' ) {
		return c - '0';
	} else {
		return c - 'a' + 10;
	}
}
