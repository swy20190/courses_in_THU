%{

#include "prepare.h"
#include "yacc.tab.h"

%}

L	        ([_A-Za-z])
D		    ([0-9])
integer		({D}+)
identifier	({L}({L}|{D})*)
blank   	([ \f\r\n\t\v ])
string      ('(\\.|[^\\'])+'|\"(\\.|[^\\"])*\")

%%
#include([^\n])*                    {}

"for"                               {return(FOR);}
"while"                             {return(WHILE);}
"break"                             {return(BREAK);}
"continue"                          {return(CONTINUE);}
"if"                                {return(IF);}
"else"                              {return(ELSE);}

"int"                               {yylval.type = intType;return(INT);}
"char"                              {yylval.type = charType;return(CHAR);}
"double"                            {yylval.type = doubleType;return(DOUBLE);}

"return"		                    {return(RETURN);}
"printf"                            {return(PRINTF);}
"strlen"                            {return(STRLEN);}

{identifier}                        {yylval.strValue=yytext;return(IDENTIFIER);}
{string}                            {yylval.strValue=yytext;return(STRING);}
{integer}                           {yylval.intValue=atoi(yytext);return(INTEGER);}

{D}*"."{D}+                         { yylval.dblValue = atof(yytext); return(DOUBLE_NUM);}
{D}+"."{D}*                         { yylval.dblValue = atof(yytext); return(DOUBLE_NUM);}

">="                                {return(GE_OP);}
"<="			                    {return(LE_OP);}
"=="			                    {return(EQ_OP);}
"!="			                    {return(NE_OP);}
"&&"			                    {return(AND_OP);}
"||"			                    {return(OR_OP);}
"++"			                    {return(INC_OP);}
"--"			                    {return(DEC_OP);}

"-"                                 {return('-');}
"+"                                 {return('+');}
"*"                                 {return('*');}
"/"                                 {return('/');}
"%"                                 {return('%');}
"<"                                 {return('<');}
"."                                 {return('.');}
">"                                 {return('>');}
","                                 {return(',');}
":"                                 {return(':');}
";"                                 {return(';');}
"="                                 {return('=');}
"("                                 {return('(');}
")"                                 {return(')');}
"{"     		                    {return('{');}
"}"      		                    {return('}');}
"["      		                    {return('[');}
"]"      		                    {return(']');}
"!"                                 {return('!');}

{blank}		                        {}

.			                        {cout << "Unknown character" << endl;}

%%

int yywrap(void)
{
	return 1;							
}
