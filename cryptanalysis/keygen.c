/*
 * Cryptanalysis-random key generation
 * 'randomly' generate a key for Vigenère cipher
 * not a true random key generation, used system time for seed
 *
 * by Jie Gu
 * Feb 26, 2014
 */
#include "hw4.h"

int newseed()
{
	struct timeval time;
	gettimeofday ( &time, NULL );
	int p1 = ( int ) time.tv_sec;
	int p2 = ( int ) time.tv_usec;
	return p1 % p2;
}
int keygen ( int t )
{
	const char dict[] = "abcdefghijklmnopqrstuvwxyz";
	int i = 0;
	int index = 0;
	char newline[KEYGEN_SPACE_SIZE];
	strcpy ( newline, dict );
	for ( i = 0; i < t; i++ ) {

		int pos = 0;
		int temp = 0;
		for ( index = 25; index > 1; index-- ) {
			srand ( newseed() );
			pos = rand() % ( index - 1 );
			temp = newline[index];
			newline[index] = newline[pos];
			newline[pos] = temp;
		}
		fprintf ( stdout, "%s\n", newline );
	}
	return 0;
}
