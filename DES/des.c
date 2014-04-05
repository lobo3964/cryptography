/**
 * Implementation of DES algorithm
 * Function to call: cryptoDES
 * support single key DES and EDE triplekey encryption
 * Detecte weak key/semiweak key
 */
 
#include "hw5.h"
#include <ctype.h>
#define FORMAT_CD		1
#define FORMAT_LR		2

void print_hex ( int sourceC[], int sourceD[], int size, int cycleNum , int format )
{
	int cycle = size / 4;
	int i = 0;
	if ( format == FORMAT_CD ) {
		fprintf ( stderr, "(C%d,D%d)=", cycleNum, cycleNum );
	} else if ( format == FORMAT_LR ) {
		fprintf ( stderr, "(L%d,R%d)=", cycleNum, cycleNum );
	} else {
		printerror ( "invalid print format" );
	}
	for ( i = 0; i < cycle; i++ ) {
		int num = sourceC[i * 4] * ( 2 * 2 * 2 );
		num += sourceC[i * 4 + 1] * ( 2 * 2 );
		num += sourceC[i * 4 + 2] * ( 2 );
		num += sourceC[i * 4 + 3] * ( 1 );
		fprintf ( stderr, "%x", num );
	}
	for ( i = 0; i < cycle; i++ ) {
		int num = sourceD[i * 4] * ( 2 * 2 * 2 );
		num += sourceD[i * 4 + 1] * ( 2 * 2 );
		num += sourceD[i * 4 + 2] * ( 2 );
		num += sourceD[i * 4 + 3] * ( 1 );
		fprintf ( stderr, "%x", num );
	}
	fprintf ( stderr, "\n" );
}

//convert input char into hex value instead of ASCII value
int charToHex ( char c )
{
	if ( c >= '0' && c <= '9' ) {
		return c - '0';
	} else {
		return c - 'a' + 10;
	}
}

void subKeys ( int *subkey, int *previousC, int *previousD, int shift, int *PC2, int cycleNum , int print )
{
	int i = 0;
	int *currentC = malloc ( sizeof ( int ) * 28 );
	int *currentD = malloc ( sizeof ( int ) * 28 );
	if ( shift == 1 ) {
		memcpy ( currentC, previousC + 1, 27 * sizeof ( int ) );
		memcpy ( currentD, previousD + 1, 27 * sizeof ( int ) );
		memcpy ( currentC + 27, previousC, 1 * sizeof ( int ) );
		memcpy ( currentD + 27, previousD, 1 * sizeof ( int ) );
	} else if ( shift == 2 ) {
		memcpy ( currentC, previousC + 2, 26 * sizeof ( int ) );
		memcpy ( currentD, previousD + 2, 26 * sizeof ( int ) );
		memcpy ( currentC + 26, previousC, 2 * sizeof ( int ) );
		memcpy ( currentD + 26, previousD, 2 * sizeof ( int ) );
	} else {
		printerror ( "invalid shift argument" );
	}
	if ( print == 1 ) {
		print_hex ( currentC, currentD, 28, cycleNum , FORMAT_CD );
	}
	memcpy ( previousC, currentC, sizeof ( int ) * 28 );
	memcpy ( previousD, currentD, sizeof ( int ) * 28 );
	free ( currentC );
	free ( currentD );
	//print subkey
	int *CD = malloc ( sizeof ( int ) * 57 );
	//int *subkey = malloc ( sizeof ( int ) * 48 );
	memcpy ( CD + 1, previousC, sizeof ( int ) * 28 );
	memcpy ( CD + 29, previousD, sizeof ( int ) * 28 );
	for ( i = 0; i < 48; i++ ) {
		( subkey + i ) [0] = ( CD + ( PC2 + i ) [0] ) [0];
	}

	if ( print == 1 ) {
		fprintf ( stderr, "k%d=", cycleNum );
		for ( i = 0; i < 12; i++ ) {
			int num = ( subkey + i * 4 ) [0] * ( 2 * 2 * 2 );
			num += ( subkey + i * 4 + 1 ) [0] * ( 2 * 2 );
			num += ( subkey + i * 4 + 2 ) [0] * ( 2 );
			num += ( subkey + i * 4 + 3 ) [0] * ( 1 );
			fprintf ( stderr, "%x", num );
		}
		fprintf ( stderr, "\n" );
	}
	free ( CD );
	//free ( subkey );
}

void readLineGeneric ( int* dest, char* source )
{
	char* ptr = source;
	int count = 0;
	int digit10 = 10;//10 as NULL value
	int digit1 = 10;
	int num = 0;
	while ( ptr[0] != '=' ) {
		ptr++;
	}
	while ( ptr[0] != 0x0a ) {
		if ( ptr[0] == 0x2c ) {
			//determine num and
			//increment counter
			if ( digit1 == 10 ) {
				num = digit10;
			} else {
				num = digit10 * 10 + digit1;
			}
			( dest + count++ ) [0] = num;
			digit10 = 10;
			digit1 = 10;
		} else if ( ptr[0] >= '0' && ptr[0] <= '9' ) {
			if ( digit10 == 10 ) {
				digit10 = ptr[0] - '0';
			} else {
				digit1 = ptr[0] - '0';
			}
		}
		ptr++;
	}
	//add the last value
	if ( digit1 == 10 ) {
		num = digit10;
	} else {
		num = digit10 * 10 + digit1;
	}
	( dest + count ) [0] = num;
}

void feistelRound ( int* L_pre, int* R_pre, int *E, int *P, int **subkeys, int **S_box,  int round )
{
	int i = 0;
	int j = 0;
	int * temp = malloc ( sizeof ( int ) * 32 );
	int * expandR = malloc ( sizeof ( int ) * 48 );
	int * substitute = malloc ( sizeof ( int ) * 32 );
	int * permute = malloc ( sizeof ( int ) * 32 );
	memset ( expandR, 0, sizeof ( int ) * 48 );
	memset ( substitute, 0, sizeof ( int ) * 32 );
	memset ( permute, 0, sizeof ( int ) * 32 );
	memset ( temp, 0, sizeof ( int ) * 32 );

	//expand
	for ( i = 0; i < 48; i++ ) {
		( expandR + i ) [0] = ( R_pre + ( ( E + i ) [0] - 1 ) ) [0];
	}

	//XOR with key
	for ( i = 0; i < 48; i++ ) {
		( expandR + i ) [0] = ( expandR + i ) [0] ^ ( subkeys[round] + i ) [0];
	}

	//8 rounds of substitution, 6bits -> 4bits per round
	for ( i = 0; i < 8; i++ ) {
		int row = ( expandR + i * 6 ) [0] * 2 + ( expandR + i * 6 + 5 ) [0] * 1;
		int col = ( expandR + i * 6 + 1 ) [0] * 2 * 2 * 2 + ( expandR + i * 6 + 2 ) [0] * 2 * 2 + ( expandR + i * 6 + 3 ) [0] * 2 + ( expandR + i * 6 + 4 ) [0] * 1;
		int fourbits = ( S_box[i] + row * 16 + col ) [0];
		( substitute + j++ ) [0] = ( fourbits >> 3 ) & 0x1;
		( substitute + j++ ) [0] = ( fourbits >> 2 ) & 0x1;
		( substitute + j++ ) [0] = ( fourbits >> 1 ) & 0x1;
		( substitute + j++ ) [0] = ( fourbits ) & 0x1;
	}
	free ( expandR );

	//permute
	for ( i = 0; i < 32; i++ ) {
		( permute + i ) [0] = ( substitute + ( ( P + i ) [0] - 1 ) ) [0];
	}
	free ( substitute );

	//save R_pre to temp
	memcpy ( temp, R_pre, sizeof ( int ) * 32 );

	//XOR L_pre with permute and save to R_pre
	for ( i = 0; i < 32; i++ ) {
		( R_pre + i ) [0] = ( L_pre + i ) [0] ^ ( permute + i ) [0];
	}
	free ( permute );

	//dump temp to L_pre
	memcpy ( L_pre, temp, sizeof ( int ) * 32 );
	free ( temp );
}

int crypto ( char *result_buf, char* key, char *in_line, int* IP, int *iIP, int *E, int *P, int *V, int *PC1, int *PC2, int **S_box,  int mode, int print )
{
	//please run tablecheck piror running the program
	//FILE* tb_file = fileopen ( in_tb );
	//FILE* pt_file = fileopen ( in_pt );
	int keyspace[65];
	int *C = malloc ( sizeof ( int ) * 28 );
	int *D = malloc ( sizeof ( int ) * 28 );
	int *L = malloc ( sizeof ( int ) * 32 );
	int *R = malloc ( sizeof ( int ) * 32 );
	int *m64 = malloc ( sizeof ( int ) * 65 ); //as IP is 1 based, alloc 64+1 space
	int **keyArray;
	int *RL17;
	int *outbuf;
	//char *result_buf;
	int i = 0;
	int j = 0;
	int k = 0;
	//int DEScount = 0;
	//int DES_end_flag = 0;
	int count = 1;
	int digit = 0;
	int retkeycheck = 0;
	char localkey[16];
	//char *in_line = malloc ( sizeof ( char ) * 8 );
	//int table_linecount = TABLELINENUM;

	// START of CONVERT KEY INTO LOWERCASE
	char* key_ptr = key;
	for ( i = 0; i < 16; i++ ) {
		localkey[i] = tolower ( ( key_ptr++ ) [0] );
	}
	// END of CONVERT KEY INTO LOWERCASE

	// START KEYCHEK
	retkeycheck = keyCheck ( localkey );
	// END KEYCHECK

	//	START of CONVERT KEY INTO BITS
	for ( i = 0; i < 16; i++ ) {
		int c = charToHex ( localkey[i] );
		for ( j = 3; j >= 0; j-- ) {
			digit = ( c >> j ) & 0x1;
			keyspace[count++] = digit;
		}
	}
	//	END of CONVERT KEY INTO BITS


	//	START of GET C0,D0 FROM PC1
	j = 0;
	k = 0;
	for ( i = 0; i < 28; i++ ) {
		( C + i ) [0] = keyspace[ ( PC1 + i ) [0]];
		//int Ci=PC1[i];
		( D + i ) [0] = keyspace[ ( PC1 + i + 28 ) [0]];
		//int Di=PC1[i+28];
		k++;
	}
	if ( print == 1 ) {
		print_hex ( C, D, 28, 0 , FORMAT_CD );
	}
	//	END of GET C0,D0 FROM PC1

	//	START of 16 SUBKEYS LOOP
	keyArray = malloc ( 16 * sizeof ( int * ) );
	for ( i = 0; i < 16; i++ ) {
		keyArray[i] = malloc ( 48 * sizeof ( int ) );
	}
	if ( retkeycheck == WEAK_KEY ) {
		fprintf ( stderr, "Weak Key detected:\n" );
	} else if ( retkeycheck == SEMI_WEAK_KEY ) {
		fprintf ( stderr, "Semi-weak Key detected:\n" );
	}
	for ( i = 0; i < 16; i++ ) {
		subKeys ( keyArray[i], C, D, ( V + i ) [0], PC2, i + 1, print );
	}
	free ( C );
	free ( D );
	if ( retkeycheck != GOOD_KEY ) {
		printerror ( "program exit..." );
	}
	//	END of 16 SUBKEYS LOOP

	// START of DES cycle
	RL17 = malloc ( sizeof ( int ) * 65 );
	outbuf = malloc ( sizeof ( int ) * 64 );

	//for ( DEScount = 0; DEScount < filesize; DEScount++ ) {
	//fprintf(stdout, "DES round: %d\n", DEScount);
	memset ( RL17, 0, sizeof ( int ) * 65 );

	//	START of L0,R0
	//memset ( in_line, 0, 8 );
	memset ( m64, 0, sizeof ( int ) * 65 );
	//count = fread ( in_line, 1, 8, pt_file );
	//if ( count == 0 ) {
	//	break;
	//} else if ( count < 8 ) {
	//	DES_end_flag = 1;
	//}

	k = 1;
	//convert 8 bytes into 64 bits
	for ( i = 0; i < 8; i++ ) {
		char c = ( in_line + i ) [0];
		for ( j = 7; j >= 0; j-- ) {
			( m64 + k++ ) [0] = ( c >> j ) & 0x1;
		}
	}
	//initial permutation
	for ( i = 0; i < 32; i++ ) {
		( L + i ) [0] = ( m64 + ( IP + i ) [0] ) [0];
		( R + i ) [0] = ( m64 + ( IP + i + 32 ) [0] ) [0];
	}
	if ( print == 1 ) {
		print_hex ( L, R, 32, 0, FORMAT_LR );
	}

	//	END of L0,R0

	//16 round of regular swap
	if ( mode == OP_ENCRYPT ) {
		for ( i = 0; i < 16; i++ ) {
			feistelRound ( L, R, E, P, keyArray, S_box, i );
			if ( print == 1 ) {
				print_hex ( L, R, 32, i + 1, FORMAT_LR );
			}
		}
	} else if ( mode == OP_DECRYPT ) {
		for ( i = 15; i >= 0; i-- ) {
			feistelRound ( L, R, E, P, keyArray, S_box, i );
			if ( print == 1 ) {
				print_hex ( L, R, 32, 16 - i, FORMAT_LR );
			}
		}
	} else {
		printerror ( "invalid crypto mode" );
	}

	//last round of iregullar swap
	memcpy ( RL17 + 1, R, sizeof ( int ) * 32 );
	memcpy ( RL17 + 33, L, sizeof ( int ) * 32 );

	for ( i = 0; i < 64; i++ ) {
		( outbuf + i ) [0] = ( RL17 + ( ( iIP + i + 1 ) [0] ) ) [0];
	}
	//result_buf=malloc ( sizeof ( char ) * 8 );
	for ( i = 0; i < 8; i++ ) {
		int num = 0;
		for ( j = 0; j < 8; j++ ) {
			num = num | ( ( outbuf + i * 8 + j ) [0] << ( 7 - j ) );
		}
		( result_buf + i ) [0] = num;
		//fprintf ( stdout, "%c", num );
	}
	//DEScount++;

	// END of DES cycle

	// clean memory
	//fprintf ( stdout, "\n" );
	for ( i = 0; i < 16; i++ ) {
		free ( keyArray[i] );
	}
	free ( keyArray );
	free ( RL17 );
	free ( outbuf );
	free ( L );
	free ( R );
	free ( m64 );
	//fclose ( pt_file );
	return 0;
}

int keyFormatCheck ( char *key, int mode )
{
	int len=0;
	int i=0;
	if ( mode == OP_ENCRYPT3 || mode == OP_DECRYPT3 ) {
		len=48;
	} else if ( mode == OP_ENCRYPT || mode == OP_DECRYPT ) {
		len=16;
	}
	char *ptr=key;
	int temp;
	for (i=0;i<len;i++){
		temp=tolower(ptr[0]);
		if ( (temp>=0x30 && temp<=0x39)|| (temp>=0x61 && temp<=0x66)){
			ptr++;
		}else{
			return -1;
		}
	}
	return 0;
}

int cryptoDES ( char* key, char* in_tb, char* in_pt , int mode )
{
	FILE* tb_file = fileopen ( in_tb );
	FILE* pt_file = fileopen ( in_pt );
	int IP[64];
	int *iIP;
	int E[48];
	int P[32];
	int **S_box;
	int V[16];
	int PC1[56];
	int PC2[48];
	int count = 0;
	int DES_end_flag = 0;
	int print_flag;
	char *printbuffer;
	char *in_line = malloc ( sizeof ( char ) * 8 );
	int i = 0;
	int table_linecount = TABLELINENUM;
	char *key1;
	char *key2;
	char *key3;
	
	if (keyFormatCheck(key, mode)!=0){
		printerror ( "invalid key format" );
	}
	
	//	START of READING TABLEFILES
	S_box = malloc ( 8 * sizeof ( int * ) );
	for ( i = 0; i < 8; i++ ) {
		S_box[i] = malloc ( 64 * sizeof ( int ) );
	}
	while ( table_linecount > 0 ) {
		char* LINE = malloc ( sizeof ( char ) * READLEN );
		memset ( LINE, 0, sizeof ( char ) *READLEN );
		if ( fgets ( LINE, READLEN, tb_file ) == NULL ) {
			printerror ( "read line error" );
		}
		if ( strncmp ( LINE, "IP=", 3 ) == 0 ) {
			readLineGeneric ( IP, LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "E=", 2 ) == 0 ) {
			readLineGeneric ( E, LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "P=", 2 ) == 0 ) {
			readLineGeneric ( P, LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "S1=", 3 ) == 0 ) {
			readLineGeneric ( S_box[0], LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "S2=", 3 ) == 0 ) {
			readLineGeneric ( S_box[1], LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "S3=", 3 ) == 0 ) {
			readLineGeneric ( S_box[2], LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "S4=", 3 ) == 0 ) {
			readLineGeneric ( S_box[3], LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "S5=", 3 ) == 0 ) {
			readLineGeneric ( S_box[4], LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "S6=", 3 ) == 0 ) {
			readLineGeneric ( S_box[5], LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "S7=", 3 ) == 0 ) {
			readLineGeneric ( S_box[6], LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "S8=", 3 ) == 0 ) {
			readLineGeneric ( S_box[7], LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "V=", 2 ) == 0 ) {
			readLineGeneric ( V, LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "PC1=", 4 ) == 0 ) {
			readLineGeneric ( PC1, LINE );
			table_linecount--;
		} else if ( strncmp ( LINE, "PC2=", 4 ) == 0 ) {
			readLineGeneric ( PC2, LINE );
			table_linecount--;
		}
		free ( LINE );
	}
	fclose ( tb_file );
	//	END of READING TABLEFILES

	//	START of inverse IP
	iIP = malloc ( sizeof ( int ) * 65 );
	for ( i = 0; i < 64; i++ ) {
		( iIP + IP[i] ) [0] = i + 1;
	}
	//	END of inverse IP

	printbuffer = malloc ( sizeof ( char ) * 8 );
	print_flag = 1;

	if ( mode == OP_ENCRYPT3 || mode == OP_DECRYPT3 ) {
		key1 = malloc ( sizeof ( char ) * 16 );
		key2 = malloc ( sizeof ( char ) * 16 );
		key3 = malloc ( sizeof ( char ) * 16 );
		memcpy ( key1, key, 16 );
		memcpy ( key2, key + 16, 16 );
		memcpy ( key3, key + 32, 16 );
	}

	if ( mode == OP_ENCRYPT || mode == OP_DECRYPT ) {
		while ( DES_end_flag == 0 ) {
			memset ( in_line, 0, 8 );
			memset ( printbuffer, 0, 8 );
			count = fread ( in_line, 1, 8, pt_file );
			if ( count == 0 ) {
				break;
			} else if ( count < 8 ) {
				DES_end_flag = 1;
			}
			crypto ( printbuffer, key, in_line, IP, iIP, E, P, V, PC1, PC2, S_box, mode, print_flag-- );

			//dump printbuffer to stdout
			for ( i = 0; i < 8; i++ ) {
				fprintf ( stdout, "%c", ( printbuffer + i ) [0] );
			}
		}
	} else if ( mode == OP_ENCRYPT3 ) {
		while ( DES_end_flag == 0 ) {
			memset ( in_line, 0, 8 );
			memset ( printbuffer, 0, 8 );
			count = fread ( in_line, 1, 8, pt_file );
			if ( count == 0 ) {
				break;
			} else if ( count < 8 ) {
				DES_end_flag = 1;
			}
			crypto ( printbuffer, key1, in_line, IP, iIP, E, P, V, PC1, PC2, S_box, OP_ENCRYPT, print_flag-- );
			crypto ( printbuffer, key2, printbuffer, IP, iIP, E, P, V, PC1, PC2, S_box, OP_DECRYPT, print_flag-- );
			crypto ( printbuffer, key3, printbuffer, IP, iIP, E, P, V, PC1, PC2, S_box, OP_ENCRYPT, print_flag-- );

			//dump printbuffer to stdout
			for ( i = 0; i < 8; i++ ) {
				fprintf ( stdout, "%c", ( printbuffer + i ) [0] );
			}
		}
	} else if ( mode == OP_DECRYPT3 ) {
		while ( DES_end_flag == 0 ) {
			memset ( in_line, 0, 8 );
			memset ( printbuffer, 0, 8 );
			count = fread ( in_line, 1, 8, pt_file );
			if ( count == 0 ) {
				break;
			} else if ( count < 8 ) {
				DES_end_flag = 1;
			}
			crypto ( printbuffer, key3, in_line, IP, iIP, E, P, V, PC1, PC2, S_box, OP_DECRYPT, print_flag-- );
			crypto ( printbuffer, key2, printbuffer, IP, iIP, E, P, V, PC1, PC2, S_box, OP_ENCRYPT, print_flag-- );
			crypto ( printbuffer, key1, printbuffer, IP, iIP, E, P, V, PC1, PC2, S_box, OP_DECRYPT, print_flag-- );

			//dump printbuffer to stdout
			for ( i = 0; i < 8; i++ ) {
				fprintf ( stdout, "%c", ( printbuffer + i ) [0] );
			}
		}
	} else {
		printerror ( "invalid DES mode" );
	}



	if ( mode == OP_ENCRYPT3 || mode == OP_DECRYPT3 ) {
		free ( key1 );
		free ( key2 );
		free ( key3 );
	}
	for ( i = 0; i < 8; i++ ) {
		free ( S_box[i] );
	}
	free ( S_box );
	free ( in_line );
	fclose ( pt_file );
	free ( iIP );
	free ( printbuffer );

	return 0;
}
