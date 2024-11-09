%{
#include<stdio.h>
void yyerror(const char * e);
int yylex();
void newline();
%}

%token SEC_DATA SEC_BSS SEC_TEXT LABEL VALUE DB_TYPE DW_TYPE DD_TYPE DQ_TYPE DT_TYPE NEWLINE RESB_TYPE RESW_TYPE RESD_TYPE RESQ_TYPE REST_TYPE STRING COMMA COMMENT

%start lines

%%
lines: line lines
	| NEWLINE {newline();}lines
	|;

line : SEC_BSS {printf("Sec bss");} NEWLINE {newline();}
  	| SEC_DATA{printf("sec data");} NEWLINE {newline();} data_lines 
	;

data_lines: NEWLINE {newline();} data_lines
	|data_line NEWLINE {newline();} data_lines
	|
	;
data_line: LABEL DB_TYPE VALUE {printf("dl");}
	; 

%%

void newline(){
	printf("\n");
}

void yyerror(const char * e){

	printf("Hii: %s\n",e);
}

int main(){
	
	yyparse();
	return 0;

}

