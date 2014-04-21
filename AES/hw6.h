#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * Structure for commandline
 */
typedef struct ArgvsTag {
	int mode;
	char* t;
	char* k;
	char* p1;
	char* p2;
	char* file1;
} Argvs;

#define m_x 0x1B
#define YES 1
#define NO 0
#define OPTION_HEAD_LEN1 3
#define OPTION_HEAD_LEN2 4

#define OP_tablecheck 1
#define OP_modprod 2
#define OP_keyexpand 3
#define OP_encrypt 4
#define OP_decrypt 5
#define OP_inverse 6

//global variable of tables
extern unsigned char S[256];
extern unsigned char IS[256];
extern unsigned char P[4];
extern unsigned char IP[4];
extern unsigned char w[44][4];
extern char *mi;

//unsigned char xtime ( unsigned char b );
unsigned char bd ( unsigned char a, unsigned char b );
void modprod ( unsigned char a[], unsigned char b[], unsigned char res[] );
void strToHex ( char p1[8], unsigned char h1[4], char p2[8], unsigned char h2[4] );
FILE* fileopen ( char *filepath );
void printerror ( char msg[] );
int charToHex ( char c );

int tablecheck ( char * tablefile );
Argvs ParseCommandLine ( int argc, char **argv );
int modproduct ( char p1[8], char p2[8] );
int KeyExp(char *key, char *table_in, int print);
int AES ( char *file_in, char *key, char *table , int mode );
int inverse ( char *poly );
