/*
 * This is the last homework of the course, I have to make comments pretty!
 */
#include "hw7.h"

#define round(X) (((X) >= 0) ? (int)((X)+0.5) : (int)((X)-0.5))

unsigned char RndByte ( FILE * rnd_input )
{
	unsigned char byte = 0x0;
	if ( fread ( &byte, 1, 1, rnd_input ) != 1 ) {
		printerror ( "ERR: read RndByte error" );
	}
	return byte;
}

BIGNUM * maurer ( int k, char* primefile, FILE * rnd_file , int level )
{
	int temp = 0;
	int m = 0;
	int num_bits_in_bn_q = 0;
	int num_bits_in_bn_n = 0;
	double r = 0.0;
	int i = 1;

	BIGNUM *bn_n = NULL;
	BIGNUM *bn_n_minus1 = NULL;
	BIGNUM *bn_q = NULL;
	BIGNUM *bn_I = NULL;
	BIGNUM *bn_k_minus2 = NULL;
	BIGNUM *bn_2 = NULL;

	BIGNUM *bn_a = NULL;
	BIGNUM *bn_b = NULL;
	BIGNUM *bn_d = NULL;
	BN_CTX *bn_ctx = NULL;
	bn_n = BN_new();


	fprintf ( stdout, "Maurer: level %d, k=%d\n", level, k );

	/*
	 * 1) if (k <= 20) do forever {
	 */
	while ( k <= 20 ) {
		/*
		 * 1.1) n = RndOddNum(k)
		 */
		RndOddNum ( k, rnd_file, bn_n );
		fprintf ( stdout, "  step 1.1, n = %s\n", BN_bn2dec ( bn_n ) );
		/*
		 * 1.2) if (TrialDivision(n) == "pass") return n
		 */
		fprintf ( stdout, "    " );
		if ( traildiv ( BN_bn2dec ( bn_n ), primefile ) ) {

			//BN_free ( bn_n );
			return bn_n;
			//fprintf ( stdout, "Maurer's Algorithm found an %d-bit prime:\n", k );
			//fprintf ( stdout, "  n = %s\n", BN_bn2dec ( bn_n ) );
		}
	}

	bn_n_minus1 = BN_new();
	bn_q = BN_new();
	bn_I = BN_new();
	bn_k_minus2 = BN_new();
	bn_2 = BN_new();

	bn_a = BN_new();
	bn_b = BN_new();
	bn_ctx = BN_CTX_new();
	if ( bn_ctx == NULL ) {
		printerror ( "ERR: CTX initialize error" );
	}
	if ( bn_n == NULL || bn_q == NULL || bn_I == NULL || bn_k_minus2 == NULL || bn_2 == NULL || bn_n_minus1 == NULL || bn_a == NULL || bn_b == NULL ) {
		printerror ( "ERR: initialize BN error" );
	}

	/*
	 * 2) c = 0.1, m = 20
	 * 3) B = c * k^2 (not used)
	 */
	m = 20;

	/*
	 *  4) (generate a fraction r, the size of q relative to n)
	 * 		4.1) if (k <= 2m) r = 0.5
	 */
	if ( k <= 2 * m ) {
		r = 0.5;
		fprintf ( stdout, "  step 4, r = 50%%\n" );
	} else {
		/*
		 *  4.2) if (k > 2m) do forever
		 */
		while ( k > 2 * m ) {
			/*
			 * 4.2.1) r = RndByte() / 255.0
			 */
			temp = RndByte ( rnd_file );
			r = ( double ) temp / 255.0;
			/*
			 * 4.2.2) r = 0.5 + r / 2.0
			 */
			r = 0.5 + r / 2.0;
			/*
			 * 4.2.3) if (k*(1-r) > m) break;
			 */
			if ( k * ( 1 - r ) > m ) {
				fprintf ( stdout, "  step 4: random byte = %d, r = %d%%\n", temp, round ( r * 100 ) );
				break;
			}
		}

	}

	/*
	 * 5) (recursion) q = Maurer(floor(r*k)+1)
	 *    //maurer ( int k, char* primefile, FILE * rnd_file , int level )//
	 */
	bn_q = maurer ( floor ( k * r ) + 1, primefile, rnd_file, level + 1 );

	fprintf ( stdout, "Maurer: back to level %d, k=%d, q=%s\n", level, k, BN_bn2dec ( bn_q ) );

	/*
	 * 6) num_bits_in_q = BN_num_bits(q)
	 */
	num_bits_in_bn_q = BN_num_bits ( bn_q );
	/*
	 * I = floor(2^(k-2) / q)
	 */



	if ( BN_set_word ( bn_k_minus2, ( k - 2 ) ) != 1 ) { // k-2
		printerror ( "ERR: BN_set_word error" );
	}
	//fprintf ( stdout, " %s\n", BN_bn2dec ( bn_k_minus2 ) );

	if ( BN_set_word ( bn_2, 2 ) != 1 ) { //2
		printerror ( "ERR: create bn_2 error" );
	}

	//fprintf ( stdout, " %s\n", BN_bn2dec ( bn_2 ) );
	if ( BN_exp ( bn_2, bn_2, bn_k_minus2, bn_ctx ) != 1 ) { //2^(k-2)
		printerror ( "ERR: BN_exp() error" );
	}

//bug
	//fprintf ( stdout, " %s\n", BN_bn2dec ( bn_2 ) );
	//fprintf ( stdout, " %s\n", BN_bn2dec ( bn_q ) );

	if ( BN_div ( bn_I, NULL, bn_2, bn_q, bn_ctx ) != 1 ) { // I= floor(...)
		printerror ( "ERR: BN_div() error" );
	}

	/*
	 * 7) do forever {
	 */

	while ( 1 ) {
		/*
		 *  7.1) R = RndOddNum(k+1-num_bits_in_q)
		 */
		BIGNUM *bn_R = NULL;
		bn_R = BN_new();
		RndOddNum ( k + 1 - num_bits_in_bn_q, rnd_file, bn_R );
		//fprintf ( stdout, " %s\n", BN_bn2dec ( bn_R ) );
		/*
		 * R = (R mod I) + I + 1
		 */
		if ( BN_mod ( bn_R, bn_R, bn_I, bn_ctx ) != 1 ) {
			printerror ( "ERR: R = (R mod I) + I + 1 (a)" );
		}
		if ( BN_add ( bn_R, bn_R, bn_I ) != 1 ) {
			printerror ( "ERR: R = (R mod I) + I + 1 (b)" );
		}
		if ( BN_add ( bn_R, bn_R, BN_value_one() ) != 1 ) {
			printerror ( "ERR: R = (R mod I) + I + 1 (c)" );
		}
		/*
		 * n = 2Rq+1
		 */
		if ( BN_mul ( bn_n, bn_R, bn_q, bn_ctx ) != 1 ) {
			printerror ( "ERR: n = 2Rq+1 (a)" );
		}
		//fprintf ( stdout, " %s\n", BN_bn2dec ( bn_n ) );
		
		BN_set_word ( bn_2, 2 );//set bn_2 back to two
		if ( BN_mul ( bn_n, bn_n, bn_2, bn_ctx ) != 1 ) {
			printerror ( "ERR: n = 2Rq+1 (b)" );
		}
		
		//fprintf ( stdout, " %s\n", BN_bn2dec ( bn_2 ) );
		//fprintf ( stdout, " %s\n", BN_bn2dec ( bn_n ) );
		if ( BN_add ( bn_n, bn_n, BN_value_one() ) != 1 ) {
			printerror ( "ERR: n = 2Rq+1 (c)" );
		}
		//fprintf ( stdout, " %s\n", BN_bn2dec ( bn_n ) );
		// get (n-1)
		if ( BN_sub ( bn_n_minus1, bn_n, BN_value_one() ) != 1 ) {
			printerror ( "ERR: get n-1 error" );
		}

		fprintf ( stdout, "  step 7, itr %d: R = %s, n = %s\n", i, BN_bn2dec ( bn_R ), BN_bn2dec ( bn_n ) );

		/*
		 * 7.2) if (TrialDivision(n) != "fail") {
		 */
		fprintf ( stdout, "    " );
		if ( traildiv ( BN_bn2dec ( bn_n ), primefile ) ) {
			num_bits_in_bn_n = BN_num_bits ( bn_n );
			/*
			 * 7.2.1) do {
			 */
			do {
				/*
				 * a = RndOddNum(num_bits_in_n)
				 */
				RndOddNum ( num_bits_in_bn_n, rnd_file, bn_a );

				/*
				 * while (a <= 1 or a >= n-1))
				 */
			} while ( BN_cmp ( bn_a, BN_value_one() ) != 1 || BN_cmp ( bn_a, bn_n_minus1 ) != -1 );
			fprintf ( stdout, "  step 7.2.1, itr %d: a = %s\n", i, BN_bn2dec ( bn_a ) );

			/*
			 * 7.2.2) b = a^{n-1} mod n
			 */
			BN_mod_exp ( bn_b, bn_a, bn_n_minus1, bn_n, bn_ctx );
			/*
			 * if (b == 1) {
			 */
			if ( BN_cmp ( bn_b, BN_value_one() ) == 0 ) {
				bn_d = BN_new();
				if ( bn_d == NULL ) {
					printerror ( "ERR: BN_new(bn_d) failed" );
				}
				/*
				 * b = a^{2R} mod n
				 */
				BN_mul ( bn_R, bn_R, bn_2, bn_ctx );
				BN_mod_exp ( bn_b, bn_a, bn_R, bn_n, bn_ctx );
				BN_sub ( bn_b, bn_b, BN_value_one() );
				/*
				 * d = gcd(b-1,n)
				 */
				BN_gcd ( bn_d, bn_b, bn_n, bn_ctx );
				/*
				 * if (d == 1) return(n)
				 */
				if ( BN_cmp ( bn_d, BN_value_one() ) == 0 ) {
					BN_CTX_free ( bn_ctx );
					//BN_free ( bn_n );
					BN_free ( bn_n_minus1 );
					BN_free ( bn_q );
					BN_free ( bn_I );
					BN_free ( bn_k_minus2 );
					BN_free ( bn_2 );
					BN_free ( bn_R );
					BN_free ( bn_a );
					BN_free ( bn_b );
					BN_free ( bn_d );
					return bn_n;
				}
			}
		}
		i++;
	}
}

int maurer_control ( int k, char* primefile, char * rndfile )
{
	BIGNUM * thePrime = NULL;
	FILE *rnd_file;
	rnd_file = fileopen ( rndfile );
	thePrime = BN_new();

	thePrime = maurer ( k, primefile, rnd_file , 0 );

	printf ( "\nMaurer's Algorithm found an %d-bit prime:\n", BN_num_bits ( thePrime ) );
	printf ( "  n = %s\n", BN_bn2dec ( thePrime ) );
	fclose ( rnd_file );
	BN_free ( thePrime );

	return 0;

}
