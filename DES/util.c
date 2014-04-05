#include "hw5.h"
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

/**
 * Checks the validity of input key
 */
int keyCheck ( char* key )
{
	if ( strncmp ( "0101010101010101", key, 16 ) == 0 ) {
		return WEAK_KEY;
	} else if ( strncmp ( "fefefefefefefefe", key, 16 ) == 0 ) {
		return WEAK_KEY;
	} else if ( strncmp ( "1f1f1f1f0e0e0e0e", key, 16 ) == 0 ) {
		return WEAK_KEY;
	} else if ( strncmp ( "e0e0e0e0f1f1f1f1", key, 16 ) == 0 ) {
		return WEAK_KEY;
	} else if ( strncmp ( "011f011f010e010e", key, 16 ) == 0 ) {
		return SEMI_WEAK_KEY;
	} else if ( strncmp ( "1f011f010e010e01", key, 16 ) == 0 ) {
		return SEMI_WEAK_KEY;
	} else if ( strncmp ( "01e001e001f101f1", key, 16 ) == 0 ) {
		return SEMI_WEAK_KEY;
	} else if ( strncmp ( "e001e001f101f101", key, 16 ) == 0 ) {
		return SEMI_WEAK_KEY;
	} else if ( strncmp ( "01fe01fe01fe01fe", key, 16 ) == 0 ) {
		return SEMI_WEAK_KEY;
	} else if ( strncmp ( "fe01fe01fe01fe01", key, 16 ) == 0 ) {
		return SEMI_WEAK_KEY;
	} else if ( strncmp ( "1fe01fe00ef10ef1", key, 16 ) == 0 ) {
		return SEMI_WEAK_KEY;
	} else if ( strncmp ( "e01fe01ff10ef10e", key, 16 ) == 0 ) {
		return SEMI_WEAK_KEY;
	} else if ( strncmp ( "1ffe1ffe0efe0efe", key, 16 ) == 0 ) {
		return SEMI_WEAK_KEY;
	} else if ( strncmp ( "fe1ffe1ffe0efe0e", key, 16 ) == 0 ) {
		return SEMI_WEAK_KEY;
	} else if ( strncmp ( "e0fee0fef1fef1fe", key, 16 ) == 0 ) {
		return SEMI_WEAK_KEY;
	} else if ( strncmp ( "fee0fee0fef1fef1", key, 16 ) == 0 ) {
		return SEMI_WEAK_KEY;
	} else {
		return GOOD_KEY;
	}
}
