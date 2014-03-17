/*
 * base64 encoding and decoding function
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hw1.h"
static const char B64TB[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
 * 1. encode mode:
 * 		treat all '\n' as binary until reaches the EOF,
 * 		where fgetc==-1 and returns. Therefore EOF is not encoded.
 *
 * 2. decode mode:
 * 		ignore '\n', if reading characters outside of base64 table,
 * 		return -1. Else return characters read.
 */
int b64read ( unsigned char* inbuf, char mode, size_t max_count, FILE * fp )
{
	int count = 0;
	int temp = 0;
	while ( count < max_count ) {
		temp = fgetc ( fp );
		inbuf[count] = temp;
		switch ( mode ) {
		case 'e':
			if ( temp >= 0 ) {
				count++;
			} else {
				inbuf[count] = 0; //reset the EOF value(-1) to 0
				return count;
			}
			break;
		case 'd':
			if ( temp == 0xA ) { //if meets [newline], this will be replaced by next char
				inbuf[count] = 0;
				break;
			} else if ( ( temp == 61 ) || ( temp == 43 ) || ( temp >= 47 && temp <= 57 ) || ( temp >= 65 && temp <= 90 ) || ( temp >= 97 && temp <= 122 ) ) {
				count++;
			} else if ( temp == -1 ) {
				inbuf[count] = 0;
				return count;
			} else {
				return -1;
			}
			break;
		default:
			fprintf ( stderr, "ERR: b64read invalid mode\n" );
			fclose ( fp );
			exit ( -1 );
		}
	}
	return count;
}
int b64dec ( FILE *fp )
{
	int i, n = 0;
	int padFlag = 0;	//set when padding is detected
	int space;
	unsigned char *inbuf = malloc ( MAX_B64_DEC_BUF ); //sizeof 4 to read
	unsigned char *outbuf = malloc ( MAX_B64_ENC_BUF ); //sizeof 3 to store encoded

	while ( !feof ( fp ) ) {
		memset ( inbuf, 0, MAX_B64_DEC_BUF );
		memset ( outbuf, 0, MAX_B64_ENC_BUF );
		n = b64read ( inbuf, 'd', MAX_B64_DEC_BUF, fp );
		if ( n == -1 ) {
			fprintf ( stderr, "ERR: b64dec illegal char detected\n" );
			free ( inbuf );
			free ( outbuf );
			fclose ( fp );
			exit ( -1 );
		} else if ( n == 0 ) {
			break;
		} else if ( n < 4 ) {
			fprintf ( stderr, "ERR: b64dec illegal file format\n" );
			free ( inbuf );
			free ( outbuf );
			fclose ( fp );
			exit ( -1 );
		}
		if ( ( inbuf[0] == 61 ) || ( inbuf[1] == 61 ) || ( padFlag == 1 ) ) {
			fprintf ( stderr, "ERR: b64dec illegal padding position\n" );
			free ( inbuf );
			free ( outbuf );
			fclose ( fp );
			exit ( -1 );
		}

		space = 0;
		if ( ( inbuf[2] == 61 ) && ( inbuf[3] == 61 ) ) {
			inbuf[2] = 0;
			inbuf[3] = 0;
			space = 2;
			padFlag = 1;
		} else if ( inbuf[3] == 61 ) {
			inbuf[3] = 0;
			space = 1;
		}

		for ( i = 0; i < ( MAX_B64_DEC_BUF - space ); i++ ) {
			if ( inbuf[i] >= 65 && inbuf[i] <= 90 ) {
				inbuf[i] = inbuf[i] - 65;
			} else if ( inbuf[i] >= 97 && inbuf[i] <= 122 ) {
				inbuf[i] = inbuf[i] - 71;
			} else if ( inbuf[i] >= 48 && inbuf[i] <= 57 ) {
				inbuf[i] = inbuf[i] + 4;
			} else if ( inbuf[i] == 43 ) {
				inbuf[i] = 62;
			} else if ( inbuf[i] == 47 ) {
				inbuf[i] = 63;
			}
		}

		outbuf[0] = ( ( inbuf[0] << 2 ) | ( inbuf[1] >> 4 ) );
		outbuf[1] = ( ( inbuf[1] & 15 ) << 4 ) | ( inbuf[2] >> 2 );
		outbuf[2] = ( ( inbuf[2] & 3 ) << 6 ) | inbuf[3];

		for ( i = 0; i < ( MAX_B64_ENC_BUF - space ); i++ ) {
			fprintf ( stdout, "%c", outbuf[i] );
		}
	}
	free ( inbuf );
	free ( outbuf );
	fclose ( fp );
	return 1;
}
int b64enc ( FILE *fp )
{
	int i, n = 0, total = 0;
	unsigned char *inbuf = malloc ( MAX_B64_ENC_BUF ); //sizeof 3 to read
	unsigned char *outbuf = malloc ( MAX_B64_DEC_BUF ); //sizeof 4 to store encoded

	while ( !feof ( fp ) ) {
		memset ( inbuf, 0, MAX_B64_ENC_BUF );
		memset ( outbuf, 0, MAX_B64_DEC_BUF );
		n = b64read ( inbuf, 'e', MAX_B64_ENC_BUF, fp );

		outbuf[0] = B64TB[inbuf[0] >> 2];
		outbuf[1] = B64TB[ ( ( inbuf[0] & 3 ) << 4 ) | ( inbuf[1] >> 4 )];
		outbuf[2] = B64TB[ ( ( inbuf[1] & 15 ) << 2 ) | ( inbuf[2] >> 6 )];
		outbuf[3] = B64TB[inbuf[2] & 63];

		if ( n == 1 ) {
			outbuf[2] = '=';
			outbuf[3] = '=';
			n = 4;
		} else if ( n == 2 ) {
			outbuf[3] = '=';
			n = 4;
		} else if ( n == 3 ) {
			n = 4;
		}
		for ( i = 0; i < n; i++ ) {
			fprintf ( stdout, "%c", outbuf[i] );
		}
		total = total + n;
		if ( total == MAX_B64_LEN ) {
			fprintf ( stdout, "\n" );
			total = 0;
		}
	}
	if ( total != 0 ) { //append \n to the last line
		fprintf ( stdout, "\n" );
	}
	free ( inbuf );
	free ( outbuf );
	fclose ( fp );
	return 1;
}

int b64main ( char* filepath, char* mode )
{
	if ( strcmp ( mode, "enc" ) == 0 ) {
		if ( b64enc ( fileopen ( filepath ) ) == 1 ) {
			return 1;
		} else {
			fprintf ( stderr, "ERR: b64main encode error\n" );
			exit ( -1 );
		}
	} else if ( strcmp ( mode, "dec" ) == 0 ) {
		if ( b64dec ( fileopen ( filepath ) ) == 1 ) {
			return 1;
		} else {
			fprintf ( stderr, "ERR: b64main decode error\n" );
			exit ( -1 );
		}
	} else {
		fprintf ( stderr, "ERR: b64main invalid mode\n" );
		exit ( -1 );
	}
}
