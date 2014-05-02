#include "hw7.h"

void RndOddNum ( int k, FILE* rnd_input, BIGNUM *n )
{
	int i = 0;
	int sizeByte = ( int ) ceil ( ( double ) k / 8.0 );
	unsigned char* rnd = malloc ( sizeof ( unsigned int ) * sizeByte );
	if ( fread ( rnd, 1, sizeByte, rnd_input ) != sizeByte ) {
		printerror ( "ERR: read rnd error" );
	}

	if ( BN_bin2bn ( rnd, sizeByte, n ) == NULL ) {
		printerror ( "ERR: small prime -> BN error" );
	}
	BN_set_bit ( n, 0 );
	BN_set_bit ( n, k - 1 );
	for ( i = k; i < ( sizeByte * 8 ); i++ ) {
		BN_clear_bit ( n, i );
	}
	//fprintf ( stdout, "num: %s\n", BN_bn2dec ( n ) );
	free ( rnd );
}

int rndsearch ( int k, int max_t, char * primefile, char * rndfile )
{
	int i = 1;
	//int j = 0;
	//int bn_s_val = 0;
	//FILE *prime_file;
	FILE *rnd_file;
	//prime_file = fileopen ( primefile );
	rnd_file = fileopen ( rndfile );
	//unsigned int* small_prime = malloc ( sizeof ( unsigned int ) );
	BN_CTX *bn_ctx = NULL;
	BIGNUM *bn_n = NULL;
//	BIGNUM *bn_s = NULL;
//	BIGNUM *bn_target = NULL;
//	BIGNUM *bn_target_one = NULL;
//	BIGNUM *bn_a = NULL;
//	BIGNUM *bn_y = NULL;
	bn_n = BN_new();
//	bn_target_one = BN_new();
//	bn_a = BN_new();
//	bn_r = BN_new();
//	bn_s = BN_new();
//	bn_y = BN_new();

	bn_ctx = BN_CTX_new();
	if ( bn_ctx == NULL ) {
		printerror ( "ERR: CTX initialize error" );
	}
	if ( bn_n == NULL ) {
		printerror ( "ERR: initialize BN error" );
	}

	while(1) {

		/*
		 * 1) n = RndOddNum(k)
		 */
		fprintf ( stdout, "RANDOM-SEARCH: iteration %d\n", i++ );
		RndOddNum ( k, rnd_file, bn_n );
		fprintf ( stdout, "  n = %s\n", BN_bn2dec ( bn_n ) );

		/*
		 * 2) if (TrialDivision(n) == "fail") goto step (1)
		 */
		 fprintf(stdout,"  ");
		if ( traildiv ( BN_bn2dec ( bn_n ), primefile ) ) {
			if (millerrabin ( BN_bn2dec ( bn_n ), max_t, primefile , 1 )){
				return 1;
			}
		}
	}

	return 0;
}
