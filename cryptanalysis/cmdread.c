/*
 * Read Command Line Argument
 * by Jie Gu
 * Reference: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
 */
#include "hw4.h"

void Usage()
{
	printerror (
	    "argument syntax error\n\
	./hw4 keygen -t=period\n\
	./hw4 crypt -k=keyfile [file]\n\
	./hw4 invkey keyfile\n\
	./hw4 histo -t=period -i=which [file]\n\
	./hw4 solve -l=max_t file\n"	);
}
void checkarglimit ( int msg_type, int argc )
{
	if ( msg_type == OP_KEYGEN && argc != 2 ) {
		Usage();
	} else if ( msg_type == OP_CRYPT && ( argc > 3 || argc < 2 ) ) {
		Usage();
	} else if ( msg_type == OP_INVKEY && argc != 2 ) {
		Usage();
	} else if ( msg_type == OP_HISTO && ( argc > 4 || argc < 3 ) ) {
		Usage();
	} else if ( msg_type == OP_SOLVE && ( argc != 3 ) ) {
		Usage();
	}
}


Argvs ParseCommandLine ( int argc, char **argv )
{

	int t_flag = 0;
	int k_flag = 0;
	int i_flag = 0;
	int l_flag = 0;
	Argvs result;
	int msg_type = 0;
	argc--, argv++;
	if ( argc <= 0 ) {
		Usage();
	}
//BEGIN REFERENCE FROM: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
	if ( strcmp ( *argv, "keygen" ) == 0 ) {
		msg_type = OP_KEYGEN;
	} else if ( strcmp ( *argv, "crypt" ) == 0 ) {
		msg_type = OP_CRYPT;
	} else if ( strcmp ( *argv, "invkey" ) == 0 ) {
		msg_type = OP_INVKEY;
	} else if ( strcmp ( *argv, "histo" ) == 0 ) {
		msg_type = OP_HISTO;
	} else if ( strcmp ( *argv, "solve" ) == 0 ) {
		msg_type = OP_SOLVE;
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
			if ( strncmp ( *argv, "-t=" , OPTION_HEAD_LEN1 ) == 0 ) {
				if ( msg_type == OP_CRYPT || msg_type == OP_INVKEY || msg_type == OP_SOLVE || t_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				++pch;
				if ( pch[0] == 0x0 ) { //NULL value
					Usage();
				}
				result.t = atoi ( pch );
				if (result.t<=0){
					printerror("t must be greater than 0");
				}
				t_flag = 1;
			} else if ( strncmp ( *argv, "-l=" , OPTION_HEAD_LEN1 ) == 0 ) {
				//result.l= NULL;
				if ( msg_type != OP_SOLVE || l_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				pch++;
				if ( pch[0] == 0x0 ) { //NULL value
					Usage();
				}
				result.l = atoi ( pch );
				if (result.i<=0){
					printerror("l must be greater than 0");
				}
				l_flag = 1;
			} else if ( strncmp ( *argv, "-k=" , OPTION_HEAD_LEN1 ) == 0 ) {
				if ( msg_type != OP_CRYPT || k_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				result.file1 = malloc ( strlen ( ++pch ) + 1 );
				strcpy ( result.file1 , pch );
				k_flag = 1;
				file1_flag = 1;
			} else if ( strncmp ( *argv, "-i=" , OPTION_HEAD_LEN1 ) == 0 ) {
				if ( msg_type != OP_HISTO || i_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				pch++;
				if ( pch[0] == 0x0 ) { //NULL value
					Usage();
				}
				result.i = atoi ( pch );
				if (result.i<=0){
					printerror("i must be one-based positive integer");
				}
				i_flag = 1;
			} else {
				Usage();
			}
		} else if ( *argv[0] == '-' && file1_flag == 1 ) {
			Usage();
		} else if ( msg_type == OP_KEYGEN ) {
			Usage();
		} else {
			if ( file1_flag == 0 ) {
				result.file1 = malloc ( strlen ( *argv ) + 1 );
				strcpy ( result.file1 , *argv );
				file1_flag = 1;
			} else {
				if ( msg_type == OP_KEYGEN || msg_type == OP_INVKEY || msg_type == OP_HISTO || msg_type ==  OP_SOLVE ) {
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
