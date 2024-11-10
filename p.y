%{
#include<stdio.h>
void yyerror(const char * e);
int yylex();
void newline();
void printLocation(long loc);

long locd  = 0; // data LC
long locb = 0;
long loct = 0;
%}

%token SEC_DATA SEC_BSS SEC_TEXT LABEL VALUE DB_TYPE DW_TYPE DD_TYPE DQ_TYPE DT_TYPE NEWLINE RESB_TYPE RESW_TYPE RESD_TYPE RESQ_TYPE REST_TYPE STRING COMMA COMMENT

%start lines

%%
lines: line lines
	| NEWLINE {newline();}lines
	|;

line : SEC_BSS {printf("Sec bss");} NEWLINE {newline();} bss_lines
  	| SEC_DATA {printf("Sec data");} NEWLINE {newline();} data_lines 
	;

data_lines: NEWLINE {newline();} data_lines
	|data_line NEWLINE {newline();} data_lines
	|
	;
data_line: LABEL DB_TYPE VALUE {printf("dd");}
	|LABEL DW_TYPE VALUE {printf("dw");}
	|LABEL DD_TYPE VALUE {printf("dd");}
	| LABEL DQ_TYPE VALUE {printf("dq");}
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

