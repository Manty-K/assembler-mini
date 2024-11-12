%{
#include<stdio.h>
#include<string.h>
#include "utils.h"
#include "symb.h"
void yyerror(const char * e);
int yylex();
void newline();
void printLocation(long loc);

long locd  = 0; // data LC
long locb = 0;
long loct = 0;
int size;
%}

%token SEC_DATA SEC_BSS SEC_TEXT NEWLINE COMMA GLOBAL LEFTBR RIGHTBR PLUS MINUS
%token <i> D_TYPE B_TYPE
%token <l> HEXVAL BINVAL VALUE
%token <s> STRING LABEL REG OPC MEM

%start lines

%union{
	int i;
	long l;
	char *s;
}


%%
lines: line lines
	| NEWLINE {newline();}lines
	|;

line : SEC_BSS {printf("SECTION BSS");} NEWLINE {newline();} bss_lines
  	| SEC_DATA {printf("SECTION DATA");} NEWLINE {newline();} data_lines
	| SEC_TEXT {printf("SECTION TEXT");} NEWLINE {newline();} text_lines
	;

data_lines: data_line NEWLINE {newline();} data_lines
	| NEWLINE {newline();} data_lines
	|
	;
data_line: LABEL  D_TYPE {printLocation(locd); size = $2; addLabel($1,locd,'d');}values
	; 

values: val
	| val COMMA values
	;

val: VALUE {parsenum($1,size); locd += size;}
	| STRING { locd += parsestr($1,size);}
	;


bss_lines: bss_line NEWLINE {newline();} bss_lines
	| NEWLINE {newline();} bss_lines
	|
	;

bss_line: LABEL B_TYPE VALUE {printLocation(locb);printf("<res %lXh>",$2 * $3); addLabel($1,locb,'b'); locb += ( $2 * $3);}
	;


text_lines: text_line NEWLINE {newline();} text_lines
	| NEWLINE {newline();} text_lines
	|
	;

text_line: GLOBAL LABEL
	|{printLocation(loct);} LABEL inst 
	|{printLocation(loct);} inst
	;

inst: OPC REG COMMA REG {printf("%s= %s %s  rr",$1,$2,$4);loct += 2 ;}
	| OPC REG COMMA addr
	| OPC REG COMMA immd {printf("%s imm: ",$1);}
	;

addr : LEFTBR REG RIGHTBR {printf("reg only");loct += 2 ;}
	| LEFTBR REG PLUS VALUE RIGHTBR {printf("reg with +offset");loct += 2 ;}
	| LEFTBR REG MINUS VALUE RIGHTBR {printf("reg with -offset");loct += 2 ;}
	;

immd: VALUE {printf("v %ld ",$1);}
	| BINVAL {printf("h %ld ",$1);}
	| HEXVAL {printf("b %ld ",$1);}
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
	puts("\n\n");
	displaySymbolTable();
	return 0;

}

