/*
 * Cryptanalysis-main program
 *
 * keygen	  :  	Generate a keyfile with a specified period to be used by the
 * 					full Vigenère cipher (with polyalphabetic substitution).
 * crypt	  :  	Encrypts (or decrypts) an input ASCII text file with the keys
 * 					specified in keyfile. The output is an ASCII text file encrypted
 * 					(or decrypted) using a full Vigenère cipher (with polyalphabetic substitution).
 * invkey	  :  	Create an inverse permutation key file from keyfile.
 * histo	  :  	Produce a histogram of ciphertext character frequencies
 * 					assuming the ciphertext is produced by a polyalphabetic
 * 					substitution cipher with period equals period. which specifies
 * 					a one-based index for the alphabets. The output must be sorted
 * 					according to the frequencies of the ciphertext characters in
 * 					descending order. Please do not consider any characters that is
 * 					not a lowercase English character (i.e., between 'a' and 'z', inclusive).
 * solve	  :  	Apply the Method of Kasiski and use Index of Coincidence in
 * 					order to determine the period of a ciphertext encrypted by
 * 					a full Vigenère cipher (with polyalphabetic substitution).
 *
 * by Jie Gu
 * Feb 26, 2014
 */
#include "hw4.h"
int main ( int argc, char **argv )
{
	Argvs cmd = ParseCommandLine ( argc, argv );
	if ( cmd.mode == OP_KEYGEN ) {
		if ( keygen ( cmd.t ) != 0 ) {
			printerror ( "Unknown keygen error" );
		}
	} else if ( cmd.mode == OP_CRYPT ) {
		if ( crypt ( cmd.file1, cmd.file2 ) != 0 ) {
			printerror ( "Unknown crypt error" );
		}
		free ( cmd.file1 );
		free ( cmd.file2 );
	} else if ( cmd.mode == OP_INVKEY ) {
		if ( invkey ( cmd.file1 ) != 0 ) {
			printerror ( "unknown invkey error" );
		}
		free ( cmd.file1 );
	} else if ( cmd.mode == OP_HISTO ) {
		if ( histo ( cmd.t, cmd.i, cmd.file1 ) != 0 ) {
			printerror ( "unknown histo error" );
		}
		free ( cmd.file1 );
	} else if ( cmd.mode == OP_SOLVE ) {
		if ( solve ( cmd.l, cmd.file1 ) != 0 ) {
			printerror ( "unknown solve error" );
		}
		free ( cmd.file1 );
	}
	return 0;
}
