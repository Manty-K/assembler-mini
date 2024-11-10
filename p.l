%{
#include "p.tab.h"
#include "utils.h"
char *label;
char *value;
%}

SECTION "section"
NUMBER 	[0-9]+
ALPNUM	[a-zA-Z_]+[a-zA-Z0-9_]*
SPACE	[ \t]+
RES	"res"[bwtqd]
%%

;.*				{}
{SECTION}{SPACE}".data"		{return SEC_DATA;}
{SECTION}{SPACE}".bss"		{return SEC_BSS;}
d[bwdq]				{
						yylval.i = sizeFromChar(yytext[1]);
						return D_TYPE;
					}	

res[bwdq]			{
						yylval.i = sizeFromChar(yytext[3]);
						return B_TYPE;
					}	

,				{return COMMA;}
{ALPNUM}			{label = strdup(yytext); return LABEL;}
\"([^\"]*)\"			{
					yylval.s = strndup(yytext+1,yyleng-2);
    				return STRING; 
				}
{NUMBER}			{yylval.i = atoi(yytext); return VALUE;}
\n		{return NEWLINE;}
{SPACE}		{}
.		{}
%%
