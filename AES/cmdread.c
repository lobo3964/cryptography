/*
 * Read Command Line Argument
 * by Jie Gu
 * Reference: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
 */
#include "hw6.h"

void Usage()
{
	printerror (
	    "argument syntax error\n\
	hw6 tablecheck -t=tablefile\n\
    hw6 modprod -p1=poly1 -p2=poly2\n\
    hw6 keyexpand -k=key -t=tablefile\n\
    hw6 encrypt -k=key -t=tablefile [file]\n\
    hw6 decrypt -k=key -t=tablefile [file]\n\
    hw6 inverse -p=poly\n"	);
}

void checkarglimit ( int msg_type, int argc )
{
	if ( msg_type == OP_tablecheck && argc != 2 ) {
		Usage();
	} else if ( msg_type == OP_modprod && argc != 3 ) {
		Usage();
	} else if ( msg_type == OP_keyexpand && argc != 3 ) {
		Usage();
	} else if ( msg_type == OP_encrypt && ( argc > 4 || argc < 3 ) ) {
		Usage();
	} else if ( msg_type == OP_decrypt && ( argc > 4 || argc < 3 ) ) {
		Usage();
	} else if ( msg_type == OP_inverse && argc != 2 ) {
		Usage();
	}
}

Argvs ParseCommandLine ( int argc, char **argv )
{
	int t_flag = 0;
	int k_flag = 0;
	int p1_flag = 0;
	int p2_flag = 0;
	int file1_flag = 0;
	Argvs result;
	int msg_type = 0;
	argc--, argv++;
	if ( argc <= 0 ) {
		Usage();
	}

//BEGIN REFERENCE FROM: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
	if ( strcmp ( *argv, "tablecheck" ) == 0 ) {
		msg_type = OP_tablecheck;
	} else if ( strcmp ( *argv, "modprod" ) == 0 ) {
		msg_type = OP_modprod;
	} else if ( strcmp ( *argv, "keyexpand" ) == 0 ) {
		msg_type = OP_keyexpand;
	} else if ( strcmp ( *argv, "encrypt" ) == 0 ) {
		msg_type = OP_encrypt;
	} else if ( strcmp ( *argv, "decrypt" ) == 0 ) {
		msg_type = OP_decrypt;
	} else if ( strcmp ( *argv, "inverse" ) == 0 ) {
		msg_type = OP_inverse;
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
				if ( msg_type == OP_tablecheck || msg_type == OP_modprod || msg_type == OP_inverse || k_flag == 1 ) {
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
				if ( msg_type == OP_modprod || msg_type == OP_inverse || t_flag == 1 ) {
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

			} else if ( strncmp ( *argv, "-p1=" , OPTION_HEAD_LEN2 ) == 0 ) {
				if ( msg_type != OP_modprod || p1_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				++pch;
				if ( pch[0] == 0x0 ) { //NULL value
					Usage();
				}
				result.p1 = malloc ( strlen ( pch ) + 1 );
				strcpy ( result.p1 , pch );
				p1_flag = 1;

			} else if ( strncmp ( *argv, "-p2=" , OPTION_HEAD_LEN2 ) == 0 ) {
				if ( msg_type != OP_modprod || p2_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				++pch;
				if ( pch[0] == 0x0 ) { //NULL value
					Usage();
				}
				result.p2 = malloc ( strlen ( pch ) + 1 );
				strcpy ( result.p2 , pch );
				p2_flag = 1;

			} else if ( strncmp ( *argv, "-p=" , OPTION_HEAD_LEN1 ) == 0 ) {
				if ( msg_type != OP_inverse || p1_flag == 1 || p2_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				++pch;
				if ( pch[0] == 0x0 ) { //NULL value
					Usage();
				}
				result.p1 = malloc ( strlen ( pch ) + 1 );
				strcpy ( result.p1 , pch );
				p1_flag = 1;
			} else {
				Usage();
			}

			//[file] is before arguments
		} else if ( *argv[0] == '-' && file1_flag == 1 ) {
			Usage();

		} else if ( msg_type == OP_tablecheck || msg_type == OP_modprod || msg_type == OP_keyexpand || msg_type == OP_inverse ) {
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
