/*
 * Simple Stream Ciper Key Generator
 * By Jie Gu
 * Feb 10, 2014
 *
 * the buffer whose MD5 checksum is getting computed
 * has 3 parts. The first part is 16 bytes long and
 * it is the MD5 checksum of the previous value of the
 * buffer. The 2nd part is an ASCII string of length 2
 * that takes on the value from "00" through "99". The
 * 3rd part is the passphrase. Initially, the first
 * part is the MD5 checksum of the passphrase.
 * 
 * Reference: http://merlot.usc.edu/cs531-s14/homeworks/hw2/#stream
 *
 */
#include "hw2.h"

int stream ( char* p, int cipher_len , unsigned char* remote, int mode )
{
	//char p[] = "yesnomaybe";
	//int cipher_len = 202;
	int cycle, remainder;
	cycle = cipher_len / STREAM_OUTPUT_LEN;
	remainder = cipher_len % STREAM_OUTPUT_LEN;
	int len = strlen ( p ) + 2 + MD5_DIGEST_LENGTH;
	char *s = malloc ( len + 1 );
	unsigned char md5_buf[MD5_DIGEST_LENGTH];
	MD5 (  ( unsigned char* ) p, strlen ( p ), ( unsigned char* ) &md5_buf );
	int count = 0;
	int i = 0;
	unsigned char* write_remote = remote;

	//BEGIN CODE ADAPTED FROM CS531 WEBSITE
	for ( count = 0; count < cycle; count++ ) {
		sprintf ( &s[MD5_DIGEST_LENGTH], "%02d%s", i, p );
		memcpy ( s, md5_buf, MD5_DIGEST_LENGTH );
		MD5 ( ( unsigned char* ) s, len, md5_buf );
		if ( mode == OP_STREAM ) {
			fwrite ( md5_buf, 1, STREAM_OUTPUT_LEN, stdout );
		}
		if ( mode == OP_ENCRYPT ) {
			memcpy ( write_remote, md5_buf, STREAM_OUTPUT_LEN );
			write_remote += STREAM_OUTPUT_LEN;
		}
		if ( ++i == 100 ) i = 0;
	}
	//END CODE ADAPTED FROM CS531 WEBSITE

	if ( remainder > 0 ) {
		sprintf ( &s[MD5_DIGEST_LENGTH], "%02d%s", i, p );
		memcpy ( s, md5_buf, MD5_DIGEST_LENGTH );
		MD5 ( ( unsigned char* ) s, len, md5_buf );
		if ( mode == OP_STREAM ) {
			fwrite ( md5_buf, 1, remainder, stdout );
		}
		if ( mode == OP_ENCRYPT ) {
			memcpy ( write_remote, md5_buf, remainder );
		}
	}
	free ( s );
	return 0;
}
