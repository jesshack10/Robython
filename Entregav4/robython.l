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
"start"			{printf("Got START token in line_num: %d\n", line_num);return START;}
"function"	 	{printf("Got FUNCTION token in line_num: %d\n", line_num);return FUNCTION;}
"var"			{printf("Got VAR token in line_num: %d\n", line_num);  return VAR;}
"bool"			{printf("Got BOOL token in line_num: %d\n", line_num); return BOOL;}
"text"			{printf("Got TEXT token in line_num: %d\n", line_num); return TEXT;}
"num"			{printf("Got NUM token in line_num: %d\n", line_num);  return NUM;}
"true"			{printf("Got TRUE token in line_num: %d\n", line_num);return TRUE;}
"false"			{printf("Got FALSE token in line_num: %d\n", line_num);return FALSE;}
"print"			{printf("Got PRINT token in line_num: %d\n", line_num);return PRINT;}
"if"			{printf("Got IF token in line_num: %d\n", line_num);return IF;}
"else"			{printf("Got ELSE token in line_num: %d\n", line_num);return ELSE;}
"for"			{printf("Got FOR token in line_num: %d\n", line_num);return FOR;}
"while"			{printf("Got WHILE token in line_num: %d\n", line_num);return WHILE;}

"robot.getTemperature"	{printf("Got TEMPERATURE token in line_num: %d\n", line_num);return TEMPERATURE;}
"robot.getDistance"		{printf("Got DISTANCE token in line_num: %d\n", line_num);return DISTANCE;}
"robot.getRightLight"	{printf("Got R_LIGHT token in line_num: %d\n", line_num);return R_LIGHT;}
"robot.getLeftLight"	{printf("Got L_LIGHT token in line_num: %d\n", line_num);return L_LIGHT;}
"robot.turnOnLED"		{printf("Got ON_LED token in line_num: %d\n", line_num);return ON_LED;}
"robot.turnOffLED"		{printf("Got OFF_LED token in line_num: %d\n", line_num);return OFF_LED;}
"robot.forward"			{printf("Got FORWARD token in line_num: %d\n", line_num);return FORWARD;}
"robot.back"			{printf("Got BACK token in line_num: %d\n", line_num);return BACK;}
"robot.left"			{printf("Got LEFT token in line_num: %d\n", line_num);return LEFT;}
"robot.right"			{printf("Got RIGHT token in line_num: %d\n", line_num);return RIGHT;}
"robot.buzzer"			{printf("Got BUZZER token in line_num: %d\n", line_num);return BUZZER;}
"robot.rgb"				{printf("Got RGB token in line_num: %d\n", line_num);return RGB;}

"DO"		{printf("Got DO token in line_num: %d\n", line_num);return DO;}
"DO+"		{printf("Got DO_P token in line_num: %d\n", line_num);return DO_P;}
"RE"		{printf("Got RE token in line_num: %d\n", line_num);return RE;}
"RE+"		{printf("Got RE_P token in line_num: %d\n", line_num);return RE_P;}
"MI"		{printf("Got MI token in line_num: %d\n", line_num);return MI;}
"MI+"		{printf("Got MI_P token in line_num: %d\n", line_num);return MI_P;}
"FA"		{printf("Got FA token in line_num: %d\n", line_num);return FA;}
"FA+"		{printf("Got FA_P token in line_num: %d\n", line_num);return FA_P;}
"SOL"		{printf("Got SOL token in line_num: %d\n", line_num);return SOL;}
"SOL+"		{printf("Got SOL_P token in line_num: %d\n", line_num);return SOL_P;}
"LA"		{printf("Got LA token in line_num: %d\n", line_num);return LA;}
"LA+"		{printf("Got LA_P token in line_num: %d\n", line_num);return LA_P;}
"SI"		{printf("Got SI token in line_num: %d\n", line_num);return SI;}
"SI+"		{printf("Got SI_P token in line_num: %d\n", line_num);return SI_P;}

"+"		{printf("Got PLUS token in line_num: %d\n", line_num);return '+';}
"-"		{printf("Got MINUS token in line_num: %d\n", line_num);return '-';}
"*"		{printf("Got MULT token in line_num: %d\n", line_num);return '*';}
"/"		{printf("Got DIV token in line_num: %d\n", line_num);return '/';}
"%"		{printf("Got MOD token in line_num: %d\n", line_num);return '%';}
"("		{printf("Got OPPAREN token in line_num: %d\n", line_num);return '(';}
")"		{printf("Got CLPAREN token in line_num: %d\n", line_num);return ')';}
"{"		{printf("Got OPKEY token in line_num: %d\n", line_num);return '{';}
"}"		{printf("Got CLKEY token in line_num: %d\n", line_num);return '}';}
"<"		{printf("Got LTHAN token in line_num: %d\n", line_num);return '<';}
">"		{printf("Got GTHAN token in line_num: %d\n", line_num);return '>';}
"=="	{printf("Got SAME token in line_num: %d\n", line_num);return SAME;}
"="		{printf("Got EQUALS token in line_num: %d\n", line_num);return EQUALS;}
"!="	{printf("Got NO_EQUAL token in line_num: %d\n", line_num);return NO_EQUAL;}
";"		{printf("Got PCOMA token in line_num: %d\n", line_num);return PCOMA;}
","		{printf("Got COMA token in line_num: %d\n", line_num);return ',';}
":"		{printf("Got DPUNTOS token in line_num: %d\n", line_num);return ':';}
"&&"	{printf("Got AND token in line_num: %d\n", line_num); return AND ;}
"||"	{printf("Got OR token in line_num: %d\n", line_num); return OR ;}

{id}	{printf("Got ID token in line_num: %d\n", line_num); yylval.sval = strdup(yytext); return ID;} 
{string}	{printf("Got STRING token in line_num: %d\n", line_num); yylval.sval = strdup(yytext); return STRING;} 
{number}	{printf("Got NUMBER token in line_num: %d\n", line_num); yylval.fval = atof(yytext);return NUMBER;}

\n 		{line_num++;}
[ \t]	{}

.		{printf("ERROR: TOKEN %s NOT ALLOWED in line_num: %d\n", yytext,line_num);} 
%% 
