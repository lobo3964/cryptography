#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Structure for commandline
 */
typedef struct ArgvsTag {
	int mode;
	char* t;
	char* k;
	char* file1;
} Argvs;

/*
 * define constants
 */ 
#define	OPTION_HEAD_LEN1	3
#define	OP_TABLECHECK		1
#define	OP_ENCRYPT			2
#define	OP_DECRYPT			3
#define	OP_ENCRYPT3		4
#define	OP_DECRYPT3		5

#define TABLELINENUM		14
#define READLEN		200

#define WEAK_KEY	1
#define SEMI_WEAK_KEY		2
#define GOOD_KEY  0
 
/*
 * key functions
 */
void checkPermute (char* tablename, char* char_array, int from, int to, int occur ,int mode);
int tablecheck ( char* tablefile );
int cryptoDES ( char* key, char* in_tb, char* in_pt ,int mode);
/*
 * utility functions
 */
int keyCheck ( char* key );
Argvs ParseCommandLine ( int argc, char **argv );
FILE* fileopen ( char *filepath );
void printerror ( char msg[] );
