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
#include <sstream>
#include <fstream>
#include <list>
#include <stack>
#include "Variable.hpp"
#include <iostream>

using namespace std; 

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin; 
extern int line_num; 
extern char* yytext; 

//---------------DEFINICIÓN DE VARIABLES----------------------
string nameID; //Guarda el nombre de variable encontrada
string nameFunct; //Guarda el nombre de funcion encontrada 
ofstream halfCode; //Variable para escribir en un archivo de salida los cuádruplos

int contadorParametros; //Lleva el conteo de los parámetros en las llamadas de funciones 
int cantPar=0; //Guarda la cantidad de parámetros declarados en una función 

//Valores scope variables globales. Empieza en 1000 hasta 1999
int contGlobalNum = 1000; //Solo se podrán declarar 500 globales Num 
int contGlobalText = 1500; //Solo se podrán declarar 250 globales Text
int contGlobalBool = 1725; //Solo se podrán declarar 250 globales Bool

//Valores scope variables locales. Empieza en 2000 hasta 2999
int contLocalNum = 2000; //Solo se podrán declarar 500 locales Num	
int contLocalText = 2500; //Solo se podrán declarar 250 locales Text
int contLocalBool = 2725; //Solo se podrán declarar 250 locales Bool 

//Valores de scope para párametros locales a funciones. Empiezan en 200 hasta 230. 
//En cada función solo se permiten como máximo 10 parámetros de tipo num, 10 de tipo text y 10 de tipo bool. 
int contParNum=200;
int contParText=210;
int contParBool=220; 

//Banderas para inserción de variables
bool bnum=false;
bool btext=false;
bool bbool=false;
bool globalOrLocal;//Booleano para saber si estoy en global o local

//Bandera Global de ERROR
bool error=false;

//Bandera de Error por Máximo Párametros en Función
bool maxParError=false; 

//Bandera de Error por Máximo Globales y Bandera de Error por Máximo Locales.
bool maxGlobalError=false;
bool maxLocalError=false; 

//Bandera para revisar si la variable en cuestión existe.
bool bcurrentIDvar=false;

//Pilas auxiliares en la validación de Cuádruplos
stack<string>PilaO;	//Para operandos
stack<int>PTipos;	//Para los tipos de los operandos
stack<int>POper;	//Para los tipos de operadores

//Lista para ayudar a guardar las variables de cada función o globales
list<Variable> listVariables;

 //Contador de cuadruplos
int cont=0,top;

//Estructura para contener el nombre de función y su tabla de variables asociada. 
struct DirObject{
	string id;
	list<Variable> listaVar;
};

//Constante para guardar el nombre de la función global 
string globalFunction; 

//Constante para realizar los cuádruplos. Me indica que se está haciendo un print de una constante
bool cuadr_print=false; 

//Variable para guardar el id dentro de un Print. 
string name_id; 

//Variable para llevar la cuenta dentro de un Print para saber si es expresión o variable. 
int cont_print; 

//Directorio de procedimientos
vector<DirObject> DirProc; 

//Objeto auxiliar para crear las variable encontradas
Variable var; 

//String que guarda el nombre actual del directorio
string currentDir;

//Cubo Semántico
int SemanticCube[3][3][12] = { {{0,0,0,0,0,2,2,2,2,0,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
					     		{{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, {-1,-1,-1,1,-1,2,2,2,2,1,-1,-1}, {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
					      		{{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,2,2,2,2,2,2,2}} };

//-----------------FUNCIONES------------------------------------

//Función que asigna una Key para identificar el tipo de una variable. 
//Si el parámetro de llegada es true indica que la variable es global.
//Si el parámetro de llegada es false indica que la variable es local. 
void addCounterOfKeyType(bool scope)
{
	if(scope)
	{
		if(bnum && (contGlobalNum<1500)) var.setKey(contGlobalNum++);
		else if(btext && (contGlobalText<1725)) var.setKey(contGlobalText++);
		else if(bbool && (contGlobalNum<2000)) var.setKey(contGlobalBool++);
		else {cout<<"¡Muchas variables declaradas!"<<endl; error=true; maxGlobalError=true;}
	}else{
		if(bnum && (contLocalNum<2500)) var.setKey(contLocalNum++);
		else if(btext && (contLocalText<2725)) var.setKey(contLocalText++);
		else if(bbool && (contLocalNum<3000)) var.setKey(contLocalBool++);
		else {cout<<"¡Muchas variables declaradas!"<<endl; error=true;maxLocalError=true;}
	}
}

//Función que asigna un Key para identificar el tipo de un parámetro dentro de una función. 
void addCounterOfParameter()
{
	if (bnum && (contParNum<210))var.setKey(contParNum++);
	else if(btext && (contParText<220))var.setKey(contParText++);
	else if (bbool && (contParBool<230))var.setKey(contParBool++);
	else {cout << "¡Muchos parámetros declarados!" << endl; error=true; maxParError=true;}
}

//Función que valida que no exista ya el nombre de la función dentro del Directorio Procedimientos.
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

//Función que valida que no exista el nombre de la variable dentro de la misma función. 
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

//Función que valida que exista la función dentro del Directorio Procedimientos. 
bool validateExistFunc(string name_fun)
{
	bool existFunc = false; 
	{
		for(int c=0; c<DirProc.size(); c++)
		{
			if (name_fun == DirProc[c].id)
				existFunc = true; 
		}
	}
	return existFunc; 
}

//Función que valida en la llamada a la función, los parámetros sean variables globales para poder utilizarse. 
bool validateExistVar(string name_var)
{	
	for(int c=0; c<DirProc.size(); c++)
		{
			if (globalFunction == DirProc[c].id)
			for (list<Variable>:: iterator it= DirProc[c].listaVar.begin();it!=DirProc[c].listaVar.end();it++)
			{
				if (name_var == (*it).getId())
				{	
					return true;
				}	
			}
		}
		error=true;
		return false;
}

//Función para retornar el tipo de la variable encontrada.
int getTypeOfVar(string nameDir,string name_var)
{	
	int type,pretype; 
	for(int c=0; c<DirProc.size(); c++)
		{
			if (nameDir == DirProc[c].id)
			for (list<Variable>:: iterator it= DirProc[c].listaVar.begin();it!=DirProc[c].listaVar.end();it++)
			{
				if (name_var == (*it).getId())
				{	
					pretype=(*it).getKey();
					if((pretype>=1000 && pretype<1500) || (pretype>=2000 && pretype<2500) || (pretype>=200 && pretype<210))
						return 0;//Retorna tipo Numérico
					if((pretype>=1500 && pretype<1725) || (pretype>=2500 && pretype<2725) || (pretype>=210 && pretype<220))
						return 1;//Retorna tipo Text
					else
						return 2;//Retorna tipo Booleano
				}	
			}
		}
}

//Función para retornar el tipo del parámetro de una función
int getTypeOfPar(string name_fun, int numPar)
{	
	int typePar=0;
	int cont=0;  
	for (int c=0; c<DirProc.size();c++)
	{
		if (name_fun==DirProc[c].id)
		{
			for (list<Variable>:: iterator it= DirProc[c].listaVar.begin();it!=DirProc[c].listaVar.end();it++)
			{
				if (cont == numPar)
				{
					typePar=(*it).getKey(); 
					if(typePar>=200 && typePar<210)
						return 0;//Retorna tipo Numérico
					if(typePar>=210 && typePar<220)
						return 1;//Retorna tipo Text
					else
						return 2;//Retorna tipo Booleano
				}
				cont++;
			}
		}
	}	
}

//Función de Semántica para decir si una variable está declarada y poderla utilizar
bool isVarAlreadyDeclared(string nameDir,string name_var)
{	

	for (list<Variable>:: iterator it= DirProc[0].listaVar.begin();it!=DirProc[0].listaVar.end();it++)
	{
		if (name_var == (*it).getId())
		{	
			return true;
		}	
	}

	for(int c=0; c<DirProc.size(); c++)
		{
			if (nameDir == DirProc[c].id)
			for (list<Variable>:: iterator it= DirProc[c].listaVar.begin();it!=DirProc[c].listaVar.end();it++)
			{
				if (name_var == (*it).getId())
				{	
					return true;
				}	
			}
		}
		cout<<"ERROR FATAL - VARIABLE NO DECLARADA EN ESTE CONTEXTO"<<endl;
		error=true;
		return false;
}

//Función para obtener el número de parámetros declarados en una función
int numParFunc(string name_fun)
{
	int contPar=0; 
	for(int c=0; c<DirProc.size(); c++)
		{
			if (name_fun == DirProc[c].id)
			for (list<Variable>:: iterator it= DirProc[c].listaVar.begin();it!=DirProc[c].listaVar.end();it++)
			{
				if ((199 < (*it).getKey()) && ((*it).getKey() < 300)) 
				{	
					contPar++;
				}	
			}
		}
	return contPar; 
} 

//Función para validar que se pueda realizar la operación en el cubo semántico
bool validateOnCube(int oper)
{
    int operD = PTipos.top();
    			PTipos.pop();
    int operI = PTipos.top();
    			PTipos.pop();
    int resp;

    resp=SemanticCube[operI][operD][oper];

    if(resp!=-1) 
    	{PTipos.push(resp); return true;}
    else
    	return false; 
}

//Función para retornar el string del operador correspondiente.
string nameOperador(int ope)
{
	switch (ope)
	{
		case 0: return "*";
		case 1: return "/";
		case 2: return "%";
		case 3:	return "+";
		case 4: return "-";
		case 5: return "<";
		case 6: return ">";
		case 7: return "==";
		case 8: return "!="; 
		case 9: return "=";
		case 10: return "&&";
		case 11: return "||"; 
		default : return " "; 
	}
}

//Función para convertir un entero en string. 
string convertInt(int number)
{
   stringstream ss;//Create a stringstream
   ss << number;//Add number to the stream
   return ss.str();//Return a string with the contents of the stream
}

//--------------------PARSER--------------------
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
	%token <sval>VAR
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
	%token <sval>NUMBER

	%left '+' '-'
	%left '*' '/' '%'

%error-verbose

%%
programa: 
		START ID {globalOrLocal = true;currentDir = $2; globalFunction=$2; halfCode.open($2);} ':' vars  
							{

							DirObject myDirObject;
							myDirObject.id = $2;
							myDirObject.listaVar = listVariables;
							DirProc.push_back(myDirObject);
							listVariables.clear();} functions {currentDir = $2; halfCode<<"mainStart"<<endl;}bloque
				 {
					halfCode<<"mainEnd";
				}
		; 

functions: 
		FUNCTION ID {globalOrLocal = false; currentDir = $2;
						halfCode<<"void "<<$2<<" startParam ";} '(' par_func ')' {halfCode<<" endParam "<<endl;
																			} vars_func {
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
																			}	else 
																			{
																				listVariables.clear(); 
																				cout << "ERROR FATAL: NO PUEDEN EXISTIR DOS FUNCIONES CON EL MISMO NOMBRE" << endl; 
																			}
																				contLocalNum = 2000;
																				contLocalText = 2500;
																				contLocalBool = 2725;
																				contParNum=200;
																				contParText=210;
																				contParBool=220;
												} bloque {halfCode<<" endFunc"<<endl;} functions
		|
		;

vars_func: 
		'{' vars '}'
		;

par_func: 
	tipo ID { //Llamada a funcion para verificar si el nombre de variable ya existe 
			nameID = $2; 
			bool insert=true; 
			insert = validateInsertVar(nameID);
			if (insert == true )
			{
				var.setId($2);
		 		addCounterOfParameter();
		 		if (maxParError==false)
		 			listVariables.push_back(var),halfCode<<$2;
		 	}
		 	else 
		 	{	cout << "ERROR FATAL: NO ES POSIBLE DECLARAR PARÁMETROS CON EL MISMO NOMBRE DENTRO DE UNA FUNCION" << endl;
		 		error=true; return -1;
		 	}			 	 
		 	} PCOMA{halfCode<<" , ";} par_func
		|
		; 

vars: 
		VAR tipo def_names PCOMA{halfCode<<" ; "<<endl;} vars 
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
			 		addCounterOfKeyType(globalOrLocal);
			 		if (maxGlobalError==false && maxLocalError==false)
			 			listVariables.push_back(var),halfCode<<$1;
			 	}
			 	else 
			 		{	cout << "ERROR FATAL: NO ES POSIBLE DECLARAR VARIABLES CON EL MISMO NOMBRE DENTRO DE UNA FUNCION" << endl;
			 			error=true; return -1;
			 		}
		 	}
			

		| ID {
				nameID = $1; 
				bool insert=true; 
				insert = validateInsertVar(nameID);
				if (insert == true)
				{
					var.setId($1);
			   		addCounterOfKeyType(globalOrLocal);
			   		if (maxGlobalError==false && maxLocalError==false)
			   			listVariables.push_back(var),halfCode<<$1;
				}
				else 
					{	cout << "ERROR FATAL: NO ES POSIBLE DECLARAR VARIABLES CON EL MISMO NOMBRE DENTRO DE UNA FUNCION" << endl;
			 			error=true; return -1; 
			 		}		
			} ','{halfCode<<" , ";} def_names
		;

tipo: 
		TEXT  {btext = true; bnum = false; bbool = false; halfCode<<"string ";}
		|NUM  {bnum = true; btext = false; bbool = false; halfCode<<"float ";}
		|BOOL {bbool = true; bnum = false; btext = false; halfCode<<"boolean ";}
		; 

bloque: 
		'{'{halfCode<<"startBlock ";} estatuto '}'{halfCode<<"endBlock ";}
		; 

estatuto:
		ciclos estatuto
		|asignacion estatuto
		|condicion estatuto
		|escritura estatuto
		|robot estatuto
		|callfunction estatuto
		|
		; 

asignacion:
		ID {bcurrentIDvar= isVarAlreadyDeclared(currentDir,$1); if(bcurrentIDvar) PilaO.push($1), PTipos.push(getTypeOfVar(currentDir,$1));
		}EQUALS{ POper.push(9); } pos_asignacion PCOMA{halfCode<<" ;"<<endl;}
		; 

pos_asignacion: 
		expresion{
						if (!POper.empty())
				 		{
				 			top=POper.top(); 
				 			if (top == 9)
				 			{
				 				string OperI=PilaO.top(); PilaO.pop();
				 				string OperD=PilaO.top(); PilaO.pop();
				 				int operador = POper.top(); POper.pop();
				 				int tipo = PTipos.top(); 
				 				halfCode << nameOperador(operador) << " " << OperI << " " << OperD << " " << endl; 
				 			}
				 		}
		}
		|STRING {PTipos.push(1);
					if(validateOnCube(9))
					{
					string OperI=PilaO.top(); PilaO.pop();
					halfCode << "=" << " " << $1 << " " << OperI;
					 }}
		;


expresion: 
		expb expr_1 
		;

expr_1: 
		'>' {POper.push(6);}expb {if(!validateOnCube(6)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}      
		|'<' {POper.push(5);}expb {if(!validateOnCube(5)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}     
		|NO_EQUAL {POper.push(8);}expb{if(!validateOnCube(8)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}} 
		|SAME {POper.push(7);}expb {if(!validateOnCube(7)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}    
		|
		;

expb:	
		exp e_bool{
        if(!POper.empty()){
            top=POper.top();
            if(top == 6 || top == 5 || top == 8 || top == 7){
                    string OperD=PilaO.top();PilaO.pop();
                    string OperI=PilaO.top();PilaO.pop();
                    int operador=POper.top();POper.pop();
                    int tipo = PTipos.top();
                    halfCode  << nameOperador(operador) << " "<<OperI<< " " << OperD << " " << "T"<<cont++<<" " << endl;
                    PilaO.push("T"+convertInt(cont-1));
               }
            }
        }
		; 

e_bool:
		AND {POper.push(10);}expb{if(!validateOnCube(10)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true;return  -1;}}
		|OR {POper.push(11);}expb{if(!validateOnCube(11)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
		|
		;

exp:
		termino e_sum{
        if(!POper.empty()){
            top=POper.top();
            if(top == 10 || top == 11){
                    string OperD=PilaO.top();PilaO.pop();
                    string OperI=PilaO.top();PilaO.pop();
                    int operador=POper.top();POper.pop();
                    int tipo = PTipos.top();
                    halfCode << nameOperador(operador) << " " << OperI << " " << OperD << " " << "T"<<cont++<<" " << endl; 
                    PilaO.push("T"+convertInt(cont-1));
                }
            }
        }
		;

e_sum:
		'+' {POper.push(3);}exp	 {if(!validateOnCube(3)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
		|'-'{POper.push(4);} exp {if(!validateOnCube(4)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
		|
		;

termino: 
		factor 
		{if(!POper.empty())
		 	{
            	top=POper.top();
            	if(top == 0 || top == 1 || top == 2)
            	{
                    string OperD=PilaO.top();PilaO.pop();
                    string OperI=PilaO.top();PilaO.pop();
                    int operador=POper.top();POper.pop();
                    int tipo = PTipos.top();
                    halfCode << nameOperador(operador) << " " << OperI << " " << OperD << " " << "T"<<cont++<<" " << endl; 
                    PilaO.push("T"+convertInt(cont-1));
                }
            }
         } e_mult{
        if(!POper.empty()){
            top=POper.top();
            if(top == 4 || top == 3){
                    string OperD=PilaO.top();PilaO.pop();
                    string OperI=PilaO.top();PilaO.pop();
                    int operador=POper.top();POper.pop();
                    int tipo = PTipos.top();
                    halfCode << nameOperador(operador) << " " << OperI << " " << OperD << " " << "T"<<cont++<<" " << endl;
                    PilaO.push("T"+convertInt(cont-1));
                }
            }
        }
		;


factor:
		 '(' { POper.push(-1); halfCode<<" ( ";} expresion ')' {while(POper.top() != -1)
		 										{
		 											string OperD=PilaO.top();PilaO.pop();
								                    string OperI=PilaO.top();PilaO.pop();
								                    int operador=POper.top();POper.pop();
								                    int tipo = PTipos.top();
								                    halfCode << nameOperador(operador) << " " << OperI << " " << OperD << " " << "T"<<cont++<<" " << endl; }
								                    POper.pop();
								                    halfCode<<" ) ";
		 									}
		 | fact var_cte
		 | var_cte 
		 ;

fact:
		'+'{POper.push(3); }
		|'-'{POper.push(4);}
		;


var_cte: 
		ID{		bcurrentIDvar=isVarAlreadyDeclared(currentDir,$1); 
				if(bcurrentIDvar) PilaO.push($1), PTipos.push(getTypeOfVar(currentDir,$1));
				cont_print++; 
				name_id=$1; 		
		  }	
		|NUMBER {PilaO.push($1); PTipos.push(0); cont_print++;}
		|TRUE   {PilaO.push("true"); PTipos.push(2);}
		|FALSE  {PilaO.push("false"); PTipos.push(2);}
		;

e_mult:
		'*' {POper.push(0);}termino {if(!validateOnCube(0)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
		|'/' {POper.push(1);}termino {if(!validateOnCube(1)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
		|'%' {POper.push(2);}termino {if(!validateOnCube(2)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
		|
		;

condicion: 
		IF{halfCode<<"if ( ";} '(' expresion ')'{halfCode<<" ) endIfParam ";} bloque con_else
		;

con_else: 
		ELSE{halfCode<<"else "<<endl;} bloque
		|
		; 

escritura: 
		PRINT '('{halfCode<<"print ( "; cuadr_print=true; cont_print=0;} esc_2 ')' PCOMA { if ((cuadr_print==true) && (cont_print==1))
																								halfCode <<  name_id + " ) endPrint" << endl;
																						   else 
																						   {
		 										
																								halfCode<<" ) endPrint" << endl;
																						   }
																							cuadr_print=false;}
		; 

esc_2: 
		expresion
		|STRING {halfCode<<$1;}
		;

robot: 
		TEMPERATURE{halfCode<<" roboTemp "<<endl;} PCOMA
		|DISTANCE {halfCode<<" roboDistance "<<endl;}PCOMA
		|R_LIGHT {halfCode<<" roboRightLight "<<endl;}PCOMA
		|L_LIGHT {halfCode<<" roboLeftLight "<<endl;}PCOMA
		|ON_LED '(' NUMBER ')' PCOMA {halfCode<<" roboOnLED "<<$3<<" "<<endl;}
		|OFF_LED '(' NUMBER ')' PCOMA{halfCode<<" roboOffLED "<<$3<<" "<<endl;}
		|FORWARD PCOMA{halfCode<<" roboForward "<<endl;}
		|BACK PCOMA{halfCode<<" roboBack "<<endl;}
		|LEFT PCOMA{halfCode<<" roboLeft "<<endl;}
		|RIGHT PCOMA{halfCode<<" roboRight "<<endl;}
		|BUZZER '(' NUMBER {halfCode<<" roboBuzzer "<<$3;} ',' note ')' PCOMA
		|RGB '(' NUMBER ',' NUMBER ',' NUMBER ')' PCOMA{halfCode<<" roboRGB "<<$3<<" "<<$5<<" "<<$7<<endl;}
		; 

note: 
		DO{halfCode<<" DO "<<endl;}
		|DO_P{halfCode<<" DOP "<<endl;}
		|RE{halfCode<<" RE "<<endl;}
		|RE_P{halfCode<<" REP "<<endl;}
		|MI{halfCode<<" MI "<<endl;}
		|MI_P{halfCode<<" MIP "<<endl;}
		|FA{halfCode<<" FA "<<endl;}
		|FA_P{halfCode<<" FAP "<<endl;}
		|SOL{halfCode<<" SOL "<<endl;}
		|SOL_P{halfCode<<" SOLP "<<endl;}
		|LA{halfCode<<" LA "<<endl;}
		|LA_P{halfCode<<" LAP "<<endl;}
		|SI{halfCode<<" SI "<<endl;}
		|SI_P{halfCode<<" SI "<<endl;}
		; 

callfunction: 
		ID {nameFunct=$1;
				if (validateExistFunc(nameFunct) != true) 
				 {
				 	cout << "ERROR FATAL: NO ES POSIBLE UTILIZAR FUNCIONES QUE NO HAN SIDO DECLARADAS" << endl;
				 	error=true; return -1; 	
				 }
				 else 
				 {
				 	halfCode <<$1; 
				 	contadorParametros=0; 
					cantPar=numParFunc(nameFunct);
				 }
			}'(' {halfCode<<" ( ";}par_callfunction ')' PCOMA {halfCode<<" ) ;"<<endl;}

par_callfunction: 
		ID{nameID=$1; 
		   if (contadorParametros < cantPar)
		   {
			   	if (validateExistVar(nameID))
			   	{
			   		halfCode<<$1;
				   	int tipoParametro=0;  
				   	tipoParametro=getTypeOfVar(globalFunction, nameID); 
				   	int tipoParametroFuncion=0; 
				   	tipoParametroFuncion=getTypeOfPar(nameFunct,contadorParametros);
				   	if (tipoParametro == tipoParametroFuncion)
				   		contadorParametros++; 
				   	else 
				   	{
				   		cout << "ERROR FATAL: EL PARAMETRO " << contadorParametros+1 << " NO ES DEL MISMO TIPO QUE EN LA DEFINICIÓN DE LA FUNCIÓN" << endl; 
				   		error=true; return -1; 
				   	} 

				}
			   	else 
			   	{
		   			cout <<"ERROR FATAL: VARIABLE COMO PARÁMETRO EN LA FUNCIÓN NO ES GLOBAL"<<endl;
		   		 	error=true; return -1; 
		   		}
			}
			else 
			{
				cout << "ERROR FATAL: EL NÚMERO DE PARÁMETROS EN LA LLAMADA SOBREPASAN LA DECLARACIÓN DE LA FUNCIÓN" << endl; 
				error=true; return -1; 	
			}
		   
		} PCOMA {if (contadorParametros == (cantPar)) halfCode<<" "; else halfCode<< " , ";} par_callfunction
		| {if (contadorParametros < (cantPar))
			{
				cout << "ERROR FATAL: EL NÚMERO DE PARÁMETROS EN LA LLAMADA SON MENOS QUE EN LA DECLARACIÓN DE LA FUNCIÓN" << endl;
				error=true; return -1; 
			} 
		} 
		;

ciclos: 
		WHILE '(' {halfCode<<" while ( ";}expresion ')'{halfCode<<" ) endWhileParam ";} bloque
		|FOR '('{halfCode<<" for ( ";} for1p PCOMA for2p PCOMA for3p ')' bloque
		; 

for1p:
		ID EQUALS NUMBER{halfCode<<$1<<"="<<$3<<"; ";}
		; 

for2p: 
		expresion{halfCode<<"; ";}
		; 

for3p: 
		ID EQUALS ID {halfCode<<$1<<"="<<$3<<" ";} op_for NUMBER {halfCode<<$6<<" )"<<endl;}
		; 

op_for: 
		'+' {halfCode<<'+';}
		|'-'{halfCode<<'-';}
		|'*'{halfCode<<'*';}
		|'/'{halfCode<<'/';}
		;

%%

void yyerror (const char *s)
{
	printf ("Error en el parser: %s", s); 
}

int main()
{
	string nameArch;
	cin>>nameArch;	
	FILE *myfile = fopen(nameArch.c_str(), "r");
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
	if (error==0)
		cout << globalFunction << endl;
	else 
		cout << "error"<< endl; 
	halfCode.close();
}
