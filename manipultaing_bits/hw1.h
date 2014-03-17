/*
 * define constants
 */
#define MAX_HEXDUMP_BUF		16
#define MAX_B64_ENC_BUF		3
#define MAX_B64_DEC_BUF		4
#define MAX_B64_LEN			64

/*
 * functions called
 */
int hexdump(char *file);
int b64main(char* filepath, char* mode);

/*
 * helper functions
 */ 
FILE* fileopen(char *filepath);
