/*
 * vis-cryptography: decrypt
 * by Jie Gu
 *
 * decrypt a merged vis-crypto pbm file
 * did not bit to bit file format checking
 * assumed every bit is valid, and only reads odd line number
 *
 */
#include "hw2.h"

int decrypt ( char* pbm_filename )
{
	int width, height, wc, hc, shift, bit_pair, bit_shift, pos;

	FILE* pbm_fp = fileopen ( pbm_filename );
	pbmVerify ( pbm_fp, &width, &height );
	if ( width % 2 != 0 || height % 2 != 0 ) {
		printerror ( "input pbm illegal dimension" );
	}

	//int extra_bit = width % 8;
	int in_widthByte = ( width % 8 > 0 ) ? ( width / 8 + 1 ) :  ( width / 8 );

	int pbm_len = in_widthByte * height;
	unsigned char* pbm_buf = malloc ( pbm_len );
	if ( fread ( pbm_buf, sizeof ( unsigned char ), pbm_len, pbm_fp ) == pbm_len  ) {
		fclose ( pbm_fp );
	} else {
		printerror ( "input file format error" );
	}

	pbmheader ( stdout, width / 2, height / 2 );

	int out_widthBit = width / 2;
	int out_widthByte = ( out_widthBit % 8 > 0 ) ? ( out_widthBit / 8 + 1 ) :  ( out_widthBit / 8 );
	int out_heightBit = height / 2;
	unsigned char *curByte;
	unsigned char *outByte = malloc ( sizeof ( unsigned char ) );
	memset ( outByte, 0, sizeof ( unsigned char ) );
	unsigned char bitPair;// = malloc ( sizeof ( unsigned char ) );
	unsigned char *outrow = malloc ( out_widthByte );
	unsigned char *inrow = malloc ( in_widthByte );


	bit_shift = 7;
	for ( hc = 0; hc < out_heightBit; hc++ ) {
		//only copy the even row, igore odd row
		memcpy ( inrow, &pbm_buf[hc * 2 * in_widthByte], in_widthByte );
		curByte = inrow;
		pos = 0;
		for ( wc = 0; wc < in_widthByte; wc++ ) {
			//read each byte in a row
			shift = 3;
			//k = ( wc == ( in_widthByte - 1 ) ) ? ( extra_bit / 2 ) : 4;
			for ( bit_pair = 0; bit_pair < 4; bit_pair++ ) {
				//compare 4 bit-pairs in a byte
				//memset ( bitPair, 0, sizeof ( unsigned char ) );
				bitPair = ( curByte[0] >> ( ( shift-- ) * 2 ) ) & 0x3;
				if ( bitPair == BIT_BLACK ) {
					outByte[0] = outByte[0] | ( 0x1 << bit_shift-- );
				} else if ( bitPair == BIT_GREY1 || bitPair == BIT_GREY2 ) {
					outByte[0] = outByte[0] | ( 0x0 << bit_shift-- );
				} else if ( bitPair == BIT_WHITE ) {
					outByte[0] = outByte[0] | ( 0x0 << bit_shift-- );
				}
				// previous condition  ( k != 4 && bit_pair == ( k - 1 ) )
				if ( bit_shift < 0 ) {
					bit_shift = 7;
					outrow[pos++] = outByte[0];
					memset ( outByte, 0, sizeof ( unsigned char ) );
				}
			}

			if ( wc == in_widthByte - 1 ) {
				bit_shift = 7;
				outrow[pos++] = outByte[0];
				memset ( outByte, 0, sizeof ( unsigned char ) );
			}

			curByte++;
		}
		if ( fwrite ( outrow, sizeof ( unsigned char ), out_widthByte, stdout ) != out_widthByte ) {
			printerror ( "error write row" );
		}
	}
	free ( pbm_buf );
	free ( outByte );
	free ( outrow );
	free ( inrow );

	return 0;
}
