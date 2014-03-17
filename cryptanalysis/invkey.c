/*
 * Cryptanalysis-Inversekey generation
 * calculate the inversekey of a polyalphabetic substitution
 * key
 *
 * by Jie Gu
 * Feb 26, 2014
 */
#include "hw4.h"

int invkey ( char * keyfile )
{
	const char dict[] = "abcdefghijklmnopqrstuvwxyz";
	long keysize_byte;
	int i, period, index, retval;
	FILE * key_fp = fileopen ( keyfile );
	char *current_key_line = malloc ( KEYGEN_SPACE_SIZE + 1 );
	char *inv_key_line = malloc ( KEYGEN_SPACE_SIZE );
	if ( fseek ( key_fp, 0, SEEK_END ) != 0 ) {
		printerror ( "seeking end of file failed" );
	}
	keysize_byte = ftell ( key_fp );
	if ( keysize_byte % ( KEYGEN_SPACE_SIZE + 1 ) != 0 ) {
		printerror ( "key file invalid" );
	}
	period = keysize_byte / ( KEYGEN_SPACE_SIZE + 1 );
	if ( fseek ( key_fp, 0, SEEK_SET ) != 0 ) {
		printerror ( "seeking begin of file failed" );
	}

	for ( i = 0; i < period; i++ ) {
		retval = fread ( current_key_line, sizeof ( char ), ( KEYGEN_SPACE_SIZE + 1 ), key_fp );
		if ( retval != ( KEYGEN_SPACE_SIZE + 1 ) ) {
			printerror ( "read keyfile error" );
		}
		for ( index = 0; index < KEYGEN_SPACE_SIZE; index++ ) {
			char cur = current_key_line[index];
			if ( cur < 0x20 || cur > 0x7e ) {
				printerror ( "illegal char detected" );
			}
			inv_key_line[cur - 'a'] = dict[index];
		}
		fprintf ( stdout, "%s\n", inv_key_line );
	}
	fclose ( key_fp );
	free ( current_key_line );
	free ( inv_key_line );
	return 0;
}
