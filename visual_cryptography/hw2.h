#ifndef HW2_H
#define HW2_H

#include <stdio.h>
#include <sys/types.h>
#include <openssl/md5.h>
#include <string.h>
#include <stdlib.h>

/*
 * STREAM
 */
#define STREAM_OUTPUT_LEN		8
#define OP_STREAM					1
#define OP_ENCRYPT				2
#define OP_MERGE					3
#define OP_DECRYPT				4
#define HALFBYTE					4
#define BUFLEN					100
#define OPTION_HEAD_LEN1			3	// -p=  -l=
#define OPTION_HEAD_LEN2			5  // -out=

/*
 * ENCRYPTION
 */
#define W01U						0x2
#define W01D						0x1
#define W02U						0x2
#define W02D						0x1
#define W11U						0x1
#define W11D						0x2
#define W12U						0x1
#define W12D						0x2

#define B01U						0x2
#define B01D						0x1
#define B02U						0x1
#define B02D						0x2
#define B11U						0x1
#define B11D						0x2
#define B12U						0x2
#define B12D						0x1

/*
 * DECRYPTION
 */
#define BIT_BLACK					0x3
#define BIT_GREY1					0x1
#define BIT_GREY2					0x2
#define BIT_WHITE					0x0

typedef struct ArgvsTag {
	int mode;
	char* pphrase;
	int len;
	char* out;
	char* file1;
	char* file2;
} Argvs;



/*
 * functions called
 */
Argvs ParseCommandLine ( int argc, char **argv );
int stream ( char* p, int cipher_len , unsigned char* remote, int mode );
int encrypt ( char * p, char * out, char* filepath );
int merge ( char *file1, char *file2 );
int decrypt ( char *pbm_filename );


/*
 * helper functions
 */
FILE* fileopen ( char *filepath );
void printerror ( char  msg[] );
void pbmVerify ( FILE *pbm_fp, int *width, int *height );
void pbmheader ( FILE* fp, int width, int height );

#endif
