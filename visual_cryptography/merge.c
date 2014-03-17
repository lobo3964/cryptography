/*
 * vis-cryptography: merge
 * by Jie Gu
 * Feb 11,2014
 * Error checking:	input files have same size info in the header
 * 						input files have AT LEAST same Bytes of data
 *
 */

#include "hw2.h"

void mergeCheck ( FILE *fp1, FILE *fp2 , int *w, int *h )
{
	int width1, width2, height1, height2;
	pbmVerify ( fp1, &width1, &height1 );
	pbmVerify ( fp2, &width2, &height2 );
	if ( ( width1 == width2 ) && ( height1 == height2 ) ) {
		memcpy ( w, &width1, sizeof ( int ) );
		memcpy ( h, &height1, sizeof ( int ) );
	} else {
		printerror ( "file1 and file2 dimension not equal" );
	}
}
/*
 * merge two
 */
int merge ( char* file1, char* file2 )
{
	int height, width, hc, wc;
	FILE* fp1 = fileopen ( file1 );
	FILE* fp2 = fileopen ( file2 );

	mergeCheck ( fp1, fp2, &width, &height );
	int in_widthByte = ( width % 8 > 0 ) ? ( width / 8 + 1 ) :  ( width / 8 );
	int pbm_len = height * in_widthByte;
	unsigned char *in_pbm1 = malloc ( pbm_len );
	unsigned char *in_pbm1_ptr = in_pbm1;
	unsigned char *in_pbm2 = malloc ( pbm_len );
	unsigned char *in_pbm2_ptr = in_pbm2;
	unsigned char in_byte1;
	unsigned char in_byte2;
	unsigned char *mergerow = malloc ( in_widthByte );
	if ( fread ( in_pbm1, sizeof ( unsigned char ), pbm_len, fp1 ) == pbm_len && fread ( in_pbm2, sizeof ( unsigned char ), pbm_len, fp2 ) == pbm_len ) {
		fclose ( fp1 );
		fclose ( fp2 );
	} else {
		printerror ( "input file(s) format error" );
	}

	//print header to stdout
	pbmheader ( stdout, width, height );

	for ( hc = 0; hc < height; hc++ ) {
		for ( wc = 0; wc < in_widthByte; wc++ ) {
			in_byte1 = in_pbm1_ptr++[0];
			in_byte2 = in_pbm2_ptr++[0];
			mergerow[wc] = in_byte1 | in_byte2;
		}
//		// print hex for debug
//		int i;
//		for(i=0;i<in_widthByte;i++){
//			fprintf(stdout,"%02X ",mergerow[i]);
//		}

		if ( fwrite ( mergerow, sizeof ( unsigned char ), in_widthByte, stdout ) != in_widthByte ) {
			printerror ( "error write row" );
		}
	}
	//fprintf(stdout,"\n");
	free ( in_pbm1 );
	free ( in_pbm2 );
	free ( mergerow );
	return 0;
}
