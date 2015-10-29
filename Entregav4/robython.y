// --------------------REFERENCIAS ---------------------------
//http://www.cplusplus.com/reference/list/list/
//http://www.cplusplus.com/reference/vector/vector/
// ----------------------------------------------------------
%{
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <queue>
#include <vector>
#include <list>
#include "Variable.hpp"
#include <iostream>

using namespace std; 

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin; 
extern int line_num; 
extern char* yytext; 

//Vector that contains elements of the class Variable
vector<Variable> tabVars; 

string nameID; //Guarda el nombre de variable encontrado
string nameFunct; //Guarda el nombre de funcion encontrada 

//Valores scope variables globales. Empieza en 1000 hasta 1999
int contGlobalNum = 1000; //Solo se podrán declarar 500 globales Num 
int contGlobalText = 1500; //Solo se podrán declarar 250 globales Text
int contGlobalBool = 1725; //Solo se podrán declarar 250 globales Bool

//Valores scope variables locales. Empieza en 2000 hasta 2999
int contLocalNum = 2000;	
int contLocalText = 2500;
int contLocalBool = 2725; 

//Banderas para inserción de variables
bool bnum=false;
bool btext=false;
bool bbool=false;
bool globalOrLocal;//Booleano para saber si estoy en global o local
//Bandera de ERROR
bool error=false;

//Lista para ayudar a guardar las variables de cada función o globales
list<Variable> listVariables;
 
//Estructura para contener el nombre de función y su tabla de variables asociada. 
struct DirObject{
	string id;
	list<Variable> listaVar;
};

//Directorio de procedimientos
vector<DirObject> DirProc; 

Variable var; //Objeto auxiliar para crear las variable encontradas

int SemanticCube[3][3][10] = { {{0,0,0,0,0,2,2,2,2,0},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
					     				 {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, {-1,-1,-1,1,-1,2,2,2,2,1}, {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
					      				 {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,2,2,2,2,2}} };

//Aumenta el contador dependiendo del tipo de variables encontradas
void addCounterOfKeyType(bool scope)
{
	if(scope)
	{
		if(bnum && (contGlobalNum<1500)) var.setKey(contGlobalNum++);
		else if(btext && (contGlobalText<1725)) var.setKey(contGlobalText++);
		else if(bbool && (contGlobalNum<2000)) var.setKey(contGlobalBool++);
		else {cout<<"¡Muchas variables declaradas!"<<endl; error=true;}
	}else{
		if(bnum && (contLocalNum<2500)) var.setKey(contLocalNum++);
		else if(btext && (contLocalText<2725)) var.setKey(contLocalText++);
		else if(bbool && (contLocalNum<3000)) var.setKey(contLocalBool++);
		else {cout<<"¡Muchas variables declaradas!"<<endl; error=true;}
	}
}

//Funcion que valida que no exista ya el nombre de la funcion dentro del Directorio Procedimientos.
bool validateInsertFunc(string name_fun)
{
	bool insertarFunc = true; 
	{
		for(int c=0; c<DirProc.size(); c++)
		{
			if (name_fun == DirProc[c].id)
				insertarFunc = false; 
		}
	}
	return insertarFunc; 
}


//Funcion que valida que no exista el nombre de la variable dentro de la misma funcion. 
bool validateInsertVar(string name_var)
{	
	bool poderInsertar = true; 
	for (list<Variable>:: iterator it= listVariables.begin();it!=listVariables.end();it++)
	{
		if (name_var == (*it).getId())	
			poderInsertar = false; 
	}
	return poderInsertar; 
}

void yyerror (const char *s);
%}

%union {
	float fval;
	bool bval;
	char *sval;
}
	%start programa
	%token START
	%token FUNCTION
	%token VAR
	%token BOOL
	%token TEXT
	%token NUM
	%token TRUE
	%token FALSE
	%token PRINT
	%token IF
	%token ELSE
	%token FOR
	%token WHILE

	%token TEMPERATURE
	%token DISTANCE
	%token R_LIGHT
	%token L_LIGHT
	%token ON_LED
	%token OFF_LED
	%token FORWARD
	%token BACK
	%token LEFT
	%token RIGHT
	%token BUZZER
	%token RGB

	%token DO
	%token DO_P
	%token RE
	%token RE_P
	%token MI
	%token MI_P
	%token FA
	%token FA_P
	%token SOL
	%token SOL_P
	%token LA
	%token LA_P
	%token SI
	%token SI_P

	%token SAME
	%token EQUALS
	%token NO_EQUAL
	%token PCOMA
	%token AND
	%token OR

	%token <sval>ID
	%token <sval>STRING
	%token <fval>NUMBER

	%left '+' '-'
	%left '*' '/' '%'

%error-verbose

%%
programa: 
		START ID {globalOrLocal = true;} ':' vars  
											{DirObject myDirObject;
											myDirObject.id = $2;
											myDirObject.listaVar = listVariables;
											DirProc.push_back(myDirObject);
											listVariables.clear();} functions bloque
				 {
					//Imprimir resultados finales
					cout<<"PROGRAM WORKS PROPERLY"<<endl;
					for(int i=0; i<DirProc.size(); i++)
					{
						cout<<DirProc[i].id<<endl;
						while(!DirProc[i].listaVar.empty())
						{
							cout<<"\t";
							DirProc[i].listaVar.front().muestra();
							DirProc[i].listaVar.pop_front();
							cout<<endl;
						}
					}
				}
		; 

functions: 
		FUNCTION ID {globalOrLocal = false;} '(' par_func ')' vars_func {
																			nameFunct=$2;
																			bool insertF = true;
																			insertF=validateInsertFunc(nameFunct);
																			if (insertF==true)
																			{
																				DirObject myDirObject;
																				myDirObject.id = $2;
																				myDirObject.listaVar = listVariables;
																				DirProc.push_back(myDirObject);
																				listVariables.clear();
																				contLocalNum = 2000;
																				contLocalText = 2500;
																				contLocalBool = 2725;
																			}
																			else 
																			{
																				listVariables.clear(); 
																				cout << "ERROR FATAL: NO PUEDEN EXISTIR DOS FUNCIONES CON EL MISMO NOMBRE" << endl; 
																			}
																		} bloque functions
		|
		;

vars_func: 
		'{' vars '}'
		;

par_func: 
		tipo ID{cout<<"Nom_Var:" <<$2<<endl; } PCOMA par_func
		|
		; 

vars: 
		VAR tipo def_names PCOMA vars 
		|
		; 
 
def_names: 
		ID { 	//Llamada a funcion para verificar si el nombre de variable ya existe 
				nameID = $1; 
				bool insert=true; 
				insert = validateInsertVar(nameID);
				if (insert == true )
				{
					var.setId($1);
			 		addCounterOfKeyType(true);
			 		listVariables.push_back(var); cout<<"push"<<endl;
			 	}
			 	else 
			 		cout << "ERROR FATAL: NO ES POSIBLE DECLARAR VARIABLES CON EL MISMO NOMBRE DENTRO DE UNA FUNCION" << endl; 
		 	}
			

		| ID {
				nameID = $1; 
				bool insert=true; 
				insert = validateInsertVar(nameID);
				if (insert == true)
				{
					var.setId($1);
			   		addCounterOfKeyType(true);
			   		listVariables.push_back(var); cout<<"push"<<endl;
				}
				else 
					cout << "ERROR FATAL: NO ES POSIBLE DECLARAR VARIABLES CON EL MISMO NOMBRE DENTRO DE UNA FUNCION" << endl; 
			} ',' def_names
		;

tipo: 
		TEXT  {btext = true; bnum = false; bbool = false;}
		|NUM  {bnum = true; btext = false; bbool = false;}
		|BOOL {bbool = true; bnum = false; btext = false;}
		; 

bloque: 
		'{' estatuto '}'
		; 

estatuto:
		ciclos estatuto
		|asignacion estatuto
		|condicion estatuto
		|escritura estatuto
		|robot estatuto
		|
		; 

asignacion:
		ID EQUALS expresion PCOMA
		; 

expresion: 
		expb expr_1 
		;

expr_1: 
		'>' expb       
		|'<' expb      
		|NO_EQUAL expb 
		|SAME expb     
		|
		;

expb:	
		exp e_bool
		; 

e_bool:
		AND expb
		|OR expb
		|
		;

exp:
		termino e_sum
		;

e_sum:
		'+' exp	 
		|'-' exp 
		|
		;

termino: 
		factor e_mult
		;


factor:
		 '(' expresion ')'
		 | fact var_cte
		 | var_cte
		 ;

fact:
		'+'
		|'-'
		;


var_cte: 
		ID
		|NUMBER 
		|TRUE
		|FALSE
		;

e_mult:
		'*' termino 
		|'/' termino 
		|'%' termino 
		|
		;

condicion: 
		IF '(' expresion ')' bloque con_else
		;

con_else: 
		ELSE bloque
		|
		; 

escritura: 
		PRINT '(' esc_2 ')' PCOMA
		; 

esc_2: 
		esc_3
		|esc_3 ',' esc_2
		;

esc_3:
		expresion
		|STRING
		; 

robot: 
		TEMPERATURE PCOMA
		|DISTANCE PCOMA
		|R_LIGHT PCOMA
		|L_LIGHT PCOMA
		|ON_LED '(' NUMBER ')' PCOMA
		|OFF_LED '(' NUMBER ')' PCOMA
		|FORWARD PCOMA
		|BACK PCOMA
		|LEFT PCOMA
		|RIGHT PCOMA
		|BUZZER '(' NUMBER ',' note ')' PCOMA
		|RGB '(' NUMBER ',' NUMBER ',' NUMBER ')' PCOMA
		; 

note: 
		DO
		|DO_P
		|RE
		|RE_P
		|MI
		|MI_P
		|FA
		|FA_P
		|SOL
		|SOL_P
		|LA
		|LA_P
		|SI
		|SI_P
		; 

ciclos: 
		WHILE '(' expresion ')' bloque
		|FOR '(' for1p PCOMA for2p PCOMA for3p ')' bloque
		; 

for1p:
		ID EQUALS NUMBER
		; 

for2p: 
		expresion
		; 

for3p: 
		ID EQUALS ID op_for NUMBER
		; 

op_for: 
		'+'
		|'-'
		|'*'
		|'/'
		;

%%

void yyerror (const char *s)
{
	printf ("Error en el parser: %s", s); 
}

int main()
{
	FILE *myfile = fopen("prueba", "r");
	if (!myfile)
	{
		printf("No se puede abrir el archivo\n");
		return -1;
	}
	yyin = myfile;
	do 
	{
		yyparse();		
	} while (!feof(yyin)); 
}
