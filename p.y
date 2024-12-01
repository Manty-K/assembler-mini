%{
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "utils.h"
#include "symb.h"
#include "modrm.h"
void yyerror(const char * e);
int yylex();
void newline();
void printLocation(long loc);

int pass;

long locd  = 0; // data LC
long locb = 0;
long loct = 0;
int size;
%}

%token SEC_DATA SEC_BSS SEC_TEXT NEWLINE COMMA GLOBAL LEFTBR RIGHTBR PLUS MINUS DWORD
%token <i> D_TYPE B_TYPE
%token <l> HEXVAL BINVAL VALUE
%token <s> STRING LABEL REG OPC MEM

%type <l> immd

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
	| LABEL {addLabel($1,loct,'t');} instg
	| instg
	;

instg: {printLocation(loct);}inst
	| NEWLINE {newline();} instg
	;

inst: OPC				{loct += onlyOp($1);}
	| OPC immd			{loct += opimm($1,$2);}
	| OPC PLUS immd			{loct += opimm($1,$3);}
	| OPC MINUS immd			{loct += opimm($1,$3 * -1);}
	| OPC LABEL			{loct += oplabel($1,$2,loct);}
	| OPC REG 			{loct += getYoo($1,$2);}
	| OPC REG COMMA REG {loct += tworeg($1,$2,$4);}
	| OPC LEFTBR REG RIGHTBR COMMA REG {loct += addrRegReg($1,$3,$6, (long) NULL);}
	| OPC LEFTBR REG  PLUS immd  RIGHTBR COMMA REG {loct += addrRegReg($1,$3,$8,$5);}
	| OPC LEFTBR REG  MINUS immd  RIGHTBR COMMA REG {loct += addrRegReg($1,$3,$8,$5 * -1);}
	| OPC REG COMMA immd {loct += regimm($1,$2,$4);}
	| OPC REG COMMA PLUS immd {loct += regimm($1,$2,$5);}
	| OPC REG COMMA MINUS immd {loct += regimm($1,$2,$5 * -1);}
	| OPC REG COMMA LABEL {loct += regLbl($1,$2,$4);}
	| OPC REG COMMA LEFTBR LABEL RIGHTBR {loct += regLblAddr($1,$2,$5,(long) NULL,0);}
	| OPC REG COMMA LEFTBR LABEL PLUS immd RIGHTBR {loct += regLblAddr($1,$2,$5,(long) NULL,$7);}
	| OPC REG COMMA LEFTBR LABEL MINUS immd RIGHTBR {loct += regLblAddr($1,$2,$5,(long) NULL,$7 * -1);}

	| OPC REG COMMA LEFTBR immd RIGHTBR {loct += regLblAddr($1,$2,NULL,$5,0);}
	| OPC REG COMMA LEFTBR  REG RIGHTBR {loct += regAddr($1,$2,$5, 0);}
	| OPC REG COMMA LEFTBR  REG PLUS immd RIGHTBR {loct += regAddr($1,$2,$5,$7);}
	| OPC REG COMMA LEFTBR  REG MINUS immd RIGHTBR {loct += regAddr($1,$2,$5,$7 * -1);}

	| OPC DWORD LEFTBR REG RIGHTBR    {loct += getYoo2($1,$4,(long)NULL);}
	| OPC DWORD LEFTBR REG PLUS immd RIGHTBR {loct += getYoo2($1,$4,$6);}
	| OPC DWORD LEFTBR REG MINUS immd RIGHTBR {loct += getYoo2($1,$4,$6 * -1);}
	| OPC DWORD LEFTBR REG RIGHTBR COMMA immd {loct += addrRegImmImm($1,$4,(long)NULL,$7);}
	| OPC DWORD LEFTBR REG PLUS immd RIGHTBR COMMA immd {loct += addrRegImmImm($1,$4,$6,$9);}
	| OPC DWORD LEFTBR REG MINUS immd RIGHTBR COMMA immd {loct += addrRegImmImm($1,$4,$6 * -1,$9);}
	| OPC DWORD LEFTBR REG RIGHTBR COMMA PLUS immd {loct += addrRegImmImm($1,$4,(long)NULL,$8);}
	| OPC DWORD LEFTBR REG PLUS immd RIGHTBR COMMA PLUS immd {loct += addrRegImmImm($1,$4,$6,$10);}
	| OPC DWORD LEFTBR REG MINUS immd RIGHTBR COMMA PLUS immd {loct += addrRegImmImm($1,$4,$6 * -1, $10);}
	| OPC DWORD LEFTBR REG RIGHTBR COMMA MINUS immd {loct += addrRegImmImm($1,$4,(long)NULL,$8 * -1);}
	| OPC DWORD LEFTBR REG PLUS immd RIGHTBR COMMA MINUS immd {loct += addrRegImmImm($1,$4,$6,$10 * -1);}
	| OPC DWORD LEFTBR REG MINUS immd RIGHTBR COMMA MINUS immd {loct += addrRegImmImm($1,$4,$6 * -1, $10 * -1);}
	;


immd: VALUE
	| BINVAL 
	| HEXVAL 
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

int main(int argc, char ** argv){
	if(argc>1){

		if(!strcmp(argv[1],"1")){
			pass = 1;
		}else if(!strcmp(argv[1],"2")){
			pass = 2;
		}else{
			printf("Invalid Pass : %s\n",argv[1]);
			return 1;			
		}
	}else{
		puts("Pass required");
			return 1;
	}

	if(pass == 2){
		importSymbolTable();
	}

	yyparse();

	if(pass == 1){
		saveSymbolTable();
	}

	return 0;

}

