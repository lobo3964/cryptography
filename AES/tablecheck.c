#include "hw6.h"

#define TABLELINENUM 3
#define READLEN 600
#define CHECK_P 1
#define CHECK_IP 2

//definition of global
unsigned char S[256];
unsigned char P[4];
unsigned char IP[4];

int checkS ( char * line )
{
	char *map = malloc ( sizeof ( char ) * 256 );
	memset ( map, 0, sizeof ( char ) * 256 );
	int i = 0;
	int index = 0;
	char* ptr = line;
	while ( ptr[0] != '=' ) {
		ptr++;
	}
	ptr++;

	for ( i = 0; i < 256; i++ ) {
		char a = ( ptr++ ) [0];
		char b = ( ptr++ ) [0];

		if ( isxdigit ( a ) == 0 || isxdigit ( b ) == 0 ) {
			return 0;
		}

		index = ( charToHex ( a ) << 4 ) | charToHex ( b );
		S[i]=index;
		if ( * ( map + index ) == 1 ) {
			return 0;
		}
		* ( map + index ) = 1;
	}
	if ( ptr[0] != 0x0a ) {
		return 0;
	} else {
		for ( i = 0; i < 256; i++ ) {
			if ( * ( map + i ) != 1 ) {
				return 0;
			}
		}
	}
	free ( map );
	return 1;
}

int checkP ( char *line, int mode )
{
	int val;
	int i = 0;
	char* ptr = line;
	while ( ptr[0] != '=' ) {
		ptr++;
	}
	ptr++;

	for ( i = 0; i < 4; i++ ) {
		char a = ( ptr++ ) [0];
		char b = ( ptr++ ) [0];
		
		if ( isxdigit ( a ) == 0 || isxdigit ( b ) == 0) {
			return 0;
		}
		val = ( charToHex ( a ) << 4 ) | charToHex ( b );
		if (mode==CHECK_P){
			P[i]=val;
		}
		if (mode==CHECK_IP){
			IP[i]=val;
		}
	}
	if ( ptr[0] != 0x0a ) {
		return 0;
	}
	return 1;
}

int tablecheck ( char * tablefile )
{
	FILE* table_in = fileopen ( tablefile );
	int linecount = TABLELINENUM;
	//char *P = malloc ( sizeof ( unsigned char ) * 8 );
	//char *IP = malloc ( sizeof ( unsigned char ) * 8 );
	unsigned char *result = malloc ( sizeof ( unsigned char ) * 4 );
	//unsigned char a[4];
	//unsigned char b[4];
	while ( linecount > 0 ) {
		char* LINE = malloc ( sizeof ( char ) * READLEN );
		memset ( LINE, 0, sizeof ( char ) * READLEN );
		if ( fgets ( LINE, READLEN, table_in ) == NULL ) {
			printerror ( "read line error" );
		}
		if ( strncmp ( LINE, "S=", 2 ) == 0 ) {
			if ( checkS ( LINE ) ) {
				linecount--;
			} else {
				printerror ( "Table S error" );
			}
		} else if ( strncmp ( LINE, "P=", 2 ) == 0 ) {
			if ( checkP ( LINE , CHECK_P ) ) {
				linecount--;
			} else {
				printerror ( "Table P error" );
			}
		} else if ( strncmp ( LINE, "INVP=", 5 ) == 0 ) {
			if ( checkP ( LINE , CHECK_IP ) ) {
				linecount--;
			} else {
				printerror ( "Table IP error" );
			}
		}
		free ( LINE );
	}

	//strToHex ( P, a, IP, b );
	modprod ( P, IP, result );
	if ( ( *result != 0 ) || ( * ( result + 1 ) != 0 ) || ( * ( result + 2 ) != 0 ) || ( * ( result + 3 ) != 1 ) ) {
		printerror ( "Table IP mulit P invalid" );
	}
	fclose(table_in);
	//free(P);
	//free(IP);
	free(result);
	return 0;
}
