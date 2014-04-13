#include "hw6.h"

void WordXOR ( unsigned char *a, unsigned char *b )
{
	a[0] = a[0] ^ b[0];
	a[1] = a[1] ^ b[1];
	a[2] = a[2] ^ b[2];
	a[3] = a[3] ^ b[3];
}
void SubWord ( unsigned char *word )
{
	word[0] = S[ ( int ) ( word[0] )];
	word[1] = S[ ( int ) ( word[1] )];
	word[2] = S[ ( int ) ( word[2] )];
	word[3] = S[ ( int ) ( word[3] )];
}

void RotWord ( unsigned char *word )
{
	unsigned char head = word[0];
	memcpy ( word, ( word + 1 ), 1 );
	memcpy ( ( word + 1 ), ( word + 2 ), 1 );
	memcpy ( ( word + 2 ), ( word + 3 ), 1 );
	memcpy ( ( word + 3 ), &head, 1 );
}

void KeyExpansion ( unsigned char key[16], unsigned char w[44][4] )
{
	int Nk = 4;
	unsigned char *temp = malloc ( sizeof ( unsigned char ) * 4 );
	int i = 0;
	int j = 0;
	//Rcon
	unsigned char Rcon[11][4];
	for ( i = 0; i < 11; i++ ) {
		for ( j = 0; j < 4; j++ ) {
			Rcon[i][j] = 0x0;
		}
	}
	Rcon[1][0] = 0x1;
	for ( i = 2; i < 11; i++ ) {
		Rcon[i][0] = bd ( Rcon[i - 1][0], 0x2 );
	}


	for ( i = 0; i < Nk ; i++ ) {
		for ( j = 0; j < 4; j++ ) {
			//word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3])
			w[i][j] = key[4 * i + j];
		}
	}


	for ( i = Nk; i < 44; i++ ) {
		memcpy ( temp, w[i - 1], 4 );
		if ( i % Nk == 0 ) {
			RotWord ( temp );
			SubWord ( temp );
			WordXOR ( temp, Rcon[i / Nk] );
		}
		//w[i] = w[i - Nk] xor temp;
		WordXOR ( temp, w[i - Nk] );
		memcpy ( w[i], temp, 4 );
	}
	free ( temp );
}

void PrintKey ( unsigned char w[44][4] )
{
	int i = 0;

	for ( i = 0; i < 44; i++ ) {
		fprintf ( stdout, "w[%2d]: %02x%02x%02x%02x\n", i, w[i][0], w[i][1], w[i][2], w[i][3] );
	}
}

int KeyExp ( char *k, char *table_in , int print )
{
	unsigned char w[44][4];
	unsigned char key[16];
	int i;
	unsigned char val;
	char *ptr;
	if ( tablecheck ( table_in ) != 0 ) {
		printerror ( "tablecheck failed" );
	}

	//convert hexstring to hex key
	ptr = k;
	for ( i = 0; i < 16; i++ ) {
		char a = ( ptr++ ) [0];
		char b = ( ptr++ ) [0];

		if ( isxdigit ( a ) == 0 || isxdigit ( b ) == 0 ) {
			printerror ( "invalid key" );
		}
		val = ( charToHex ( a ) << 4 ) | charToHex ( b );
		key[i] = val;
	}
	KeyExpansion ( key, w );

	if ( print == YES ) {
		PrintKey(w);
	}

	return 0;
}
