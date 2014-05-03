/*
 * This is the last homework of the course, I have to make comments pretty!
 */
#include "hw7.h"

/*
 * calculate S and R by counting/shifting bits
 */
void helper_SR ( BIGNUM *num_one, BIGNUM *num, BIGNUM *s, BIGNUM *r , char* prefix)
{
	int i = 0;
	int len = 0;
	//get n-1
	BN_sub ( num_one, num, BN_value_one() );
	len = BN_num_bits ( num_one );
	if ( !BN_zero ( s ) ) {
		printerror ( "ERR: set BN_zero error" );
	}
	for ( i = 0; i < len; i++ ) {
		if ( BN_is_bit_set ( num_one, i ) ) {
			break;
		}
		//get s
		BN_add ( s, s, BN_value_one() );
	}
	//get r
	BN_rshift ( r, num_one, i );
	fprintf ( stdout, "%s  n-1 = %s\n",prefix, BN_bn2dec ( num_one ) );
	fprintf ( stdout, "%s  s = %s\n",prefix, BN_bn2dec ( s ) );
	fprintf ( stdout, "%s  r = %s\n",prefix, BN_bn2dec ( r ) );
	return;
}

int millerrabin ( char* num, int max_t, char* input_file , int AsSubRoutine )
{
	int i = 0;
	int j = 0;
	int bn_s_val = 0;
	FILE *prime_file;
	prime_file = fileopen ( input_file );
	unsigned int* small_prime = malloc ( sizeof ( unsigned int ) );

	BN_CTX *bn_ctx = NULL;
	BIGNUM *bn_r = NULL;
	BIGNUM *bn_s = NULL;
	BIGNUM *bn_target = NULL;
	BIGNUM *bn_target_one = NULL;
	BIGNUM *bn_a = NULL;
	BIGNUM *bn_y = NULL;
	bn_target = BN_new();
	bn_target_one = BN_new();
	bn_a = BN_new();
	bn_r = BN_new();
	bn_s = BN_new();
	bn_y = BN_new();
	bn_ctx = BN_CTX_new();
	if ( bn_ctx == NULL ) {
		printerror ( "ERR: CTX initialize error" );
	}
	if ( bn_target == NULL || bn_s == NULL || bn_r == NULL || bn_a == NULL || bn_y == NULL || bn_target_one == NULL ) {
		printerror ( "ERR: initialize BN error" );
	}

	char* prefix = malloc ( sizeof ( unsigned char ) * 2 );
	if ( AsSubRoutine ) {
		strcpy ( prefix, "  " );
	} else {
		strcpy ( prefix, "" );
	}
	
	allNum(num);
	BN_dec2bn ( &bn_target, num );
	
	if ( !AsSubRoutine ) {
		fprintf ( stdout, "n = %s\n", BN_bn2dec ( bn_target ) );
	}
	/*
	 * 1) write n-1 = 2sr such that r is odd
	 */
	helper_SR ( bn_target_one, bn_target, bn_s, bn_r ,prefix);
	bn_s_val = ( int ) strtol ( BN_bn2dec ( bn_s ), NULL, 10 );
	//skip maxval in prime file
	if ( fread ( small_prime, sizeof ( unsigned int ), 1, prime_file ) != 1 ) {
		printerror ( "ERR: read primefile maxval error" );
	}

	/*
	 * 2) for (i=1; i <= maxitr; i++) {
	 */
	for ( i = 1; i <= max_t; i++ ) {
		/*
		 * 2.1) a = the ith smallest prime number
		 */
		if ( fread ( small_prime, sizeof ( unsigned int ), 1, prime_file ) != 1 ) {
			printerror ( "ERR: read small prime error" );
		}
		if ( BN_bin2bn ( ( unsigned char * ) ( small_prime ), sizeof ( unsigned int ), bn_a ) == NULL ) {
			printerror ( "ERR: small prime -> BN error" );
		}
		fprintf ( stdout, "%s  Itr %d of %d, ",prefix, i, max_t );

		/*
		 * 2.1.1) if (a > n-1) return("failure")
		 */
		if ( BN_cmp ( bn_a, bn_target ) > 1 ) {
			printerror ( "ERR: small prime is larger than target" );
		}

		/*
		 * 2.2) compute y = ar mod n
		 */
		if ( BN_mod_exp ( bn_y, bn_a, bn_r, bn_target, bn_ctx ) != 1 ) {
			printerror ( "ERR: BN_mod_exp failed" );
		}
		if ( BN_cmp ( bn_y, bn_target_one ) == 0 ) {
			fprintf ( stdout, "a = %s, y = %s (which is n-1)\n", BN_bn2dec ( bn_a ) , BN_bn2dec ( bn_y ) );
		} else {
			fprintf ( stdout, "a = %s, y = %s\n", BN_bn2dec ( bn_a ) , BN_bn2dec ( bn_y ) );
		}

		/*
		 * 2.3) if (y != 1 and y != n-1)
		 */
		if ( BN_cmp ( bn_y, BN_value_one() ) != 0 && BN_cmp ( bn_y, bn_target_one ) != 0 ) {
			/*
			 * 2.3.1) for (j=1; j <= s-1 and y != n-1; j++)
			 */
			for ( j = 1; ( j <= ( bn_s_val - 1 ) ) && ( BN_cmp ( bn_y, bn_target_one ) != 0 ); j++ ) {

				/*
				 * 2.3.1.1) y = y2 mod n
				 */
				BN_mod_sqr ( bn_y, bn_y, bn_target, bn_ctx );
				if ( BN_cmp ( bn_y, bn_target_one ) == 0 ) {
					fprintf ( stdout, "%s    j = %d of %d, y = %s (which is n-1)\n",prefix, j, bn_s_val - 1, BN_bn2dec ( bn_y ) );
				} else {
					fprintf ( stdout, "%s    j = %d of %d, y = %s\n",prefix, j, bn_s_val - 1, BN_bn2dec ( bn_y ) );
				}
				/*
				 * 2.3.1.2) if (y == 1) return("composite")
				 */
				if ( BN_cmp ( bn_y, BN_value_one() ) == 0 ) {
					fprintf ( stdout, "%sMiller-Rabin found a strong witness %s\n",prefix, BN_bn2dec ( bn_a ) );
					goto notprime;
				}

			}
			/*
			 * 2.3.2) if (y != n-1) return("composite")
			 */
			if ( BN_cmp ( bn_y, bn_target_one ) != 0 ) {
				fprintf ( stdout, "%sMiller-Rabin found a strong witness %s\n",prefix, BN_bn2dec ( bn_a ) );
				goto notprime;
			}
		}
	}
	/*
	 * 3) return("prime")
	 */
	fprintf ( stdout, "%sMiller-Rabin declares n to be a prime number\n",prefix);
	goto prime;

notprime:
	BN_free ( bn_target );
	BN_free ( bn_target_one );
	BN_free ( bn_s );
	BN_free ( bn_r );
	BN_free ( bn_y );
	BN_free ( bn_a );
	BN_CTX_free ( bn_ctx );
	free ( small_prime );
	fclose ( prime_file );
	return 0;


prime:
	BN_free ( bn_target );
	BN_free ( bn_target_one );
	BN_free ( bn_s );
	BN_free ( bn_r );
	BN_free ( bn_y );
	BN_free ( bn_a );
	BN_CTX_free ( bn_ctx );
	free ( small_prime );
	fclose ( prime_file );
	return 1;
}
