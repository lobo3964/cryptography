#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>


/*
 * Structure for commandline
 */
typedef struct ArgvsTag {
	int mode;
	int t;
	int i;
	int l;
	char* file1;
	char* file2;
} Argvs;

/*
 * Structure for frequency statistics
 */
typedef struct histpairTag {
	char lowercase;
	int count;
} histpair;

/*
 * Define Constants
 */
#define	OPTION_HEAD_LEN1		3
#define	OP_KEYGEN				0
#define	OP_CRYPT				1
#define	OP_INVKEY				2
#define	OP_HISTO				3
#define	OP_SOLVE				4

#define	KEYGEN_SPACE_SIZE		26

/*
 * Functions for HW4
 */
Argvs ParseCommandLine ( int argc, char **argv );
int keygen ( int t );
int crypt ( char* key, char* in_file );
int invkey ( char * keyfile );
int histo ( int period, int which, char * in_file );
int solve ( int max_t, char* in_file );
/*
 * utility functions
 */
FILE* fileopen ( char *filepath );
void printerror ( char msg[] );
