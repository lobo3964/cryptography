/*
 * CS531 HW2
 * by Jie Gu
 * Feb 11, 2014
 *
 * main function
 *
 * reads input arguments
 * and parse to apporiate function
 *
 * visual-cryptography: 	key stream generation
 * 							pbm file encryption
 * 							pbm files merge
 * 							pbm file decryption
 *
 */
#include "hw2.h"

int main ( int argc, char **argv )
{
	Argvs cmd = ParseCommandLine ( argc, argv );
	if ( cmd.mode == OP_STREAM ) {
		if ( stream ( cmd.pphrase, cmd.len, NULL, OP_STREAM ) != 0 ) {
			printerror ( "Unknown Stream error" );
		}
		free ( cmd.pphrase );
	} else if ( cmd.mode == OP_ENCRYPT ) {
		if ( encrypt ( cmd.pphrase, cmd.out, cmd.file1 ) != 0 ) {
			printerror ( "Unknown Encrypt error" );
		}
		free ( cmd.pphrase );
		free ( cmd.file1 );
		free ( cmd.out );
	} else if ( cmd.mode == OP_MERGE ) {
		if ( merge ( cmd.file1, cmd.file2 ) != 0 ) {
			printerror ( "unknown merge error" );
		}
		free ( cmd.file1 );
		free ( cmd.file2 );
	} else if ( cmd.mode == OP_DECRYPT ) {
		if ( decrypt ( cmd.file1 ) != 0 ) {
			printerror ( "Unknown decrypt error" );
		}
		free ( cmd.file1 );
	}

	return 0;
}
