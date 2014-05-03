/*
 * This is the last homework of the course, I have to make comments pretty!
 */
#include "hw7.h"

/*
 * traildivision to test if a number is prime
 * return 1 if passed test
 * return 0 if its a composite
 * return -1 if passed test because there is not enought numbers in the inputfile
 */
int traildiv ( char* target, char* file_input )
{
	FILE *prime_file;
	prime_file = fileopen ( file_input );
	unsigned int* small_prime = malloc ( sizeof ( unsigned int ) );

	BN_CTX *bn_ctx = NULL;
	BIGNUM *bn_target = NULL;
	BIGNUM *bn_maxval = NULL;
	BIGNUM *bn_sPrime = NULL;
	BIGNUM* bn_rem = NULL;
	bn_maxval = BN_new();
	bn_sPrime = BN_new();
	bn_rem = BN_new();
	bn_target = BN_new();
	bn_ctx = BN_CTX_new();

	if ( bn_maxval == NULL || bn_sPrime == NULL || bn_rem == NULL || bn_target == NULL ) {
		printerror ( "initialize BN error" );
	}

	if ( bn_ctx == NULL ) {
		printerror ( "ERR: CTX initialize error" );
	}
	BN_CTX_init ( bn_ctx );

	if ( fread ( small_prime, sizeof ( unsigned int ), 1, prime_file ) != 1 ) {
		printerror ( "ERR: read primefile maxval error" );
	}
	if ( BN_bin2bn ( ( unsigned char * ) ( small_prime ), sizeof ( unsigned int ), bn_maxval ) == NULL ) {
		printerror ( "ERR: maxval -> BN error" );
	}
	allNum(target);
	BN_dec2bn ( &bn_target, target );

	//printf ( "The num is %s.\n", BN_bn2dec ( bn_target ) );

	//read maxval of input file, convert back to system endiness and print
	//fprintf(stdout,"%u\n",ntohl(*small_prime));

	//begin trail division
	while ( fread ( small_prime, sizeof ( unsigned int ), 1, prime_file ) == 1 ) {
		if ( BN_bin2bn ( ( unsigned char * ) ( small_prime ), sizeof ( unsigned int ), bn_sPrime ) == NULL ) {
			printerror ( "ERR: small prime -> BN error" );
		}
		if ( !BN_mod ( bn_rem, bn_target, bn_sPrime, bn_ctx ) ) {
			printerror ( "ERR: BN_mod error" );
		}
		//read every number in file and compare, ignore target itself
		if ( BN_is_zero ( bn_rem ) && BN_cmp ( bn_target, bn_sPrime ) != 0 ) {
			fprintf ( stdout, "n is composite by trial division (mod %s = 0)\n", BN_bn2dec ( bn_sPrime ) );
			BN_free ( bn_target );
			BN_free ( bn_maxval );
			BN_free ( bn_sPrime );
			BN_free ( bn_rem );
			BN_CTX_free ( bn_ctx );
			free ( small_prime );
			fclose ( prime_file );
			return 0;
		}
	}

	if ( !BN_sqr ( bn_maxval, bn_maxval, bn_ctx ) ) {
		printerror ( "ERR: BN_sqr error" );
	}
	if ( BN_cmp ( bn_maxval, bn_target ) < 0 ) {
		fprintf ( stdout, "n passes trial division test (not enough primes)\n" );
		BN_free ( bn_target );
		BN_free ( bn_maxval );
		BN_free ( bn_sPrime );
		BN_free ( bn_rem );
		BN_CTX_free ( bn_ctx );
		free ( small_prime );
		fclose ( prime_file );
		return -1;
	} else {
		fprintf ( stdout, "n passes trial division test\n" );
		BN_free ( bn_target );
		BN_free ( bn_maxval );
		BN_free ( bn_sPrime );
		BN_free ( bn_rem );
		BN_CTX_free ( bn_ctx );
		free ( small_prime );
		fclose ( prime_file );
		return 1;
	}

}
