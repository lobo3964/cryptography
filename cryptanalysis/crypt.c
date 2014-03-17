/*
 * Cryptanalysis-Crypt
 * crypt a file with provided keyfile
 * Also suitable for decrypt with inverse keyfile
 *
 * by Jie Gu
 * Feb 26, 2014
 */
#include "hw4.h"

int crypt ( char* key, char* in_file )
{
	/*
	 * Begin preprocess:
	 * 		determine keysize
	 * 		calculate period
	 * 		dynamic alloc mem
	 * 		copy key into mem
	 */
	long keysize_byte;
	int period, index, retval;
	char cur;
	char *key_store;
	FILE* key_fp = fileopen ( key );
	FILE* in_fp = fileopen ( in_file );
	if ( fseek ( key_fp, 0, SEEK_END ) != 0 ) {
		printerror ( "seeking end of file failed" );
	}
	keysize_byte = ftell ( key_fp );
	if (keysize_byte%(KEYGEN_SPACE_SIZE+1)!=0){
		printerror("key file invalid");
	}
	period = keysize_byte / (KEYGEN_SPACE_SIZE+1);
	key_store=malloc(keysize_byte*sizeof(char));
	if ( fseek ( key_fp, 0, SEEK_SET ) != 0 ) {
		printerror ( "seeking begin of file failed" );
	}
	retval = fread(key_store,sizeof(char),keysize_byte,key_fp);
	if (retval!=keysize_byte){
		printerror("key file read error");
	}
	fclose(key_fp);
	
	/*
	 * done with preprocess, start crypt
	 */
	index=0;
	while(feof(in_fp)==0){
		cur=fgetc(in_fp);
		if (cur==0xA){
			break;
		}else if(cur<0x20 || cur>0x7e){
			printerror("illegal char detected");
		}else if(cur>=0x61 && cur<=0x7a){
			int curchar=cur-'a';
			int line=index%period;
			fprintf(stdout,"%c",key_store[line*(KEYGEN_SPACE_SIZE+1)+curchar]);
		}else{
			fprintf(stdout,"%c",cur);
		}
		index++;
	}
	fprintf(stdout,"\n");
	fclose(in_fp);
	free(key_store);
	return 0;
	

}
