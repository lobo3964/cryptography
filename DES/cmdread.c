/*
 * Read Command Line Argument
 * by Jie Gu
 * Reference: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
 */
#include "hw5.h"

void Usage()
{
	printerror (
	    "argument syntax error\n\
	./hw5 tablecheck -t=tablefile\n\
	./hw5 encrypt -k=key -t=tablefile [file]\n\
	./hw5 decrypt -k=key -t=tablefile [file]\n\
	./hw5 encrypt3 -k=key3 -t=tablefile [file]\n\
	./hw5 decrypt3 -k=key3 -t=tablefile [file]\n"	);
}

void checkarglimit ( int msg_type, int argc )
{
	if ( msg_type == OP_TABLECHECK && argc != 2 ) {
		Usage();
	} else if ( msg_type == OP_ENCRYPT && ( argc > 4 || argc < 3 ) ) {
		Usage();
	} else if ( msg_type == OP_DECRYPT && ( argc > 4 || argc < 3 ) ) {
		Usage();
	} else if ( msg_type == OP_ENCRYPT3 && ( argc > 4 || argc < 3 ) ) {
		Usage();
	} else if ( msg_type == OP_DECRYPT3 && ( argc > 4 || argc < 3 ) ) {
		Usage();
	}
}

Argvs ParseCommandLine ( int argc, char **argv )
{

	int t_flag = 0;
	int k_flag = 0;
	int file1_flag = 0;
	Argvs result;
	int msg_type = 0;
	argc--, argv++;
	if ( argc <= 0 ) {
		Usage();
	}
//BEGIN REFERENCE FROM: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
	if ( strcmp ( *argv, "tablecheck" ) == 0 ) {
		msg_type = OP_TABLECHECK;
	} else if ( strcmp ( *argv, "encrypt" ) == 0 ) {
		msg_type = OP_ENCRYPT;
	} else if ( strcmp ( *argv, "decrypt" ) == 0 ) {
		msg_type = OP_DECRYPT;
	} else if ( strcmp ( *argv, "encrypt3" ) == 0 ) {
		msg_type = OP_ENCRYPT3;
	} else if ( strcmp ( *argv, "decrypt3" ) == 0 ) {
		msg_type = OP_DECRYPT3;
	} else {
		Usage();
	}
	result.file1 = NULL;
	result.mode = msg_type;
	checkarglimit ( msg_type, argc );

	for ( argc--, argv++; argc > 0; argc--, argv++ ) {
		//before read [file]
		if ( *argv[0] == '-' && file1_flag == 0 ) {
			if ( strncmp ( *argv, "-k=" , OPTION_HEAD_LEN1 ) == 0 ) {
				if ( msg_type == OP_TABLECHECK || k_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				++pch;
				if ( pch[0] == 0x0 ) { //NULL value
					Usage();
				}
				result.k = malloc ( strlen ( pch ) + 1 );
				strcpy ( result.k , pch );
				k_flag = 1;
			} else if ( strncmp ( *argv, "-t=" , OPTION_HEAD_LEN1 ) == 0 ) {
				//result.l= NULL;
				if ( t_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				++pch;
				if ( pch[0] == 0x0 ) { //NULL value
					Usage();
				}
				result.t = malloc ( strlen ( pch ) + 1 );
				strcpy ( result.t , pch );
				t_flag = 1;

			} else {
				Usage();
			}
			//[file] is before arguments
		} else if ( *argv[0] == '-' && file1_flag == 1 ) {
			Usage();
		} else if ( msg_type == OP_TABLECHECK ) {
			Usage();
			//read [file]
		} else {
			if ( file1_flag == 0 ) {
				result.file1 = malloc ( strlen ( *argv ) + 1 );
				strcpy ( result.file1 , *argv );
				file1_flag = 1;
			} else {
				Usage();
			}
		}
//END REFERENCE FROM: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
	}
	return result;
}
