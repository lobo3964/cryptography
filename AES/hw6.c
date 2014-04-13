#include "hw6.h"

int main ( int argc, char **argv )
{
	Argvs cmd = ParseCommandLine ( argc, argv );
	if ( cmd.mode == OP_tablecheck ) {
		if ( tablecheck ( cmd.t ) != 0 ) {
			printerror ( "unknow tablecheck error" );
		}
		free ( cmd.t );
	} else if ( cmd.mode == OP_modprod ) {
		if ( modproduct ( cmd.p1, cmd.p2 ) != 0 ) {
			printerror ( "unknow modprod error" );
		}
		free ( cmd.p1 );
		free ( cmd.p2 );
	} else if ( cmd.mode == OP_keyexpand ) {
		if ( KeyExp ( cmd.k, cmd.t , YES ) != 0 ) {
			printerror ( "unkown keyexpand error" );
		}
		free ( cmd.t );
		free ( cmd.k );
	} else if ( cmd.mode == OP_encrypt ) {

	} else if ( cmd.mode == OP_decrypt ) {

	} else if ( cmd.mode == OP_inverse ) {

	} else {
		printerror ( "Unknown mode error" );
	}
	return 0;
}
