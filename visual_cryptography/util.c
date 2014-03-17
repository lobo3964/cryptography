#include "hw2.h"
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

/*
 * verify the format of pbm, check magicnum
 * did not implement strict format checking according
 * to pbm file description
 */
void pbmVerify ( FILE * pbm_fp, int *width, int *height )
{
	char magicnum[10] = "";
	fgets ( magicnum, 10, pbm_fp );
	int w = 0;
	int h = 0;
	if ( strcmp ( magicnum, "P4\n" ) == 0 ) {
		fscanf ( pbm_fp, "%d", &w );
		fscanf ( pbm_fp, "%d", &h );
		memcpy ( width, &w, sizeof ( int ) );
		memcpy ( height, &h, sizeof ( int ) );
		fscanf ( pbm_fp, " " );
		return;
	}
	printerror ( "pbm verify failed" );
}

/*
 * print pbm header with magicnum and size info
 */
void pbmheader ( FILE* fp, int width, int height )
{
	fprintf ( fp, "P4\n" );
	if ( ferror ( fp ) != 0 )
		printerror ( "header write failed" );
	fprintf ( fp, "%d %d\n", width, height );
	if ( ferror ( fp ) != 0 )
		printerror ( "header write failed" );
}
