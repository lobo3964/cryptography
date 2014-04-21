/*
 * Calculate the multiplicative inverse of 
 * polynomial poly with coefficients in GF(28) 
 * Showing internal states, print error msg if not found
 * 
 * By Jie Gu
 * April 17, 2014
 * 
 */
#include "hw6.h"

char *mi;

void miInit()
{
	mi = malloc ( sizeof ( char ) * 512 );
	mi = "00018df6cb527bd1e84f29c0b0e1e5c7\
74b4aa4b992b605f583ffdccff40eeb2\
3a6e5af1554da8c9c10a98153044a2c2\
2c45926cf3396642f235206f77bb5919\
1dfe37672d31f569a764ab135425e909\
ed5c05ca4c2487bf183e22f051ec6117\
165eafd349a63643f44791df3393213b\
79b7978510b5ba3cb670d006a1fa8182\
837e7f809673be569b9e95d9f702b9a4\
de6a326dd88a84722a149f88f9dc899a\
fb7c2ec38fb8654826c8124acee7d262\
0ce01fef11757871a58e763dbdbc8657\
0b282fa3dad4e40fa92753041bfcace6\
7a07ae63c5dbe2ea948bc4d59df8906b\
b10dd6ebc60ecfad084ed7e35d501eb3\
5b2338346846038cdd9c7da0cd1a411c";
}

int miLookup ( int index )
{
	unsigned char a = mi[index * 2];
	unsigned char b = mi[index * 2 + 1];
	int inv;
	inv = ( charToHex ( a ) << 4 ) | charToHex ( b );
	return inv;
}

void ToHex ( char *p1, unsigned char h1[5] )
{
	int i;
	h1[0] = 0x0;
	
	
	//copy
	for ( i = 0; i < 4; i++ ) {
		if ( isxdigit(p1 [i * 2])==0 || isxdigit(p1 [i * 2+1])==0){
			printerror("invalid poly");
		}
		h1[i + 1] = ( charToHex ( p1 [i * 2] ) << 4 ) | charToHex ( p1 [i * 2 + 1] );
	}
	if (p1[8]!=0){
		printerror("invalid poly");
	}
}

void longdivision ( unsigned char a[5], unsigned char b[5], unsigned char dest[5], unsigned char quo[4], unsigned char x[6][4], int round )
{
	int i, j;
	int index_a;
	int index_b;
	unsigned char inv;
	unsigned char prod[4] = {0, 0, 0, 0};
	unsigned char temp[4] = {0, 0, 0, 0};
	unsigned char tempx[4] = {0, 0, 0, 0};
	//first longdivision: {01}{00}{00}{00}{01}
	if ( a[0] == 1 ) {
		quo[0] = 0;
		quo[1] = 0;
		inv = miLookup ( b[1] );
		for ( i = 0; i < 3; i++ ) {
			prod[i] = bd ( inv, b[i + 2] );
		}
		prod[3] = a[4];
		quo[2] = inv;
		inv = bd ( inv, prod[0] );
		quo[3] = inv;
		for ( i = 0; i < 4; i++ ) {
			temp[i] = bd ( inv, b[i + 1] );
		}
		//prod-temp
		dest[0] = 0x0;
		for ( i = 0; i < 4; i++ ) {
			dest[i + 1] = prod[i] ^ temp[i];
		}
		//calculate x
		modprod ( x[round], quo, tempx );
		for ( i = 0; i < 4; i++ ) {
			x[round + 1][i] = x[round - 1][i] ^ tempx[i];
		}
	} else {
		quo[0] = 0;
		quo[1] = 0;
		//determine the length of dividend and divisor
		for ( index_a = 0; index_a < 5; index_a++ ) {
			if ( a[index_a] != 0 ) {
				break;
			}
		}
		for ( index_b = 0; index_b < 5; index_b++ ) {
			if ( b[index_b] != 0x0 ) {
				break;
			}
		}
		inv = miLookup ( b[index_b] );
		quo[2] = bd ( inv, a[index_a] );

		//dynamically adjusting the product cycle using {5-index_b}
		j = 0;
		for ( i = index_a - 1; i < 4 - index_b + index_a; i++, j++ ) {
			prod[i] = bd ( quo[2], b[index_b + j] );
		}

		//minus 1st time
		j = 0;
		for ( i = index_a - 1; i < 4 - index_b + index_a; i++, j++ ) {
			temp[i] = a[index_a + j] ^ prod[i];
		}
		temp[3] = a[4];
		quo[3] = bd ( inv, temp[index_a] );

		//minus 2nd time
		for ( i = 0; i < 4; i++ ) {
			prod[i] = bd ( quo[3], b[i + 1] );
		}

		//get remainder
		dest[0] = 0x0;
		for ( i = 0; i < 4; i++ ) {
			dest[i + 1] = prod[i] ^ temp[i];
		}

		//calculate x
		modprod ( x[round], quo, tempx );
		for ( i = 0; i < 4; i++ ) {
			x[round + 1][i] = x[round - 1][i] ^ tempx[i];
		}
	}
}

void lastdivision ( unsigned char a[5], unsigned char b[5], unsigned char quo[4], unsigned char x[6][4] )
{

	int i;
	unsigned char temp;
	unsigned char inv;
	unsigned char tempx[4] = {0, 0, 0, 0};

	//some checking, not sure if its necessary
	if ( a[0] != 0 || a[1] != 0 || a[2] != 0 || b[0] != 0 || b[1] != 0 || b[2] != 0 || b[3] != 0 ) {
		printerror ( "last devision check failed(1)" );
	}
	if ( a[3] == 0 || a[4] == 0 || b[4] == 0 ) {
		printerror ( "last devision check failed(2)" );
	}

	inv = miLookup ( b[4] );
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = bd ( inv, a[3] );

	//check point
	if ( ( bd ( quo[2], b[4] ) ^ a[3] ) != 0 ) {
		printerror ( "last devision check failed(3)" );
	}
	temp = ( a[4] ^ 0x1 );//
	quo[3] = bd ( inv, temp );

	//check point
	if ( ( bd ( quo[3], b[4] ) ^ a[4] ) != 0x1 ) {
		//printerror ( "last devision check failed(4)" );
	}
	//calculate X
	modprod ( x[4], quo, tempx );
	for ( i = 0; i < 4; i++ ) {
		x[5][i] = x[3][i] ^ tempx[i];
	}
}

void printheader ( unsigned char rem[5] )
{
	fprintf ( stdout, "i=1, rem[i]={00}{00}{00}{01}, quo[i]={00}{00}{00}{00}, aux[i]={00}{00}{00}{00}\n" );
	fprintf ( stdout, "i=2, rem[i]={%02x}{%02x}{%02x}{%02x}, quo[i]={00}{00}{00}{00}, aux[i]={00}{00}{00}{01}\n", rem[1], rem[2], rem[3], rem[4] );
}

void printfooter ( unsigned char a[5], unsigned char quo[4], unsigned char x[4] )
{
	fprintf (stdout, "i=6, rem[i]={00}{00}{00}{01}, ");
	fprintf ( stdout, "quo[i]={%02x}{%02x}{%02x}{%02x}, ", quo[0], quo[1], quo[2], quo[3] );
	fprintf ( stdout, "aux[i]={%02x}{%02x}{%02x}{%02x}\n", x[0], x[1], x[2], x[3] );
	fprintf ( stdout, "Multiplicative inverse of {%02x}{%02x}{%02x}{%02x} is ", a[1], a[2], a[3], a[4] );
	fprintf ( stdout, "{%02x}{%02x}{%02x}{%02x}\n", x[0], x[1], x[2], x[3] );
}

//print corresponding line and also perform validity check, return 1 if pass, -1 otherwise
int printline ( unsigned char rem[5], unsigned char quo[4], unsigned char x[4], int i )
{
	fprintf ( stdout, "i=%d, ", i );
	fprintf ( stdout, "rem[i]={%02x}{%02x}{%02x}{%02x}, ", rem[1], rem[2], rem[3], rem[4] );
	fprintf ( stdout, "quo[i]={%02x}{%02x}{%02x}{%02x}, ", quo[0], quo[1], quo[2], quo[3] );
	fprintf ( stdout, "aux[i]={%02x}{%02x}{%02x}{%02x}\n", x[0], x[1], x[2], x[3] );

	//check rem
	if ( rem[i - 1] == 0x0 ) {
		return -1;
	}
	return 1;
}

void printNotFound ( unsigned char a[5] )
{
	fprintf ( stdout, "{%02x}{%02x}{%02x}{%02x} does not have a multiplicative inverse.\n", a[1], a[2], a[3], a[4] );
}

int inverse ( char *poly )
{
	//initialize everything!!
	miInit();
	unsigned char a[5];
	unsigned char rem[6][5];
	unsigned char quo[6][4];
	unsigned char x[6][4];
	unsigned char x0[5] = {0x0, 0x0, 0x0, 0x0};
	unsigned char x1[5] = {0x0, 0x0, 0x0, 0x1};
	unsigned char MX[5] = {0x1, 0x0, 0x0, 0x0, 0x1};
	ToHex ( poly, a );
	memcpy ( rem[0], MX, 5 );
	memcpy ( rem[1], a, 5 );
	memcpy ( x[0], x0, 4 );
	memcpy ( x[1], x1, 4 );

	//actual shit start
	printheader ( a ); //i=1,i=2

	//round 1; i=3
	longdivision ( rem[0], rem[1], rem[2], quo[2], x, 1 );
	if ( printline( rem[2], quo[2], x[2], 3 )!=1 ) {
		printNotFound ( a );
		return 0;
	}

	//round 2; i=4
	longdivision ( rem[1], rem[2], rem[3], quo[3], x, 2 );
	if ( printline ( rem[3], quo[3], x[3], 4 )!=1) {
		printNotFound ( a );
		return 0;
	}

	//round 3; i=5
	longdivision ( rem[2], rem[3], rem[4], quo[4], x, 3 );
	if ( printline ( rem[4], quo[4], x[4], 5 ) !=1) {
		printNotFound ( a );
		return 0;
	}

	//round 4: remainder must be {00}{00}{00}{01}; i=6
	lastdivision ( rem[3], rem[4], quo[5], x );
	printfooter ( a, quo[5], x[5] );
	return 0;
}
