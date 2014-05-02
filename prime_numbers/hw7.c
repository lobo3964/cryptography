#include <stdio.h>

#include "hw7.h"

int main ( int argc, char **argv )
{
	Argvs cmd = ParseCommandLine ( argc, argv );
	if ( cmd.mode == OP_PRIME ) {
		if ( prime_gen ( cmd.n ) != 0 ) {
			printerror ( "unknow prime error" );
		}
		free ( cmd.n );
	} else if ( cmd.mode == OP_TRIALDIV ) {
		traildiv ( cmd.n, cmd.file_p );
		free ( cmd.n );
		free ( cmd.file_p );
	} else if ( cmd.mode == OP_MILLRABIN ) {
		millerrabin ( cmd.n, cmd.t, cmd.file_p, 0 );
		free ( cmd.n );
		free ( cmd.file_p );
	} else if ( cmd.mode == OP_RNDSEARCH ) {
		rndsearch ( cmd.k, cmd.t, cmd.file_p, cmd.file_r );
		free ( cmd.file_p );
		free ( cmd.file_r );
	} else if ( cmd.mode == OP_MAURER ) {

	} else {
		printerror ( "Unknown mode error" );
	}
	return 0;
}
