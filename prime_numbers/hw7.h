/*
 * This is the last homework of the course, I have to make comments pretty!
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <openssl/bn.h>
#include <arpa/inet.h>
#include <string.h>


/*
 * STREAM
 */
#define STREAM_OUTPUT_LEN		8

#define OP_PRIME					1
#define OP_TRIALDIV				2
#define OP_MILLRABIN				3
#define OP_RNDSEARCH				4
#define OP_MAURER					5

#define OPTION_HEAD_LEN1			3


typedef struct ArgvsTag {
	int mode;
	char* n;
	int k;
	int t;
	char* file_p;
	char* file_r;
} Argvs;



/*
 * functions called
 */
Argvs ParseCommandLine ( int argc, char **argv );
int prime_gen(char* maxval);
int traildiv(char* number, char* file_input);
int millerrabin(char* num, int max_t, char* input_file, int AsSubRoutine );
int rndsearch(int k, int max_t, char * primefile, char * rndfile);
void RndOddNum ( int k, FILE* rnd_input, BIGNUM *n );
int maurer_control ( int k, char* primefile, char * rndfile );
/*
 * helper functions
 */
FILE* fileopen ( char *filepath );
void printerror ( char  msg[] );
void allNum(char * str);

