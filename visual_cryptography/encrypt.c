/*
 * vis-cryptography: encrypt
 * by Jie Gu
 * Feb 10,2014
 *
 * This is a 4X expansion verison of vis-crypt
 * the input pbm image is encrypted into 2 files
 * using a passphrase generated key.
 *
 */
#include "hw2.h"

/*
 * take pbm_buf and key_buf to encrypt two shares of image
 * return the total number of bytes wrote for a single image (exclude header size)
 *
 * bug fixed: where if width%8!=0 but 2*width/8==0 would produce error result.
 */
int generatePBMs ( char* outputname, int width, int height, unsigned char  *key_buf, unsigned char *pbm_buf )
{
	int totalByte = 0;
	int pbm_i, key_i, k, shift, halfByte, pos, wc, hc;
	int extra_bit = width % 8;
	int in_widthByte = ( width % 8 > 0 ) ? ( width / 8 + 1 ) :  ( width / 8 );
	int out_widthByte = ( width * 2 % 8 > 0 ) ?  ( width * 2 / 8 + 1 ) : ( width * 2 / 8 );
	int limiter = out_widthByte * height;
	unsigned char cur_key_byte;
	unsigned char cur_pbm_byte;

	char outname1[BUFLEN];
	char outname2[BUFLEN];
	strcpy ( outname1, outputname );
	strcpy ( outname2, outputname );
	FILE * share1;
	FILE * share2;
	share1 = fopen ( strcat ( outname1, ".1.pbm" ), "w" );
	share2 = fopen ( strcat ( outname2, ".2.pbm" ), "w" );
	if ( share1 == NULL || share2 == NULL ) {
		printerror ( "file create failed" );
	}
	pbmheader ( share1, width * 2, height * 2 );
	pbmheader ( share2, width * 2, height * 2 );

	unsigned char* row1byteO1 = malloc ( out_widthByte );
	unsigned char* row2byteO1 = malloc ( out_widthByte );
	unsigned char* row1byteO2 = malloc ( out_widthByte );
	unsigned char* row2byteO2 = malloc ( out_widthByte );
	int key_bit = 0;
	int pbm_bit = 0;
	unsigned char* ByteUO1 = malloc ( sizeof ( unsigned char ) );
	unsigned char* ByteDO1 = malloc ( sizeof ( unsigned char ) );
	unsigned char* ByteUO2 = malloc ( sizeof ( unsigned char ) );
	unsigned char* ByteDO2 = malloc ( sizeof ( unsigned char ) );
	memset ( ByteUO1, 0, sizeof ( unsigned char ) );
	memset ( ByteDO1, 0, sizeof ( unsigned char ) );
	memset ( ByteUO2, 0, sizeof ( unsigned char ) );
	memset ( ByteDO2, 0, sizeof ( unsigned char ) );

	cur_key_byte = key_buf++[0];
	cur_pbm_byte = pbm_buf++[0];

	//initialize before looping
	shift = 3;
	halfByte = 0;
	pos = 0;
	key_i = 7;
	//hc=height counter
	for ( hc = 0; hc < height; hc++ ) {
		//wc=width counter
		for ( wc = 0; wc < in_widthByte; wc++ ) {

			//k limits how many bits pbm_i would iterate, if
			//its last byte, this would calculate based on
			//extra_bit, otherwise its always 0.
			k = ( wc == ( in_widthByte - 1 ) ) ? ( 7 - extra_bit + 1 ) : 0;

			//problem with lastByte flag set too ealier fixed
			for ( pbm_i = 7; pbm_i >= k; pbm_i-- ) {
				key_bit = ( int ) ( ( cur_key_byte >> key_i-- ) & 0x1 );
				pbm_bit = ( int ) ( ( cur_pbm_byte >> pbm_i ) & 0x1 );
				if ( key_bit == 0 && pbm_bit == 0 ) {
					ByteUO1[0] = ByteUO1[0] | ( W01U << ( shift * 2 ) );
					ByteDO1[0] = ByteDO1[0] | ( W01D << ( shift * 2 ) );
					ByteUO2[0] = ByteUO2[0] | ( W02U << ( shift * 2 ) );
					ByteDO2[0] = ByteDO2[0] | ( W02D << ( shift * 2 ) );
				} else if ( key_bit == 1 && pbm_bit == 0 ) {
					ByteUO1[0] = ByteUO1[0] | ( W11U << ( shift * 2 ) );
					ByteDO1[0] = ByteDO1[0] | ( W11D << ( shift * 2 ) );
					ByteUO2[0] = ByteUO2[0] | ( W12U << ( shift * 2 ) );
					ByteDO2[0] = ByteDO2[0] | ( W12D << ( shift * 2 ) );
				} else if ( key_bit == 0 && pbm_bit == 1 ) {
					ByteUO1[0] = ByteUO1[0] | ( B01U << ( shift * 2 ) );
					ByteDO1[0] = ByteDO1[0] | ( B01D << ( shift * 2 ) );
					ByteUO2[0] = ByteUO2[0] | ( B02U << ( shift * 2 ) );
					ByteDO2[0] = ByteDO2[0] | ( B02D << ( shift * 2 ) );
				} else if ( key_bit == 1 && pbm_bit == 1 ) {
					ByteUO1[0] = ByteUO1[0] | ( B11U << ( shift * 2 ) );
					ByteDO1[0] = ByteDO1[0] | ( B11D << ( shift * 2 ) );
					ByteUO2[0] = ByteUO2[0] | ( B12U << ( shift * 2 ) );
					ByteDO2[0] = ByteDO2[0] | ( B12D << ( shift * 2 ) );
				} else {
					printerror ( "generatePMBs unkown error" );
				}

				shift--;
				//pbm_i == k as temp fix **test case passed**
				if ( ++halfByte == HALFBYTE || pbm_i == k ) {
					memcpy ( &row1byteO1[pos], ByteUO1, sizeof ( unsigned char ) );
					memcpy ( &row2byteO1[pos], ByteDO1, sizeof ( unsigned char ) );
					memcpy ( &row1byteO2[pos], ByteUO2, sizeof ( unsigned char ) );
					memcpy ( &row2byteO2[pos], ByteDO2, sizeof ( unsigned char ) );

					memset ( ByteUO1, 0, sizeof ( unsigned char ) );
					memset ( ByteDO1, 0, sizeof ( unsigned char ) );
					memset ( ByteUO2, 0, sizeof ( unsigned char ) );
					memset ( ByteDO2, 0, sizeof ( unsigned char ) );
					halfByte = 0;
					shift = 3;
					pos++;
					totalByte++;
				}
				//key_i is a seperate iterator, it reset itself when reaches 0
				if ( key_i < 0 ) {
					key_i = 7;
					cur_key_byte = key_buf++[0];
				}
			}
			//limit reading out of index in the very end to cause memory error
			if ( totalByte < limiter ) {
				cur_pbm_byte = pbm_buf++[0];
			}

		}
		//row1 and row2 for each image constructed, write to file and check retval
		int ck1 = fwrite ( row1byteO1 , sizeof ( unsigned char ), out_widthByte, share1 );
		int ck2 = fwrite ( row2byteO1 , sizeof ( unsigned char ), out_widthByte, share1 );
		int ck3 = fwrite ( row1byteO2 , sizeof ( unsigned char ), out_widthByte, share2 );
		int ck4 = fwrite ( row2byteO2 , sizeof ( unsigned char ), out_widthByte, share2 );
		if ( ( ck1 == ck2 ) && ( ck2 == ck3 ) && ( ck3 == ck4 ) && ( ck4 == out_widthByte ) ) {
			pos = 0;
		} else {
			printerror ( "write to file errors" );
		}
	}
	fclose ( share1 );
	fclose ( share2 );
	free ( row1byteO1 );
	free ( row2byteO1 );
	free ( row1byteO2 );
	free ( row2byteO2 );
	free ( ByteUO1 );
	free ( ByteDO1 );
	free ( ByteUO2 );
	free ( ByteDO2 );
	return totalByte * 2;
}
/*
 * main function to handle visual-cryptography
 * return 0 if no error found
 */
int encrypt ( char * p, char * out, char* filepath )
{
	int width, height, keylen, pbmlen, retval, eofflag, expect;
	unsigned char* key_buf;
	unsigned char* pbm_buf;
	FILE* pbm_fp = fileopen ( filepath );

	// verify the magic number and width height of pbm file
	pbmVerify ( pbm_fp, &width, &height ) ;
	keylen = ( width * height ) / 8 + 1;
	pbmlen = ( height * ( width / 8 + 1 ) );
	key_buf = ( unsigned char* ) malloc ( keylen );
	pbm_buf = ( unsigned char* ) malloc ( pbmlen );

	stream ( p, keylen, key_buf, OP_ENCRYPT );

	/* read pbmlen+1 byte is to make sure read extra byte,
	 * if the filecontains extactly pbmlen data, it should
	 * return correct data size
	 */
	retval = fread ( pbm_buf, 1, pbmlen + 1, pbm_fp );
	eofflag = feof ( pbm_fp );

	//check if file actually contains pbmlen of data and reaches the eof
	if ( retval == pbmlen && eofflag != 0 ) {
		fclose ( pbm_fp );
	} else {
		printerror ( "pbm verify failed" );
	}

	// key_buf and pbm_buf filled
	retval = generatePBMs ( out, width, height, key_buf, pbm_buf );
	expect = ( width * 2 % 8 > 0 ) ?  ( width * 2 / 8 + 1 ) : ( width * 2 / 8 );
	expect = expect * height * 2;
	if ( retval == expect ) {
		free ( key_buf );
		free ( pbm_buf );
		return 0;
	} else {
		printerror ( "encrypt output length error" );
	}
	return 1;
}
