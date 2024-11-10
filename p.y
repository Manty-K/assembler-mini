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

%token SEC_DATA SEC_BSS SEC_TEXT NEWLINE COMMA 
%token <i> VALUE D_TYPE B_TYPE
%token <s> STRING LABEL

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
	| SEC_TEXT {printf("SECTION TEXT");}
	;

data_lines: data_line NEWLINE {newline();} data_lines
	| NEWLINE {newline();} data_lines
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


bss_lines: bss_line NEWLINE {newline();} bss_lines
	| NEWLINE {newline();} bss_lines
	|
	;

bss_line: LABEL B_TYPE VALUE {printLocation(locb);printf("<res %Xh>",$2 * $3); locb += ( $2 * $3);}
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

