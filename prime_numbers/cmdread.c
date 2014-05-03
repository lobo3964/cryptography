/*
 * Read Command Line Argument
 * by Jie Gu
 * Reference: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
 */
#include "hw7.h"

void Usage()
{
	printerror (
	    "argument syntax error\n\
hw7 primes -n=maxval\n\
hw7 trialdiv -n=number -p=primesfile\n\
hw7 millerrabin -n=number -t=maxitr -p=primesfile\n\
hw7 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile\n\
hw7 maurer -k=numbits -p=primesfile -r=rndfile\n"	);
}




void checkarglimit ( int msg_type, int argc )
{
	if ( msg_type == OP_PRIME && argc != 2 ) {
		Usage();
	} else if ( msg_type == OP_TRIALDIV && argc != 3 ) {
		Usage();
	} else if ( msg_type == OP_MILLRABIN && argc != 4 ) {
		Usage();
	} else if ( msg_type == OP_RNDSEARCH && argc != 5 ) {
		Usage();
	} else if ( msg_type == OP_MAURER && argc != 4 ) {
		Usage();
	}
}

Argvs ParseCommandLine ( int argc, char **argv )
{
	int t_flag = 0;
	int k_flag = 0;
	int n_flag = 0;
	int file_p_flag = 0;
	int file_r_flag = 0;
	Argvs result;
	int msg_type = 0;
	argc--, argv++;
	if ( argc <= 0 ) {
		Usage();
	}

//BEGIN REFERENCE FROM: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
	if ( strcmp ( *argv, "primes" ) == 0 ) {
		msg_type = OP_PRIME;
	} else if ( strcmp ( *argv, "trialdiv" ) == 0 ) {
		msg_type = OP_TRIALDIV;
	} else if ( strcmp ( *argv, "millerrabin" ) == 0 ) {
		msg_type = OP_MILLRABIN;
	} else if ( strcmp ( *argv, "rndsearch" ) == 0 ) {
		msg_type = OP_RNDSEARCH;
	} else if ( strcmp ( *argv, "maurer" ) == 0 ) {
		msg_type = OP_MAURER;
	} else {
		Usage();
	}
	result.file_p=NULL;
	result.file_r=NULL;
	result.mode = msg_type;
	checkarglimit ( msg_type, argc );

	for ( argc--, argv++; argc > 0; argc--, argv++ ) {
		//before read [file]
		if ( *argv[0] == '-' && (file_p_flag == 0 || file_r_flag == 0)) {
			if ( strncmp ( *argv, "-k=" , OPTION_HEAD_LEN1 ) == 0 ) {
				if ( msg_type == OP_PRIME || msg_type == OP_TRIALDIV || msg_type == OP_MILLRABIN || k_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				pch++;
				allNum(pch);
				result.k = atoi ( pch );
				k_flag = 1;

			} else if ( strncmp ( *argv, "-t=" , OPTION_HEAD_LEN1 ) == 0 ) {
				//result.l= NULL;
				if ( (msg_type != OP_MILLRABIN && msg_type != OP_RNDSEARCH) || t_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				pch++;
				allNum(pch);
				result.t = atoi ( pch );
				t_flag = 1;

			} else if ( strncmp ( *argv, "-n=" , OPTION_HEAD_LEN1 ) == 0 ) {
				if ( msg_type == OP_RNDSEARCH || msg_type == OP_MAURER || n_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				++pch;
				allNum(pch);
				if ( pch[0] == 0x0 ) { //NULL value
					Usage();
				}
				result.n = malloc ( strlen ( pch ) + 1 );
				strcpy ( result.n , pch );
				n_flag = 1;
				
			} else if ( strncmp ( *argv, "-p=" , OPTION_HEAD_LEN1 ) == 0 ) {
				//result.l= NULL;
				if ( msg_type == OP_PRIME || file_p_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				++pch;
				if ( pch[0] == 0x0 ) { //NULL value
					Usage();
				}
				result.file_p = malloc ( strlen ( pch ) + 1 );
				strcpy ( result.file_p , pch );
				file_p_flag = 1;
				
			} else if ( strncmp ( *argv, "-r=" , OPTION_HEAD_LEN1 ) == 0 ) {
				//result.l= NULL;
				if ( (msg_type != OP_RNDSEARCH && msg_type != OP_MAURER) || file_r_flag == 1 ) {
					Usage();
				}
				char * pch;
				pch = strstr ( *argv, "=" );
				++pch;
				if ( pch[0] == 0x0 ) { //NULL value
					Usage();
				}
				result.file_r = malloc ( strlen ( pch ) + 1 );
				strcpy ( result.file_r , pch );
				file_r_flag = 1;			
	
			} else {
				Usage();
			}

			//[file] is before arguments
		} else if ( *argv[0] == '-' && (file_p_flag == 1 || file_r_flag == 1)) {
			Usage();
		} else {
			Usage();
		}
//END REFERENCE FROM: http://merlot.usc.edu/cs531-s14/misc/prog-faq/#howto_parse
	}
	return result;
}
