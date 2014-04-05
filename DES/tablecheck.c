/**
 * Tablecheck for DES
 * checks the table format
 * Alert apporiate error messages to stderr
 * 
 * No output if table is valid
 */
#include "hw5.h"
#define BUFFLEN		100
#define 	GENERAL	1
#define	E	2
#define	PC1		3
#define	PC2		4

int rangeCheck ( char* tablename, int num, int from, int to, int mode )
{
	if ( ( mode == GENERAL || mode == E || mode == PC2 ) && ( num > to || num < from ) ) {
		fprintf ( stderr, "%s", tablename );
		fprintf ( stderr, "value %d is invalid, must between [%d, %d]\n", num, from, to );
		return 0;
	}

	if ( mode == PC1 && ( num > to || num < from ) && ( num % 8 == 0 ) ) {
		fprintf ( stderr, "%s", tablename );
		fprintf ( stderr, "value %d is invalid, must between [%d, %d], excludes multiples of 8\n", num, from, to );
		return 0;
	}
	return 1;
}

void checkPermuteV ( char* char_array )
{
	int i = 0;
	int oneCount = 0;
	int twoCount = 0;
	char* ptr = char_array;

	//skip V=
	while ( ptr[0] != '=' ) {
		ptr++;
	}
	ptr++;

	while ( ptr[0] != 0x0a ) {
		i++;
		if ( i > 100 ) {
			printerror ( "critical error, new line not found" );
		}
		if ( ptr[0] == '2' ) {
			twoCount++;
		} else if ( ptr[0] == '1' ) {
			oneCount++;
		} else if ( ptr[0] == ',' ) {
			//skip
		} else {
			fprintf ( stderr, "(V) found value other than 1 and 2\n" );
		}
		ptr++;
	}
	if ( oneCount != 4 ) {
		fprintf ( stderr, "(V) value 1 did not occur 4 times\n" );
	}
	if ( twoCount != 12 ) {
		fprintf ( stderr, "(V) value 2 did not occur 12 times\n" );
	}
}

/*
 * check if each element in [from, to] appeared in char_array exactly [occur] times
 * can be used to check general tables
 */
void checkPermute ( char* tablename, char* char_array, int from, int to, int occur , int mode )
{
	int i = 0;
	int num = 0;
	char* count = malloc ( to + from + 1 );
	memset ( count, 0, to + from + 1 );
	int digit10 = 10;//10 as NULL value
	int digit1 = 10;
	char* ptr = char_array;
	while ( ptr[0] != '=' ) {
		ptr++;
	}

	while ( ptr[0] != 0x0a ) {
		i++;
		if ( i > 200 ) {
			printerror ( "critical error, new line not found" );
		}
		if ( ptr[0] == 0x2c ) {
			//determine num and
			//increment counter
			if ( digit1 == 10 ) {
				num = digit10;
			} else {
				num = digit10 * 10 + digit1;
			}
			//rangeCheck();
			if ( rangeCheck ( tablename, num, from, to, mode )==1 ) {


				if ( ( ( count + num ) [0] ) >= occur ) {
					fprintf ( stderr, "%s", tablename );
					fprintf ( stderr, "the value %d occur too many times\n", num );
				} else {
					//increment
					memset ( ( count + num ), ( ( count + num ) [0] + 1 ), 1 );
				}

			}
			digit10 = 10;
			digit1 = 10;
		} else if ( ptr[0] >= '0' && ptr[0] <= '9' ) {
			if ( digit10 == 10 ) {
				digit10 = ptr[0] - '0';
			} else {
				digit1 = ptr[0] - '0';
			}
		}
		ptr++;
	}
	//add the last num
	if ( digit1 == 10 ) {
		num = digit10;
	} else {
		num = digit10 * 10 + digit1;
	}

	//rangeCheck();
	if ( rangeCheck ( tablename, num, from, to, mode )==1 ) {

		if ( ( ( count + num ) [0] ) >= occur ) {
			fprintf ( stderr, "%s", tablename );
			fprintf ( stderr, "the value %d occur too many times\n", num );
		} else {
			memset ( ( count + num ), ( ( count + num ) [0] + 1 ), 1 );
		}

	}


	//final verify
	if ( mode == GENERAL ) {
		for ( i = from; i < to + 1; i++ ) {
			if ( ( count + i ) [0] != occur ) {
				fprintf ( stderr, "%s", tablename );
				if ( occur == 1 ) {
					fprintf ( stderr, "the value %d did not occur\n", i );
				} else {
					fprintf ( stderr, "the value %d did not occur %d times\n", i, occur );
				}
			}
		}
	} else if ( mode == E ) {
		for ( i = from; i < to + 1; i++ ) {
			if ( ( count + i ) [0] < 1 ) {
				fprintf ( stderr, "%s", tablename );
				fprintf ( stderr, "the value %d must occur at least one time\n", i );
			}
		}
	} else if ( mode == PC1 ) {
		for ( i = from; i < to + 1; i++ ) {
			int k = ( count + i ) [0];
			if ( i % 8 != 0 && k == 0  ) {
				fprintf ( stderr, "%s", tablename );
				fprintf ( stderr, "the value %d did not occur\n", i );
			}
		}
	} else if ( mode == PC2 ) {
		int total = 0;
		for ( i = from; i < to + 1; i++ ) {
			total += ( count + i ) [0];
		}
		if ( total < 48 ) {
			fprintf ( stderr, "%s", tablename );
			fprintf ( stderr, "read %d values, expect 48\n", total );
		}
	}
	memset ( count, 0, sizeof ( char ) * ( to + from ) );
	free ( count );
}


int tablecheck ( char* tablefile )
{
	FILE* table_in = fileopen ( tablefile );
	int linecount = TABLELINENUM;
	//int retval=0;
	while ( linecount > 0 ) {
		char* LINE = malloc ( sizeof ( char ) * READLEN );
		memset ( LINE, 0, sizeof ( char ) *READLEN );
		if ( fgets ( LINE, READLEN, table_in ) == NULL ) {
			printerror ( "read line error" );
		}
		if ( strncmp ( LINE, "IP=", 3 ) == 0 ) {
			checkPermute ( "(IP) ", LINE, 1, 64, 1 , GENERAL );
			linecount--;
		} else if ( strncmp ( LINE, "E=", 2 ) == 0 ) {
			checkPermute ( "(E) ", LINE, 1, 32, 2 , E );
			linecount--;
		} else if ( strncmp ( LINE, "P=", 2 ) == 0 ) {
			checkPermute ( "( P) ", LINE, 1, 32, 1 , GENERAL );
			linecount--;
		} else if ( strncmp ( LINE, "S1=", 3 ) == 0 ) {
			checkPermute ( "(S1) ", LINE, 0, 15, 4 , GENERAL );
			linecount--;
		} else if ( strncmp ( LINE, "S2=", 3 ) == 0 ) {
			checkPermute ( "(S2) ", LINE, 0, 15, 4 , GENERAL );
			linecount--;
		} else if ( strncmp ( LINE, "S3=", 3 ) == 0 ) {
			checkPermute ( "(S3) ", LINE, 0, 15, 4 , GENERAL );
			linecount--;
		} else if ( strncmp ( LINE, "S4=", 3 ) == 0 ) {
			checkPermute ( "(S4) ", LINE, 0, 15, 4 , GENERAL );
			linecount--;
		} else if ( strncmp ( LINE, "S5=", 3 ) == 0 ) {
			checkPermute ( "(S5) ", LINE, 0, 15, 4 , GENERAL );
			linecount--;
		} else if ( strncmp ( LINE, "S6=", 3 ) == 0 ) {
			checkPermute ( "(S6) ", LINE, 0, 15, 4 , GENERAL );
			linecount--;
		} else if ( strncmp ( LINE, "S7=", 3 ) == 0 ) {
			checkPermute ( "(S7) ", LINE, 0, 15, 4 , GENERAL );
			linecount--;
		} else if ( strncmp ( LINE, "S8=", 3 ) == 0 ) {
			checkPermute ( "(S8) ", LINE, 0, 15, 4 , GENERAL );
			linecount--;
		} else if ( strncmp ( LINE, "V=", 2 ) == 0 ) {
			checkPermuteV ( LINE );
			linecount--;
		} else if ( strncmp ( LINE, "PC1=", 4 ) == 0 ) {
			checkPermute ( "(PC1) ", LINE, 1, 64, 1 , PC1 );
			linecount--;
		} else if ( strncmp ( LINE, "PC2=", 4 ) == 0 ) {
			checkPermute ( "(PC2) ", LINE, 1, 56, 1 , PC2 );
			linecount--;
		} else {
			printerror("invalid table file");
		}
		free ( LINE );
	}
	fclose ( table_in );

	return 0;
}
