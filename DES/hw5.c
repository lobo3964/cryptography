/**
 * DES main program
 * 
 * A table driven version of DES
 * Perform tablecheck and support EDE triplekey encryption
 * by: Jie Gu
 * march 26, 2014
 */
 
#include "hw5.h"
int main(int argc, char **argv){
	Argvs cmd = ParseCommandLine(argc, argv);
	if(cmd.mode==OP_TABLECHECK){
		if (tablecheck(cmd.t)!=0){
			printerror("unknow tablecheck error");
		}
		free(cmd.t);
	}else if(cmd.mode==OP_ENCRYPT || cmd.mode==OP_DECRYPT || cmd.mode==OP_ENCRYPT3 || cmd.mode==OP_DECRYPT3){
		tablecheck(cmd.t);
		if (cryptoDES(cmd.k, cmd.t, cmd.file1, cmd.mode)!=0){
			printerror("unkown DES error");
		}
		free(cmd.t);
		free(cmd.k);
		free(cmd.file1);	
	}else{
		printerror("Unknown mode error");
	}
	return 0;
}
