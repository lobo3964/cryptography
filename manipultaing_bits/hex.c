#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hw1.h"
/*
 * reads from file and hexdump to stdout
 * if file is not specfied the program will reads from stdin
 * return 1 on success, otherwise print to stderr
 */
int hexdump ( char *filepath )
{
	int n= 0, total=0;
	FILE *fp=fileopen ( filepath );
	unsigned char *inbuf = malloc ( MAX_HEXDUMP_BUF );
	while ( !feof ( fp ) ) {
		memset ( inbuf,0,MAX_HEXDUMP_BUF );
		n=fread ( inbuf,1,MAX_HEXDUMP_BUF,fp );
		
		/*
		 * BUG FIX: if n=0 means previous read just read before the EOF
		 * and this time fread() would read nothing.
		 * Therefore need to make sure that n>0
		 */
		if ( n>0 ) {
			int i=0;
			//print line number
			fprintf ( stdout,"%06x: ",total );
			//print single hex string, if single char, padd with 0
			for ( i=0; i<n; i++ ) {
				fprintf ( stdout,"%02x",inbuf[i] );
				if ( ( i+1 ) %8==0 ) {
					fprintf ( stdout,"  " );
				} else {
					fprintf ( stdout," " );
				}
			}
			//pad missing char with "--"
			for ( ; i<16; i++ ) {
				fprintf ( stdout,"--" );
				if ( ( i+1 ) %8==0 ) {
					fprintf ( stdout,"  " );
				} else {
					fprintf ( stdout," " );
				}
			}
			//print original string and replace chars
			for ( i=0; i<n; i++ ) {
				if ( inbuf[i]<=0x1f || inbuf[i]==0x7f ) {
					fprintf ( stdout,"." );
				} else if ( inbuf[i]>=0x80 ) {
					fprintf ( stdout,"~" );
				} else {
					fprintf ( stdout,"%c",inbuf[i] );
				}
			}
			//padding with space, really need this?
			for ( i=0; i< ( MAX_HEXDUMP_BUF-n ); i++ ) {
				fprintf ( stdout," " );
			}
			//add newline at the end
			fprintf ( stdout,"\n" );
			total=total+n;
		}
	}
	fclose ( fp );
	free ( inbuf );
	return 1;
}
