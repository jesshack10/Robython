%{
	#include <stdio.h>
	#include <stdlib.h>	
	#include <string.h>
	#include "robython.tab.h" 	//The output of Bison
	using namespace std; 
	#define YY_DECL extern "C" int yylex()	
	int line_num=1; 	

%}

%option noyywrap
%option yylineno 

id [a-zA-Z][a-zA-Z0-9]*
string \"[^\"]*\"
number [0-9]+|[0-9]+\.[0-9]+

%%
"start"			{return START;}
"function"	 	{return FUNCTION;}
"var"			{return VAR;}
"bool"			{return BOOL;}
"text"			{return TEXT;}
"num"			{return NUM;}
"true"			{return TRUE;}
"false"			{return FALSE;}
"print"			{return PRINT;}
"if"			{return IF;}
"else"			{return ELSE;}
"for"			{return FOR;}
"while"			{return WHILE;}

"robot.getTemperature"	{return TEMPERATURE;}
"robot.getDistance"		{return DISTANCE;}
"robot.getRightLight"	{return R_LIGHT;}
"robot.getLeftLight"	{return L_LIGHT;}
"robot.turnOnLED"		{return ON_LED;}
"robot.turnOffLED"		{return OFF_LED;}
"robot.forward"			{return FORWARD;}
"robot.back"			{return BACK;}
"robot.left"			{return LEFT;}
"robot.right"			{return RIGHT;}
"robot.buzzer"			{return BUZZER;}
"robot.rgb"				{return RGB;}

"DO"		{return DO;}
"DO+"		{return DO_P;}
"RE"		{return RE;}
"RE+"		{return RE_P;}
"MI"		{return MI;}
"MI+"		{return MI_P;}
"FA"		{return FA;}
"FA+"		{return FA_P;}
"SOL"		{return SOL;}
"SOL+"		{return SOL_P;}
"LA"		{return LA;}
"LA+"		{return LA_P;}
"SI"		{return SI;}
"SI+"		{return SI_P;}

"+"		{return '+';}
"-"		{return '-';}
"*"		{return '*';}
"/"		{return '/';}
"%"		{return '%';}
"("		{return '(';}
")"		{return ')';}
"{"		{return '{';}
"}"		{return '}';}
"<"		{return '<';}
">"		{return '>';}
"=="	{return SAME;}
"="		{return EQUALS;}
"!="	{return NO_EQUAL;}
";"		{return PCOMA;}
","		{return ',';}
":"		{return ':';}
"&&"	{return AND;}
"||"	{return OR;}

{id}	{yylval.sval = strdup(yytext); return ID;} 
{string}	{yylval.sval = strdup(yytext); return STRING;} 
{number}	{yylval.sval = strdup(yytext);return NUMBER;}

\n 		{line_num++;}
[ \t]	{}

.		{printf("ERROR: TOKEN %s NOT ALLOWED in line_num: %d\n", yytext,line_num);} 
%% 
