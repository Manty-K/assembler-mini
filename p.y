%{
#include<stdio.h>
#include<string.h>
#include "utils.h"
void yyerror(const char * e);
int yylex();
void newline();
void printLocation(long loc);

long locd  = 0; // data LC
long locb = 0;
long loct = 0;
int size;
%}

%token SEC_DATA SEC_BSS SEC_TEXT LABEL DB_TYPE DW_TYPE DD_TYPE DQ_TYPE DT_TYPE NEWLINE RESB_TYPE RESW_TYPE RESD_TYPE RESQ_TYPE REST_TYPE COMMA COMMENT
%token <i> VALUE
%token <s> STRING
%token <i> D_TYPE

%start lines

%union{
	int i;
	char *s;
}


%%
lines: line lines
	| NEWLINE {newline();}lines
	|;

line : SEC_BSS {printf("SECTION BSS");} NEWLINE {newline();} bss_lines
  	| SEC_DATA {printf("SECTION DATA");} NEWLINE {newline();} data_lines 
	;

data_lines: NEWLINE {newline();} data_lines
	|data_line NEWLINE {newline();} data_lines
	|
	;
data_line: LABEL D_TYPE {printLocation(locd); size = $2;}values
	; 

values: val
	| val COMMA values
	;

val: VALUE {parsenum($1,size);locd += size;}
	| STRING { locd += parsestr($1,size);}
	;


bss_lines: NEWLINE {newline();} bss_lines
	|bss_line NEWLINE {newline();} bss_lines
	|
	;

bss_line: LABEL RESB_TYPE VALUE {printLocation(locb); printf("<res %Xh>",$3); locb += $3;}
	|  LABEL RESW_TYPE VALUE {printLocation(locb); printf("<res %Xh>",$3 * 2); locb += 2 * $3;}
	|  LABEL RESD_TYPE VALUE {printLocation(locb); printf("<res %Xh>",$3 * 4) ;  locb += 4 * $3;}
	|  LABEL RESQ_TYPE VALUE {printLocation(locb); printf("<res %Xh>",$3 * 8);  locb += 8 * $3;}
	;


%%

void newline(){
	printf("\n");
}

void printLocation(long loc){
	printf("%08lX ",loc);
}

void yyerror(const char * e){

	printf("Hii: %s\n",e);
}

int main(){
	
	yyparse();
	return 0;

}

