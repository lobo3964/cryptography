#include "hw6.h"

unsigned char xtime ( unsigned char b )
{
	if ( ( b & 0x80 ) == 0 ) {
		return ( b << 1 );
	} else {
		return ( ( ( b << 1 ) ^ m_x ) & 0xFF );
	}
}

//big dot operation
unsigned char bd ( unsigned char a, unsigned char b )
{
	unsigned char V[8];
	unsigned char sum = 0;
	int i;
	V[0] = a;
	for ( i = 1; i < 8; i++ ) { //fill up V array
		V[i] = xtime ( V[i - 1] );
	}
	for ( i = 0; i < 8; i++ ) {
		if ( ( ( b >> i ) & 0x1 ) == 0x1 ) {
			sum ^= V[i];
		}
	}
	return sum;
}
//index are in reverse order compare to EQ 4.12 in FIPS-197
void modprod ( unsigned char a[], unsigned char b[], unsigned char res[] )
{
	//d[3]
	res[0] = bd ( a[0], b[3] ) ^  bd ( a[1], b[2] ) ^ bd ( a[2], b[1] ) ^ bd ( a[3], b[0] );
	//d[2]
	res[1] = bd ( a[1], b[3] ) ^  bd ( a[2], b[2] ) ^ bd ( a[3], b[1] ) ^ bd ( a[0], b[0] );
	//d[1]
	res[2] = bd ( a[2], b[3] ) ^  bd ( a[3], b[2] ) ^ bd ( a[0], b[1] ) ^ bd ( a[1], b[0] );
	//d[0]
	res[3] = bd ( a[3], b[3] ) ^  bd ( a[0], b[2] ) ^ bd ( a[1], b[1] ) ^ bd ( a[2], b[0] );
}

void printresult ( unsigned char a[], unsigned char b[], unsigned char result[] )
{
	int i;
	for ( i = 0; i < 4; i++ ) {
		fprintf ( stdout, "{%02x}", a[i] );
	}
	fprintf ( stdout, " CIRCLEX " );
	for ( i = 0; i < 4; i++ ) {
		fprintf ( stdout, "{%02x}", b[i] );
	}
	fprintf ( stdout, " = " );
	for ( i = 0; i < 4; i++ ) {
		fprintf ( stdout, "{%02x}", result[i] );
	}
	fprintf ( stdout, "\n" );

}

void strToHex ( char p1[8], unsigned char h1[4], char p2[8], unsigned char h2[4] )
{
	int i;
	for ( i = 0; i < 4; i++ ) {
		h1[i] = ( charToHex ( p1 [i * 2] ) << 4 ) | charToHex ( p1 [i * 2 + 1] );
	}
	for ( i = 0; i < 4; i++ ) {
		h2[i] = ( charToHex ( p2 [i * 2] ) << 4 ) | charToHex ( p2 [i * 2 + 1] );
	}
}

//two order of 8 inputs
int modproduct ( char p1[8], char p2[8] )
{
	unsigned char a[4];
	unsigned char b[4];
	//int i;

	unsigned char *result = malloc ( sizeof ( unsigned char ) * 4 );

	strToHex ( p1, a, p2, b );
	modprod ( a, b, result );
	printresult ( a, b, result );
	free ( result );
	return 0;
}
