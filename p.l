%{
#include "p.tab.h"
char *label;
char *value;
%}

SECTION "section"
NUMBER 	[0-9]+
ALPNUM	[a-zA-Z_]+[a-zA-Z0-9_]*
SPACE	[ \t]+
RES	"res"[bwtqd]
%%

;.*				{return COMMENT;}
{SECTION}{SPACE}".data"		{return SEC_DATA;}
{SECTION}{SPACE}".bss"		{return SEC_BSS;}
"db"				{return DB_TYPE;}
"dw"				{return DW_TYPE;}
"dd"				{return DD_TYPE;}
"dq"				{return DQ_TYPE;}
"dt"				{return DT_TYPE;}
"resb"				{return RESB_TYPE;}
"resw"				{return RESW_TYPE;}
"resd"				{return RESD_TYPE;}
"resq"				{return RESQ_TYPE;}
"rest"				{return REST_TYPE;}
,				{return COMMA;}
{ALPNUM}			{label = strdup(yytext); return LABEL;}
  /*\"([^\"]*)\"			{value = strdup(yytext); return STRING;} */
\"([^\"]*)\"			{
				
				strncpy(value, yytext + 1,yyleng-2); // strdup without the leading quote
    				 // remove the trailing quote
    				return STRING; 
				}
{NUMBER}			{value = strdup(yytext); return VALUE;}
\n		{return NEWLINE;}
{SPACE}		{}
.		{}
%%
