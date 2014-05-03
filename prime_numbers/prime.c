/*
 * This is the last homework of the course, I have to make comments pretty!
 */
#include "hw7.h"

#define MAXLIMIT	16777216

/*
 * convert an integer in bigendian
 * and print as 4 bytes word
 */
void printWord ( int val )
{
	uint32_t word = htonl ( val );
	unsigned char* index = ( unsigned char* ) &word;
	fprintf ( stdout, "%c", index[0] );
	fprintf ( stdout, "%c", ( index + 1 ) [0] );
	fprintf ( stdout, "%c", ( index + 2 ) [0] );
	fprintf ( stdout, "%c", ( index + 3 ) [0] );
}

/*
 * return the byte position of a certain bit in a memory block
 * use posInBlock to determine which byte it belongs to
 */
int posInByte ( int bit_num )
{
	return ( bit_num % 8 );
}

/*
 * return the byte position of a certain bit in a memory block
 * use posInBlock to determine which byte it belongs to
 */
int posInBlock ( int bit_num )
{
	return ( bit_num / 8 );
}


void Eratosthenes ( int maxval )
{
	int array_size = ( maxval / 8 ) + 1;
	unsigned char *temp_array = malloc ( array_size );

	//initialized values in the array
	memset ( temp_array, 0xff, array_size );

	//sieve op
	int i = 2;
	int j = 0;
	unsigned char *byte_prt;
	for ( ; ( i * i ) <= maxval; i++ ) {
		byte_prt = temp_array + posInBlock ( i );
		if ( ( ( byte_prt[0] << posInByte ( i ) ) & 0x80 ) == 0x80 ) {
			j = i * i;
			for ( ; j <= maxval; j = j + i ) {
				byte_prt = temp_array + posInBlock ( j );
				byte_prt[0] = byte_prt[0] & ( ~ ( 0x80 >> posInByte ( j ) ) );
			}
		}
	}

	//print m+1 word
	for ( i = 2; i <= maxval; i++ ) {
		byte_prt = temp_array + posInBlock ( i );
		if ( ( ( byte_prt[0] << posInByte ( i ) ) & 0x80 ) == 0x80 ) {
			printWord ( i );
		}
	}

	free ( temp_array );
}



int prime_gen ( char* maxval )
{
	//TODO maxval size detection
	int val=0;
	allNum(maxval);
	sscanf(maxval,"%d",&val);
	if (val<2 || val>MAXLIMIT){
		printerror("out of maxval limit");
	}
	//print maxval in word
	printWord ( val );
	Eratosthenes ( val );

	return 0;
}
