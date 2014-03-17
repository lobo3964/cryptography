#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hw1.h"
/*
 * CS531 HW1
 * Jie Gu
 * Implementing hexdump, base64 Encode and base64 Decode
 */
void command_process ( int argc, char **argv )
{
	if ( argc < 2 || argc > 3 ) {
		fprintf ( stderr, "ERR: Invalid arguments\nUsage: hexdump [file], enc-base64 [file], dec-base64 [file]\n" );
		exit ( -1 );
	} else {
		if ( strcmp ( argv[1], "hexdump" ) == 0 ) {
			if ( hexdump ( argv[2] ) == 1 )
				return;
		} else if ( strcmp ( argv[1], "enc-base64" ) == 0 ) {
			if ( b64main ( argv[2], "enc" ) == 1 )
				return;
		} else if ( strcmp ( argv[1], "dec-base64" ) == 0 ) {
			if ( b64main ( argv[2], "dec" ) == 1 )
				return;
		} else {
			fprintf ( stderr, "ERR: Invalid arguments\nUsage: hexdump [file], enc-base64 [file], dec-base64 [file]\n" );
			exit ( -1 );
		}
		fprintf ( stderr, "ERR: Unknown Error\n" );
		exit ( -1 );
	}
}


int main ( int argc, char **argv )

{
	command_process ( argc, argv );
	return 0;
}
