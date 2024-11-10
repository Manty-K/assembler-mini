#include<limits.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include "utils.h"

void parsenum(long  n,enum ByteType bt){

	if(bt == BYTE && (n < SCHAR_MIN  || n > SCHAR_MAX)){
		puts("Out of range");
		return;

	}

	if(bt == WORD && (n < SHRT_MIN  || n > SHRT_MAX)){
		puts("Out of range");
		return;

	}

	if(bt == DWORD && (n < INT_MIN  || n > INT_MAX)){
		puts("Out of range");
		return;

	}

	if(bt == QUAD && (n < LONG_MIN  || n > LONG_MAX)){
		puts("Out of range");
		return;

	}

	if(bt == BYTE){

		printf("%02lX",n);
	}else if(bt == WORD){

		printf("%04lX",n);
	}else if(bt == DWORD){

		printf("%08lX",n);
	}else if(bt == QUAD){

		printf("%06lX",n);
	}else{
		puts("Not Defined");
	}	

}
void parsestr(char * s ,enum ByteType bt ){

	int i = 0;	
	while( *(s+i) != 0){

		printf("%02X",*(s+i));
		i++;
	}

	int r = strlen(s) %  bt;

	if(bt > 1 && r != 0){
		for (i = 0 ; i< bt -r; i++){
			printf("00");
		}
	}
	//printf("\n");
}

