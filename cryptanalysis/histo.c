/*
 * Cryptanalysis-Histogram
 * provide a printout of frequency analysis
 * of a given input file, must provide a one-based index to specify
 * which location to analyze
 *
 * by Jie Gu
 * Feb 26, 2014
 */
#include "hw4.h"

static int compare ( const void *a, const void *b )
{
	const histpair *elem1=a;
	const histpair *elem2=b;
	
	if ( elem1->count != elem2->count )
		return ( elem2->count ) - ( elem1->count );
	else if ( elem1->count == elem2->count )
		return ( elem1->lowercase ) - ( elem2->lowercase );
	else
		return 0;
}

int histo ( int period, int which, char * in_file )
{
	if (which<1){
		printerror("value of 'i' must start from 1 (one)");
	}
	if (which>period){
		printerror("index exceeds period length");
	}
	FILE* in_fp = fileopen ( in_file );
	histpair * result = malloc ( KEYGEN_SPACE_SIZE * sizeof ( histpair ) );
	char * curline = malloc ( sizeof ( char ) * period );
	int count,i;
	for ( count = 0; count < KEYGEN_SPACE_SIZE; count++ ) {
		result[count].lowercase = count + 'a';
		result[count].count = 0;
	}
	count = 0;
	while ( feof ( in_fp ) == 0 ) {
		fread ( curline, sizeof ( char ), period, in_fp );
		char cur = curline[which - 1];
		if ( cur >= 0x61 && cur <= 0x7a ) {
			count++;
			int pos = cur - 'a';
			result[pos].count++;
		}
	}
	qsort ( result, KEYGEN_SPACE_SIZE, sizeof ( histpair ), compare );
	
	fprintf(stdout,"L=%d\n",count);
	
	for (i=0;i< KEYGEN_SPACE_SIZE; i++){
		double pct=(double)result[i].count/(double)count;
		fprintf(stdout,"%c: %d (%.2f%%)\n",result[i].lowercase,result[i].count,pct*100);
	}
	fclose(in_fp);
	free(result);
	free(curline);
	return 0;
}
