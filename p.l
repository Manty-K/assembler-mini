%{
	#include "p.tab.h"
	#include "utils.h"

%}
%option caseless

SECTION "section"
NUMBER 	[0-9]+
ALPNUM	[a-zA-Z_]+[a-zA-Z0-9_]*
SPACE	[ \t]+
RES	"res"[bwtqd]
REGS (e?[abcd]x|[abcd][lh]|e?[sb]p|e?[sd]i)

%%

;.*				{}
\"([^\"]*)\"			{
					yylval.s = strndup(yytext+1,yyleng-2);
    				return STRING; 
				}
{SECTION}{SPACE}".data"		{return SEC_DATA;}
{SECTION}{SPACE}".bss"		{return SEC_BSS;}
{SECTION}{SPACE}".text"		{return SEC_TEXT;}
d[bwdq]				{
						yylval.i = sizeFromChar(yytext[1]);
						return D_TYPE;
					}	

res[bwdq]			{
						yylval.i = sizeFromChar(yytext[3]);
						return B_TYPE;
					}	

{REGS}	{  lowercase(yytext); yylval.s =  strdup(yytext); return REG;}

,				{return COMMA;}
\+					{return PLUS;}
\-					{return MINUS;}

\[					{return LEFTBR;};
\]					{return RIGHTBR;};


MOV|ADD|INC|DEC|DIV|MUL|JMP|SUB|XOR|CMP|AND|OR|NOT|NEG|RET|NOP|JNE|JE|JNZ|JZ|INT					{lowercase(yytext); yylval.s = strdup(yytext); return OPC;}

global				{return GLOBAL;}



{NUMBER}			{yylval.l = atoi(yytext); return VALUE;}

0[Xx][0-9a-fA-F][0-9a-fA-F]*	{yylval.l= longFromHex(yytext); return HEXVAL;}
0[Bb][0-1][0-1]*				{yylval.l= longFromBin(yytext+2);return BINVAL;}

dword 						{return DWORD;}
{ALPNUM}			{yylval.s = strdup(yytext); return LABEL;}

\n		{return NEWLINE;}
{SPACE}		{}
.		{}
%%
