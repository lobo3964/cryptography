/*
 * Cryptanalysis-Analysis report
 * provide following analysis:
 * 		Method of Kasiski
 * 		Index of Coincidence
 * 		Auto Correlation
 *
 * by Jie Gu
 * Feb 26, 2014
 */
#include "hw4.h"

int verifyLC ( char *content, int size )
{
	int i;
	for ( i = 0; i < size; i++ ) {
		if ( content[i] < 0x61 || content[i] > 0x7a ) {
			return 0;
		}
	}
	return 1;
}
int kasiski ( char * ct , int filesize )
{
	fprintf ( stdout, "Kasiski Method\n==============\n" );
	int i, j, count, found_flag;
	int window;
	char *content;
	count = 0;
	for ( window = 4; window > 0; window++ ) {
		char *head = ct;
		content = malloc ( window * sizeof ( char ) );
		found_flag = 0;
		for ( count = 0; count < ( filesize - window + 1 ); count++ ) {
			memcpy ( content, head + count, window );
			if ( verifyLC ( content, window ) == 1 ) {
				i = count;
				for ( j = i + 1; j < ( filesize - window + 1 ); j++ ) {
					if ( memcmp ( content, head + j, window ) == 0 ) {
						found_flag = 1;
						fprintf ( stdout, "len=%d, i=%d, j=%d, j-i=%d, ", window, i, j, j - i );
						int temp;
						for ( temp = 0; temp < window; temp++ ) {
							fprintf ( stdout, "%c", content[temp] );
						}
						fprintf ( stdout, "\n" );
					}
				}
			} else {
				//do nothing
			}
		}
		free ( content );
		if ( found_flag == 0 ) {
			fprintf ( stdout, "len=%d, no more matches\n\n", window );
			break;
		}
	}
	return 0;
}

int coincidence ( char * ct, int filesize, int max_t )
{
	fprintf ( stdout, "Average Index of Coincidence\n============================\n" );
	int i, count, t;
	double fi, sig, IC, kr, kp, P1, P2;
	char * head = ct;
	int * stat = malloc ( KEYGEN_SPACE_SIZE * sizeof ( int ) );
	memset ( stat, 0, KEYGEN_SPACE_SIZE * sizeof ( int ) );
	count = 0;
	for ( i = 0; i < filesize; i++ ) {
		char cur = ( head + i ) [0];
		if ( cur >= 0x61 && cur <= 0x7a ) {
			count++;
			int pos = cur - 'a';
			int temp = * ( stat + pos );
			temp++;
			stat[pos] = temp;
		}
	}
	fprintf ( stdout, "L=%d\n", count );
	sig = 0;
	for ( i = 0; i < KEYGEN_SPACE_SIZE; i++ ) {
		fprintf ( stdout, "f('%c')=%d\n", ( i + 'a' ), stat[i] );
		fi = ( double ) stat[i];
		sig = sig + fi * ( fi - 1 );
	}
	IC = sig / ( ( double ) count * ( ( double ) count - 1 ) );
	fprintf ( stdout, "IC=%.9f\n", IC );
	kr = 0.0384615384615385;
	kp = 0.0658;
	for ( t = 1; t <= max_t; t++ ) {
		P1 = kp * ( count - t ) / ( t * ( count - 1 ) );
		P2 = kr * count * ( t - 1 ) / ( t * ( count - 1 ) );
		IC = P1 + P2;
		fprintf ( stdout, "t=%d, E(IC)=%.9f\n", ( int ) t, IC );
	}
	fprintf ( stdout, "\n" );
	free(stat);
	return 0;
}

int autocorrelation ( char * ct, int filesize, int max_t )
{
	fprintf ( stdout, "Auto-correlation Method\n=======================\n" );
	int t, count, i;
	char * head = ct;
	for ( t = 1; t <= max_t; t++ ) {
		count = 0;
		for ( i = 0; i < filesize - t; i++ ) {
			char cur = ( head + i ) [0];
			if ( cur  >= 0x61 && cur <= 0x7a ) {
				char next = ( head + i + t ) [0];
				if ( next == cur ) {
					count++;
				}
			}
		}
		fprintf ( stdout, "t=%d, count=%d\n", t, count );
	}

	return 0;
}

int solve ( int max_t, char * in_file )
{
	int in_size_byte, retval;
	char *ct;
	/*
	 * preprocessing, copy ct file into memory
	 */
	FILE * in_fp = fileopen ( in_file );
	if ( fseek ( in_fp, 0, SEEK_END ) != 0 ) {
		printerror ( "seeking end of file failed" );
	}
	in_size_byte = ftell ( in_fp );
	if ( fseek ( in_fp, 0, SEEK_SET ) != 0 ) {
		printerror ( "seeking begin of file failed" );
	}
	ct = malloc ( in_size_byte );
	retval = fread ( ct, sizeof ( char ), in_size_byte, in_fp );
	if ( retval != in_size_byte ) {
		printerror ( "copy ct to mem failed" );
	}

	if ( kasiski ( ct, in_size_byte ) != 0 ) {
		printerror ( "unknown kasiski error" );
	}

	if ( coincidence ( ct, in_size_byte, max_t ) != 0 ) {
		printerror ( "unknown kasiski error" );
	}

	if ( autocorrelation ( ct, in_size_byte, max_t ) != 0 ) {
		printerror ( "unknown autocorrelation error" );
	}
	fclose ( in_fp );
	free(ct);
	return 0;

}
