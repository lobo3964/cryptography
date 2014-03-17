/*
 * Read Command Line Argument
 * by Jie Gu
 * Reference: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
 */
#include "hw2.h"

void Usage()
{
	printerror (
	    "argument syntax error\n\
./hw2 stream -p=pphrase -l=len\n\
./hw2 encrypt -p=pphrase -out=name [pbmfile]\n\
./hw2 merge pbmfile1 pbmfile2\n\
./hw2 decrypt [pbmfile]" );
}
void checkarglimit ( int msg_type, int argc )
{
	if ( msg_type == OP_STREAM && argc != 3 ) {
		Usage();
	} else if ( msg_type == OP_ENCRYPT && ( argc > 4 || argc < 3 ) ) {
		Usage();
	} else if ( msg_type == OP_MERGE && argc != 3 ) {
		Usage();
	} else if ( msg_type == OP_DECRYPT && ( argc > 2 || argc < 1 ) ) {
		Usage();
	}
}


Argvs ParseCommandLine ( int argc, char **argv )
{
	int p_flag = 0;
	int l_flag = 0;
	int out_flag = 0;
	Argvs result;
	int msg_type = 0;
	argc--, argv++;
	if ( argc <= 0 ) {
		Usage();
	}
//BEGIN REFERENCE FROM: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
	if ( strcmp ( *argv, "stream" ) == 0 ) {
		msg_type = OP_STREAM;
	} else if ( strcmp ( *argv, "encrypt" ) == 0 ) {
		msg_type = OP_ENCRYPT;
	} else if ( strcmp ( *argv, "merge" ) == 0 ) {
		msg_type = OP_MERGE;
	} else if ( strcmp ( *argv, "decrypt" ) == 0 ) {
		msg_type = OP_DECRYPT;
	} else {
		Usage();
	}
	result.file1 = NULL;
	result.file2 = NULL;
	result.mode = msg_type;
	checkarglimit ( msg_type, argc );
	int file1_flag = 0;
	for ( argc--, argv++; argc > 0; argc--, argv++ ) {
		if ( *argv[0] == '-' && file1_flag == 0 ) {
			if ( strncmp ( *argv, "-p=" , OPTION_HEAD_LEN1 ) == 0 ) {
				if ( msg_type == OP_MERGE || msg_type == OP_DECRYPT || p_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				result.pphrase = malloc ( strlen ( ++pch ) + 1 );
				strcpy ( result.pphrase , pch );
				p_flag = 1;
			} else if ( strncmp ( *argv, "-l=" , OPTION_HEAD_LEN1 ) == 0 ) {
				if ( msg_type == OP_ENCRYPT || msg_type == OP_MERGE || msg_type == OP_DECRYPT || l_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				pch++;
				result.len = atoi ( pch );
				l_flag = 1;
			} else if ( strncmp ( *argv, "-out=" , OPTION_HEAD_LEN2 ) == 0 ) {
				if ( msg_type == OP_STREAM || msg_type == OP_MERGE || msg_type == OP_DECRYPT || out_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				result.out = malloc ( strlen ( ++pch ) + 1 );
				strcpy ( result.out , pch );
				out_flag = 1;
			} else {
				Usage();
			}
		} else if ( *argv[0] == '-' && file1_flag == 1 ) {
			Usage();
		} else if ( msg_type == OP_STREAM ) {
			Usage();
		} else {
			if ( file1_flag == 0 ) {
				result.file1 = malloc ( strlen ( *argv ) + 1 );
				strcpy ( result.file1 , *argv );
				file1_flag = 1;
			} else {
				if ( msg_type == OP_STREAM || msg_type == OP_ENCRYPT || msg_type == OP_DECRYPT ) {
					Usage();
				}
				result.file2 = malloc ( strlen ( *argv ) + 1 );
				strcpy ( result.file2 , *argv );
			}
		}
//END REFERENCE FROM: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
	}
	return result;
}
