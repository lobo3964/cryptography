/*
 * AES 128 ECB Demo
 * Perform decrypt and encrypt for the first 128 bits
 * and print the internal states
 * 
 * By Jie Gu
 * April 17, 2014
 */
#include "hw6.h"

//begin the awesomeness!

unsigned char IS[256];

void AddRoundKey ( unsigned char state[16], int w_begin, int w_stop )
{
	int state_col;
	int state_row;
	int w_row;
	//refer to FIPS-197 appendix B, otherwise its mindfuck
	for ( state_col = 0, w_row = w_begin; state_col < 4; state_col++, w_row++ ) {
		//w_row = w_begin;
		for ( state_row = 0; state_row < 4; state_row++ ) {
			state[state_col * 4 + state_row] = state[state_col * 4 + state_row] ^ w[w_row][state_row];
		}
		//w_row++;
	}
}
void ShiftRows ( unsigned char state[16] )
{
	int i = 0;
	unsigned char temp;
	//row 1
	temp = state[1];
	state[1] = state[5];
	state[5] = state[9];
	state[9] = state[13];
	state[13] = temp;

	//row2
	for ( i = 0; i < 2; i++ ) {
		temp = state[2];
		state[2] = state[6];
		state[6] = state[10];
		state[10] = state[14];
		state[14] = temp;
	}

	//row3
	for ( i = 0; i < 3; i++ ) {
		temp = state[3];
		state[3] = state[7];
		state[7] = state[11];
		state[11] = state[15];
		state[15] = temp;
	}
}
void InvShiftRows ( unsigned char state[16] )
{
	int i = 0;
	unsigned char temp;
	//row 1
	temp = state[13];
	state[13] = state[9];
	state[9] = state[5];
	state[5] = state[1];
	state[1] = temp;

	//row2
	for ( i = 0; i < 2; i++ ) {
		temp = state[14];
		state[14] = state[10];
		state[10] = state[6];
		state[6] = state[2];
		state[2] = temp;
	}

	//row3
	for ( i = 0; i < 3; i++ ) {
		temp = state[15];
		state[15] = state[11];
		state[11] = state[7];
		state[7] = state[3];
		state[3] = temp;
	}
}
void SubBytes ( unsigned char state[16] )
{
	int i = 0;
	for ( i = 0; i < 16; i++ ) {
		state[i] = S[ ( int ) ( state[i] )];
	}
}
void InvSubBytes ( unsigned char state[16] )
{
	int i = 0;
	for ( i = 0; i < 16; i++ ) {
		state[i] = IS[ ( int ) ( state[i] )];
	}
}
void MixColumns ( unsigned char state[16] )
{
	int i = 0;
	int j = 0;
	unsigned char a[4] = {P[0], P[1], P[2], P[3]};
	unsigned char s[4];
	unsigned char res[4];
	for ( i = 0; i < 4; i++ ) {
		for ( j = 0; j < 4; j++ ) {
			s[j] = state[4 * i + ( 3 - j )];
		}
		modprod ( a, s, res );
		for ( j = 0; j < 4; j++ ) {
			state[4 * i + j] = res[3 - j];
		}
	}
}
void InvMixColumns ( unsigned char state[16] )
{
	int i = 0;
	int j = 0;
	unsigned char ia[4] = {IP[0], IP[1], IP[2], IP[3]};
	unsigned char s[4];
	unsigned char res[4];
	for ( i = 0; i < 4; i++ ) {
		for ( j = 0; j < 4; j++ ) {
			s[j] = state[4 * i + ( 3 - j )];
		}
		modprod ( ia, s, res );
		for ( j = 0; j < 4; j++ ) {
			state[4 * i + j] = res[3 - j];
		}
	}
}
void printKey ( int round, char* string, int start )
{
	int i = 0;
	int j = 0;
	fprintf ( stdout, "round[%2d]%-10s", round, string );
	for ( i = 0; i < 4; i++ ) {
		for ( j = 0; j < 4; j++ ) {
			fprintf ( stdout, "%02x", w[start + i][j] );
		}
	}
	fprintf ( stdout, "\n" );
}

void print16 ( unsigned char array[16], char* string, int round )
{
	int i = 0;
	fprintf ( stdout, "round[%2d]%-10s", round, string );
	for ( i = 0; i < 16; i++ ) {
		fprintf ( stdout, "%02x", array[i] );
	}
	fprintf ( stdout, "\n" );
}

//FIPS-197
void AES_encrypt ( unsigned char state[16] )
{
	int round = 0;
	print16 ( state, ".input\0", round );
	AddRoundKey ( state, round, 3 );
	printKey ( round, ".k_sch\0", 0 );
	for ( round = 1; round < 10; round++ ) {
		print16 ( state, ".start\0", round );
		SubBytes ( state );
		print16 ( state, ".s_box\0", round );
		ShiftRows ( state );
		print16 ( state, ".s_row\0", round );
		MixColumns ( state );
		print16 ( state, ".m_col\0", round );
		AddRoundKey ( state, round * 4, ( round + 1 ) * 4 - 1 );
		printKey ( round, ".k_sch\0", round * 4 );
	}
	print16 ( state, ".start\0", round );
	SubBytes ( state );
	print16 ( state, ".s_box\0", round );
	ShiftRows ( state );
	print16 ( state, ".s_row\0", round );
	AddRoundKey ( state, 40, 43 );
	printKey ( round, ".k_sch\0", 40 );
	print16 ( state, ".output\0", round );
}

void AES_decrypt ( unsigned char state[16] )
{
	//calculate invSbox
	int i = 0;
	for ( i = 0; i < 256; i++ ) {
		IS[S[i]] = i;
	}

	int count = 0;
	int round = 0;
	print16 ( state, ".iinput\0", round );
	AddRoundKey ( state, 40, 43 );
	printKey ( round++, ".ik_sch\0", 40 );
	for ( count = 9; count > 0; count--,round++ ) {
		
		print16 ( state, ".istart\0", round );
		
		InvShiftRows ( state );
		print16 ( state, ".is_row\0", round );
		
		InvSubBytes ( state );
		print16 ( state, ".is_box\0", round );
		
		printKey ( round, ".ik_sch\0", count * 4 );
		AddRoundKey ( state, count * 4, ( count + 1 ) * 4 - 1 );
		print16 ( state, ".ik_add\0", round );
		
		InvMixColumns ( state );
	}
	print16 ( state, ".istart\0", round );
	InvShiftRows ( state );
	print16 ( state, ".is_row\0", round );
	InvSubBytes ( state );
	print16 ( state, ".is_box\0", round );
	//print16 ( state, ".im_col\0", round );
	printKey ( round, ".ik_sch\0", 0 );
	AddRoundKey ( state, 0, 3 );
	print16 ( state, ".ioutput\0", round );
}


int AES ( char *file_in, char *key, char *table , int mode )
{
	unsigned char * state = malloc ( sizeof ( unsigned char ) * 16 );
	int retval;
	if ( KeyExp ( key, table , NO ) != 0 ) {
		printerror ( "unkown keyexpand error" );
	}
	FILE * input = fileopen ( file_in );
	retval = fread ( state, 1, 16, input );
	if ( retval != 16 ) {
		printerror ( "input file too small" );
	}
	if ( mode == OP_encrypt ) {
		AES_encrypt ( state );
	}
	if ( mode == OP_decrypt ) {
		AES_decrypt ( state );
	}
	free ( state );
	fclose ( input );
	return 0;
}

