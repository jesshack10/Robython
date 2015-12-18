/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 5 "robython.y" /* yacc.c:339  */

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

#line 408 "robython.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "robython.tab.h".  */
#ifndef YY_YY_ROBYTHON_TAB_H_INCLUDED
# define YY_YY_ROBYTHON_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    START = 258,
    FUNCTION = 259,
    VAR = 260,
    BOOL = 261,
    TEXT = 262,
    NUM = 263,
    TRUE = 264,
    FALSE = 265,
    PRINT = 266,
    IF = 267,
    ELSE = 268,
    FOR = 269,
    WHILE = 270,
    TEMPERATURE = 271,
    DISTANCE = 272,
    R_LIGHT = 273,
    L_LIGHT = 274,
    ON_LED = 275,
    OFF_LED = 276,
    FORWARD = 277,
    BACK = 278,
    LEFT = 279,
    RIGHT = 280,
    BUZZER = 281,
    RGB = 282,
    DO = 283,
    DO_P = 284,
    RE = 285,
    RE_P = 286,
    MI = 287,
    MI_P = 288,
    FA = 289,
    FA_P = 290,
    SOL = 291,
    SOL_P = 292,
    LA = 293,
    LA_P = 294,
    SI = 295,
    SI_P = 296,
    SAME = 297,
    EQUALS = 298,
    NO_EQUAL = 299,
    PCOMA = 300,
    AND = 301,
    OR = 302,
    ID = 303,
    STRING = 304,
    NUMBER = 305
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 347 "robython.y" /* yacc.c:355  */

	float fval;
	bool bval;
	char *sval;

#line 505 "robython.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ROBYTHON_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 520 "robython.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   188

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  81
/* YYNRULES -- Number of rules.  */
#define YYNRULES  142
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  248

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   305

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    55,     2,     2,
      57,    58,    53,    51,    61,    52,     2,    54,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    56,     2,
      63,     2,    62,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    59,     2,    60,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   413,   413,   414,   420,   413,   427,   428,   429,   451,
     427,   452,   456,   460,   475,   460,   476,   480,   480,   481,
     485,   503,   518,   503,   522,   523,   524,   528,   528,   532,
     533,   534,   535,   536,   537,   538,   542,   543,   542,   547,
     561,   571,   575,   575,   576,   576,   577,   577,   578,   578,
     579,   583,   599,   599,   600,   600,   601,   605,   621,   621,
     622,   622,   623,   628,   627,   658,   658,   668,   669,   673,
     674,   679,   684,   685,   686,   690,   690,   691,   691,   692,
     692,   693,   697,   697,   697,   701,   701,   702,   706,   706,
     717,   718,   722,   722,   723,   723,   724,   724,   725,   725,
     726,   727,   728,   729,   730,   731,   732,   732,   733,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,   750,   754,   766,   754,   769,   800,   769,   801,
     810,   810,   810,   811,   811,   815,   819,   823,   823,   827,
     828,   829,   830
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "START", "FUNCTION", "VAR", "BOOL",
  "TEXT", "NUM", "TRUE", "FALSE", "PRINT", "IF", "ELSE", "FOR", "WHILE",
  "TEMPERATURE", "DISTANCE", "R_LIGHT", "L_LIGHT", "ON_LED", "OFF_LED",
  "FORWARD", "BACK", "LEFT", "RIGHT", "BUZZER", "RGB", "DO", "DO_P", "RE",
  "RE_P", "MI", "MI_P", "FA", "FA_P", "SOL", "SOL_P", "LA", "LA_P", "SI",
  "SI_P", "SAME", "EQUALS", "NO_EQUAL", "PCOMA", "AND", "OR", "ID",
  "STRING", "NUMBER", "'+'", "'-'", "'*'", "'/'", "'%'", "':'", "'('",
  "')'", "'{'", "'}'", "','", "'>'", "'<'", "$accept", "programa", "$@1",
  "$@2", "$@3", "functions", "$@4", "$@5", "$@6", "$@7", "vars_func",
  "par_func", "$@8", "$@9", "vars", "$@10", "def_names", "$@11", "$@12",
  "tipo", "bloque", "$@13", "estatuto", "asignacion", "$@14", "$@15",
  "pos_asignacion", "expresion", "expr_1", "$@16", "$@17", "$@18", "$@19",
  "expb", "e_bool", "$@20", "$@21", "exp", "e_sum", "$@22", "$@23",
  "termino", "$@24", "factor", "$@25", "fact", "var_cte", "e_mult", "$@26",
  "$@27", "$@28", "condicion", "$@29", "$@30", "con_else", "$@31",
  "escritura", "$@32", "esc_2", "robot", "$@33", "$@34", "$@35", "$@36",
  "$@37", "note", "callfunction", "$@38", "$@39", "par_callfunction",
  "$@40", "$@41", "ciclos", "$@42", "$@43", "$@44", "for1p", "for2p",
  "for3p", "$@45", "op_for", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,    43,    45,    42,    47,    37,    58,    40,    41,   123,
     125,    44,    62,    60
};
# endif

#define YYPACT_NINF -189

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-189)))

#define YYTABLE_NINF -124

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      55,   -36,    60,  -189,  -189,     6,    58,    38,  -189,  -189,
    -189,  -189,    16,    67,    11,    28,    32,  -189,    37,  -189,
    -189,    40,  -189,    58,    43,  -189,  -189,    16,  -189,    38,
      70,  -189,    45,    53,    48,  -189,    49,    50,  -189,  -189,
    -189,  -189,    51,    52,    65,    66,    68,    69,    59,    76,
      77,    57,    70,    70,    70,    70,    70,    70,  -189,  -189,
    -189,    78,  -189,  -189,    92,    93,    94,    95,    62,    86,
    -189,  -189,  -189,  -189,    96,    97,    72,    84,  -189,  -189,
    -189,  -189,  -189,  -189,  -189,    83,    98,     4,    26,   100,
      26,  -189,  -189,  -189,  -189,    87,    99,  -189,    88,  -189,
    -189,    58,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,
    -189,  -189,  -189,   -40,   -26,   -10,  -189,     9,  -189,   101,
     102,   107,   106,   103,   108,   109,   104,   105,    18,   110,
     111,    40,    38,    26,  -189,  -189,  -189,  -189,  -189,  -189,
    -189,  -189,  -189,  -189,  -189,   -38,  -189,   117,  -189,   113,
      26,  -189,  -189,  -189,    91,   112,  -189,   119,  -189,  -189,
     114,  -189,  -189,  -189,   116,    26,    26,    26,    26,    26,
      26,    26,    26,  -189,  -189,  -189,  -189,  -189,    40,  -189,
    -189,   121,    40,  -189,  -189,  -189,  -189,  -189,  -189,  -189,
    -189,  -189,  -189,  -189,  -189,  -189,  -189,   118,   120,  -189,
     122,   123,    67,  -189,  -189,  -189,  -189,  -189,  -189,  -189,
    -189,  -189,    26,    26,    26,   131,   127,  -189,   124,   125,
    -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,   134,   126,
    -189,   133,   110,    40,   132,    40,  -189,  -189,  -189,  -189,
    -189,   -14,  -189,  -189,  -189,  -189,   129,  -189
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     1,     0,    19,     0,     3,    26,
      24,    25,     0,    11,    20,     0,     0,     4,     0,    17,
       6,     0,    22,    19,     0,    27,     5,     0,    18,    16,
      35,    23,     0,     0,     0,    82,     0,     0,    92,    94,
      96,    98,     0,     0,     0,     0,     0,     0,     0,     0,
      36,     0,    35,    35,    35,    35,    35,    35,     7,    13,
      88,     0,   133,   130,     0,     0,     0,     0,     0,     0,
     102,   103,   104,   105,     0,     0,     0,     0,    28,    30,
      31,    32,    33,    34,    29,     0,     0,     0,     0,     0,
       0,    93,    95,    97,    99,     0,     0,   106,     0,    37,
     124,    19,     8,    14,    73,    74,    71,    91,    72,    69,
      70,    65,    90,    50,    56,    62,    63,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   129,
       0,     0,    16,     0,    48,    46,    42,    44,    41,    52,
      54,    51,    58,    60,    57,    81,    67,     0,    83,     0,
       0,   131,   100,   101,     0,     0,    40,     0,    39,   126,
       0,    12,     9,    15,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    77,    79,    64,    89,     0,   135,
     136,     0,     0,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,     0,     0,    38,
       0,     0,    11,    66,    49,    47,    43,    45,    53,    55,
      59,    61,     0,     0,     0,    87,     0,   132,     0,     0,
     127,   125,    10,    76,    78,    80,    85,    84,     0,     0,
     107,     0,   129,     0,     0,     0,   108,   128,    86,   137,
     134,     0,   139,   140,   141,   142,     0,   138
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -189,  -189,  -189,  -189,  -189,   -50,  -189,  -189,  -189,  -189,
    -189,    24,  -189,  -189,   -22,  -189,   154,  -189,  -189,   175,
    -131,  -189,   -46,  -189,  -189,  -189,  -189,   -85,  -189,  -189,
    -189,  -189,  -189,  -136,  -189,  -189,  -189,  -122,  -189,  -189,
    -189,  -188,  -189,  -189,  -189,  -189,    71,  -189,  -189,  -189,
    -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,
    -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,   -47,
    -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,
    -189
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     5,    13,    21,    17,    24,    85,   131,   202,
     102,    32,    86,   132,     8,    23,    15,    18,    27,    33,
      26,    30,    51,    52,    76,   128,   157,   112,   138,   167,
     168,   166,   165,   113,   141,   169,   170,   114,   144,   171,
     172,   115,   145,   116,   133,   117,   118,   176,   212,   213,
     214,    53,    61,   178,   227,   233,    54,    87,   119,    55,
      64,    65,    66,    67,   126,   197,    56,    77,   129,   160,
     200,   232,    57,    90,   182,    89,   122,   181,   229,   241,
     246
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     162,    28,   134,   120,   135,   123,    79,    80,    81,    82,
      83,    84,     3,   104,   105,   173,   174,   175,   104,   105,
     139,   140,   136,   137,   223,   224,   225,   104,   105,   204,
     205,   206,   207,   208,   209,   104,   105,   242,   243,   244,
     245,   142,   143,   158,     9,    10,    11,   215,   164,   210,
     211,   217,   106,   107,   108,   109,   110,   106,     1,   108,
       4,   111,     6,     7,    14,   180,   106,   156,   108,   109,
     110,    16,   -21,    19,   106,   111,   108,   109,   110,   130,
      20,    34,    35,   111,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    22,    25,
      29,    59,   238,    58,   240,    60,    62,    63,    68,    69,
      70,    71,    95,    72,    73,    99,    74,    78,    50,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,    75,  -123,    88,    96,    91,    92,    93,
      94,   100,   101,   103,   226,   124,    97,    98,   121,   127,
     149,   150,   222,   152,   153,   155,   163,   125,   159,   147,
     148,   151,   177,   179,   199,   154,   216,   220,   221,   230,
     219,   161,   201,   198,   203,   228,   218,   234,   236,   247,
     239,    31,    12,   231,   235,   237,     0,     0,   146
};

static const yytype_int16 yycheck[] =
{
     131,    23,    42,    88,    44,    90,    52,    53,    54,    55,
      56,    57,    48,     9,    10,    53,    54,    55,     9,    10,
      46,    47,    62,    63,   212,   213,   214,     9,    10,   165,
     166,   167,   168,   169,   170,     9,    10,    51,    52,    53,
      54,    51,    52,   128,     6,     7,     8,   178,   133,   171,
     172,   182,    48,    49,    50,    51,    52,    48,     3,    50,
       0,    57,    56,     5,    48,   150,    48,    49,    50,    51,
      52,     4,    61,    45,    48,    57,    50,    51,    52,   101,
      48,    11,    12,    57,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    61,    59,
      57,    48,   233,    58,   235,    57,    57,    57,    57,    57,
      45,    45,    50,    45,    45,    43,    57,    60,    48,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    57,    57,    57,    50,    45,    45,    45,
      45,    57,    59,    45,    13,    58,    50,    50,    48,    61,
      43,    45,   202,    45,    45,    50,   132,    58,    48,    58,
      58,    58,    45,    50,    45,    61,    45,    45,    45,    45,
      50,    60,    58,    61,    58,    48,    58,    43,    45,    50,
      48,    27,     7,    58,    58,   232,    -1,    -1,   117
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    65,    48,     0,    66,    56,     5,    78,     6,
       7,     8,    83,    67,    48,    80,     4,    69,    81,    45,
      48,    68,    61,    79,    70,    59,    84,    82,    78,    57,
      85,    80,    75,    83,    11,    12,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      48,    86,    87,   115,   120,   123,   130,   136,    58,    48,
      57,   116,    57,    57,   124,   125,   126,   127,    57,    57,
      45,    45,    45,    45,    57,    57,    88,   131,    60,    86,
      86,    86,    86,    86,    86,    71,    76,   121,    57,   139,
     137,    45,    45,    45,    45,    50,    50,    50,    50,    43,
      57,    59,    74,    45,     9,    10,    48,    49,    50,    51,
      52,    57,    91,    97,   101,   105,   107,   109,   110,   122,
      91,    48,   140,    91,    58,    58,   128,    61,    89,   132,
      78,    72,    77,   108,    42,    44,    62,    63,    92,    46,
      47,    98,    51,    52,   102,   106,   110,    58,    58,    43,
      45,    58,    45,    45,    61,    50,    49,    90,    91,    48,
     133,    60,    84,    75,    91,    96,    95,    93,    94,    99,
     100,   103,   104,    53,    54,    55,   111,    45,   117,    50,
      91,   141,   138,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,   129,    61,    45,
     134,    58,    73,    58,    97,    97,    97,    97,    97,    97,
     101,   101,   112,   113,   114,    84,    45,    84,    58,    50,
      45,    45,    69,   105,   105,   105,    13,   118,    48,   142,
      45,    58,   135,   119,    43,    58,    45,   133,    84,    48,
      84,   143,    51,    52,    53,    54,   144,    50
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    64,    66,    67,    68,    65,    70,    71,    72,    73,
      69,    69,    74,    76,    77,    75,    75,    79,    78,    78,
      80,    81,    82,    80,    83,    83,    83,    85,    84,    86,
      86,    86,    86,    86,    86,    86,    88,    89,    87,    90,
      90,    91,    93,    92,    94,    92,    95,    92,    96,    92,
      92,    97,    99,    98,   100,    98,    98,   101,   103,   102,
     104,   102,   102,   106,   105,   108,   107,   107,   107,   109,
     109,   110,   110,   110,   110,   112,   111,   113,   111,   114,
     111,   111,   116,   117,   115,   119,   118,   118,   121,   120,
     122,   122,   124,   123,   125,   123,   126,   123,   127,   123,
     123,   123,   123,   123,   123,   123,   128,   123,   123,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   131,   132,   130,   134,   135,   133,   133,
     137,   138,   136,   139,   136,   140,   141,   143,   142,   144,
     144,   144,   144
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     0,     9,     0,     0,     0,     0,
      12,     0,     3,     0,     0,     6,     0,     0,     6,     0,
       1,     0,     0,     5,     1,     1,     1,     0,     4,     2,
       2,     2,     2,     2,     2,     0,     0,     0,     6,     1,
       1,     2,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     2,     0,     3,     0,     3,     0,     2,     0,     3,
       0,     3,     0,     0,     3,     0,     4,     2,     1,     1,
       1,     1,     1,     1,     1,     0,     3,     0,     3,     0,
       3,     0,     0,     0,     8,     0,     3,     0,     0,     6,
       1,     1,     0,     3,     0,     3,     0,     3,     0,     3,
       5,     5,     2,     2,     2,     2,     0,     8,     9,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     0,     7,     0,     0,     5,     0,
       0,     0,     7,     0,    10,     3,     1,     0,     6,     1,
       1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 413 "robython.y" /* yacc.c:1646  */
    {globalOrLocal = true;currentDir = (yyvsp[0].sval); globalFunction=(yyvsp[0].sval); halfCode.open((yyvsp[0].sval));}
#line 1779 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 414 "robython.y" /* yacc.c:1646  */
    {

							DirObject myDirObject;
							myDirObject.id = (yyvsp[-3].sval);
							myDirObject.listaVar = listVariables;
							DirProc.push_back(myDirObject);
							listVariables.clear();}
#line 1791 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 420 "robython.y" /* yacc.c:1646  */
    {currentDir = (yyvsp[-5].sval); halfCode<<"mainStart"<<endl;}
#line 1797 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 421 "robython.y" /* yacc.c:1646  */
    {
					halfCode<<"mainEnd";
				}
#line 1805 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 427 "robython.y" /* yacc.c:1646  */
    {globalOrLocal = false; currentDir = (yyvsp[0].sval);
						halfCode<<"void "<<(yyvsp[0].sval)<<" startParam ";}
#line 1812 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 428 "robython.y" /* yacc.c:1646  */
    {halfCode<<" endParam "<<endl;
																			}
#line 1819 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 429 "robython.y" /* yacc.c:1646  */
    {
																			nameFunct=(yyvsp[-6].sval);
																			bool insertF = true;
																			insertF=validateInsertFunc(nameFunct);
																			if (insertF==true)
																			{
																				DirObject myDirObject;
																				myDirObject.id = (yyvsp[-6].sval);
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
												}
#line 1847 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 451 "robython.y" /* yacc.c:1646  */
    {halfCode<<" endFunc"<<endl;}
#line 1853 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 460 "robython.y" /* yacc.c:1646  */
    { //Llamada a funcion para verificar si el nombre de variable ya existe 
			nameID = (yyvsp[0].sval); 
			bool insert=true; 
			insert = validateInsertVar(nameID);
			if (insert == true )
			{
				var.setId((yyvsp[0].sval));
		 		addCounterOfParameter();
		 		if (maxParError==false)
		 			listVariables.push_back(var),halfCode<<(yyvsp[0].sval);
		 	}
		 	else 
		 	{	cout << "ERROR FATAL: NO ES POSIBLE DECLARAR PARÁMETROS CON EL MISMO NOMBRE DENTRO DE UNA FUNCION" << endl;
		 		error=true; return -1;
		 	}			 	 
		 	}
#line 1874 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 475 "robython.y" /* yacc.c:1646  */
    {halfCode<<" , ";}
#line 1880 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 480 "robython.y" /* yacc.c:1646  */
    {halfCode<<" ; "<<endl;}
#line 1886 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 485 "robython.y" /* yacc.c:1646  */
    { 	//Llamada a funcion para verificar si el nombre de variable ya existe 
				nameID = (yyvsp[0].sval); 
				bool insert=true; 
				insert = validateInsertVar(nameID);
				if (insert == true )
				{
					var.setId((yyvsp[0].sval));
			 		addCounterOfKeyType(globalOrLocal);
			 		if (maxGlobalError==false && maxLocalError==false)
			 			listVariables.push_back(var),halfCode<<(yyvsp[0].sval);
			 	}
			 	else 
			 		{	cout << "ERROR FATAL: NO ES POSIBLE DECLARAR VARIABLES CON EL MISMO NOMBRE DENTRO DE UNA FUNCION" << endl;
			 			error=true; return -1;
			 		}
		 	}
#line 1907 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 503 "robython.y" /* yacc.c:1646  */
    {
				nameID = (yyvsp[0].sval); 
				bool insert=true; 
				insert = validateInsertVar(nameID);
				if (insert == true)
				{
					var.setId((yyvsp[0].sval));
			   		addCounterOfKeyType(globalOrLocal);
			   		if (maxGlobalError==false && maxLocalError==false)
			   			listVariables.push_back(var),halfCode<<(yyvsp[0].sval);
				}
				else 
					{	cout << "ERROR FATAL: NO ES POSIBLE DECLARAR VARIABLES CON EL MISMO NOMBRE DENTRO DE UNA FUNCION" << endl;
			 			error=true; return -1; 
			 		}		
			}
#line 1928 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 518 "robython.y" /* yacc.c:1646  */
    {halfCode<<" , ";}
#line 1934 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 522 "robython.y" /* yacc.c:1646  */
    {btext = true; bnum = false; bbool = false; halfCode<<"string ";}
#line 1940 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 523 "robython.y" /* yacc.c:1646  */
    {bnum = true; btext = false; bbool = false; halfCode<<"float ";}
#line 1946 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 524 "robython.y" /* yacc.c:1646  */
    {bbool = true; bnum = false; btext = false; halfCode<<"boolean ";}
#line 1952 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 528 "robython.y" /* yacc.c:1646  */
    {halfCode<<"startBlock ";}
#line 1958 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 528 "robython.y" /* yacc.c:1646  */
    {halfCode<<"endBlock ";}
#line 1964 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 542 "robython.y" /* yacc.c:1646  */
    {bcurrentIDvar= isVarAlreadyDeclared(currentDir,(yyvsp[0].sval)); if(bcurrentIDvar) PilaO.push((yyvsp[0].sval)), PTipos.push(getTypeOfVar(currentDir,(yyvsp[0].sval)));
		}
#line 1971 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 543 "robython.y" /* yacc.c:1646  */
    { POper.push(9); }
#line 1977 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 543 "robython.y" /* yacc.c:1646  */
    {halfCode<<" ;"<<endl;}
#line 1983 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 547 "robython.y" /* yacc.c:1646  */
    {
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
#line 2002 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 561 "robython.y" /* yacc.c:1646  */
    {PTipos.push(1);
					if(validateOnCube(9))
					{
					string OperI=PilaO.top(); PilaO.pop();
					halfCode << "=" << " " << (yyvsp[0].sval) << " " << OperI;
					 }}
#line 2013 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 575 "robython.y" /* yacc.c:1646  */
    {POper.push(6);}
#line 2019 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 575 "robython.y" /* yacc.c:1646  */
    {if(!validateOnCube(6)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
#line 2025 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 576 "robython.y" /* yacc.c:1646  */
    {POper.push(5);}
#line 2031 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 576 "robython.y" /* yacc.c:1646  */
    {if(!validateOnCube(5)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
#line 2037 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 577 "robython.y" /* yacc.c:1646  */
    {POper.push(8);}
#line 2043 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 577 "robython.y" /* yacc.c:1646  */
    {if(!validateOnCube(8)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
#line 2049 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 578 "robython.y" /* yacc.c:1646  */
    {POper.push(7);}
#line 2055 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 578 "robython.y" /* yacc.c:1646  */
    {if(!validateOnCube(7)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
#line 2061 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 583 "robython.y" /* yacc.c:1646  */
    {
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
#line 2079 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 599 "robython.y" /* yacc.c:1646  */
    {POper.push(10);}
#line 2085 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 599 "robython.y" /* yacc.c:1646  */
    {if(!validateOnCube(10)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true;return  -1;}}
#line 2091 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 600 "robython.y" /* yacc.c:1646  */
    {POper.push(11);}
#line 2097 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 600 "robython.y" /* yacc.c:1646  */
    {if(!validateOnCube(11)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
#line 2103 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 605 "robython.y" /* yacc.c:1646  */
    {
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
#line 2121 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 621 "robython.y" /* yacc.c:1646  */
    {POper.push(3);}
#line 2127 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 621 "robython.y" /* yacc.c:1646  */
    {if(!validateOnCube(3)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
#line 2133 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 622 "robython.y" /* yacc.c:1646  */
    {POper.push(4);}
#line 2139 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 622 "robython.y" /* yacc.c:1646  */
    {if(!validateOnCube(4)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
#line 2145 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 628 "robython.y" /* yacc.c:1646  */
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
         }
#line 2164 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 641 "robython.y" /* yacc.c:1646  */
    {
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
#line 2182 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 658 "robython.y" /* yacc.c:1646  */
    { POper.push(-1); halfCode<<" ( ";}
#line 2188 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 658 "robython.y" /* yacc.c:1646  */
    {while(POper.top() != -1)
		 										{
		 											string OperD=PilaO.top();PilaO.pop();
								                    string OperI=PilaO.top();PilaO.pop();
								                    int operador=POper.top();POper.pop();
								                    int tipo = PTipos.top();
								                    halfCode << nameOperador(operador) << " " << OperI << " " << OperD << " " << "T"<<cont++<<" " << endl; }
								                    POper.pop();
								                    halfCode<<" ) ";
		 									}
#line 2203 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 673 "robython.y" /* yacc.c:1646  */
    {POper.push(3); }
#line 2209 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 674 "robython.y" /* yacc.c:1646  */
    {POper.push(4);}
#line 2215 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 679 "robython.y" /* yacc.c:1646  */
    {		bcurrentIDvar=isVarAlreadyDeclared(currentDir,(yyvsp[0].sval)); 
				if(bcurrentIDvar) PilaO.push((yyvsp[0].sval)), PTipos.push(getTypeOfVar(currentDir,(yyvsp[0].sval)));
				cont_print++; 
				name_id=(yyvsp[0].sval); 		
		  }
#line 2225 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 684 "robython.y" /* yacc.c:1646  */
    {PilaO.push((yyvsp[0].sval)); PTipos.push(0); cont_print++;}
#line 2231 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 685 "robython.y" /* yacc.c:1646  */
    {PilaO.push("true"); PTipos.push(2);}
#line 2237 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 686 "robython.y" /* yacc.c:1646  */
    {PilaO.push("false"); PTipos.push(2);}
#line 2243 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 690 "robython.y" /* yacc.c:1646  */
    {POper.push(0);}
#line 2249 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 690 "robython.y" /* yacc.c:1646  */
    {if(!validateOnCube(0)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
#line 2255 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 691 "robython.y" /* yacc.c:1646  */
    {POper.push(1);}
#line 2261 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 691 "robython.y" /* yacc.c:1646  */
    {if(!validateOnCube(1)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
#line 2267 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 692 "robython.y" /* yacc.c:1646  */
    {POper.push(2);}
#line 2273 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 692 "robython.y" /* yacc.c:1646  */
    {if(!validateOnCube(2)){ cout<<"ERROR FATAL: OPERACIÓN INVÁLIDA"<<endl; error=true; return  -1;}}
#line 2279 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 697 "robython.y" /* yacc.c:1646  */
    {halfCode<<"if ( ";}
#line 2285 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 697 "robython.y" /* yacc.c:1646  */
    {halfCode<<" ) endIfParam ";}
#line 2291 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 701 "robython.y" /* yacc.c:1646  */
    {halfCode<<"else "<<endl;}
#line 2297 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 706 "robython.y" /* yacc.c:1646  */
    {halfCode<<"print ( "; cuadr_print=true; cont_print=0;}
#line 2303 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 706 "robython.y" /* yacc.c:1646  */
    { if ((cuadr_print==true) && (cont_print==1))
																								halfCode <<  name_id + " ) endPrint" << endl;
																						   else 
																						   {
		 										
																								halfCode<<" ) endPrint" << endl;
																						   }
																							cuadr_print=false;}
#line 2316 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 718 "robython.y" /* yacc.c:1646  */
    {halfCode<<(yyvsp[0].sval);}
#line 2322 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 722 "robython.y" /* yacc.c:1646  */
    {halfCode<<" roboTemp "<<endl;}
#line 2328 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 723 "robython.y" /* yacc.c:1646  */
    {halfCode<<" roboDistance "<<endl;}
#line 2334 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 724 "robython.y" /* yacc.c:1646  */
    {halfCode<<" roboRightLight "<<endl;}
#line 2340 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 725 "robython.y" /* yacc.c:1646  */
    {halfCode<<" roboLeftLight "<<endl;}
#line 2346 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 726 "robython.y" /* yacc.c:1646  */
    {halfCode<<" roboOnLED "<<(yyvsp[-2].sval)<<" "<<endl;}
#line 2352 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 727 "robython.y" /* yacc.c:1646  */
    {halfCode<<" roboOffLED "<<(yyvsp[-2].sval)<<" "<<endl;}
#line 2358 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 728 "robython.y" /* yacc.c:1646  */
    {halfCode<<" roboForward "<<endl;}
#line 2364 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 729 "robython.y" /* yacc.c:1646  */
    {halfCode<<" roboBack "<<endl;}
#line 2370 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 730 "robython.y" /* yacc.c:1646  */
    {halfCode<<" roboLeft "<<endl;}
#line 2376 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 731 "robython.y" /* yacc.c:1646  */
    {halfCode<<" roboRight "<<endl;}
#line 2382 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 732 "robython.y" /* yacc.c:1646  */
    {halfCode<<" roboBuzzer "<<(yyvsp[0].sval);}
#line 2388 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 733 "robython.y" /* yacc.c:1646  */
    {halfCode<<" roboRGB "<<(yyvsp[-6].sval)<<" "<<(yyvsp[-4].sval)<<" "<<(yyvsp[-2].sval)<<endl;}
#line 2394 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 737 "robython.y" /* yacc.c:1646  */
    {halfCode<<" DO "<<endl;}
#line 2400 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 738 "robython.y" /* yacc.c:1646  */
    {halfCode<<" DOP "<<endl;}
#line 2406 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 739 "robython.y" /* yacc.c:1646  */
    {halfCode<<" RE "<<endl;}
#line 2412 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 740 "robython.y" /* yacc.c:1646  */
    {halfCode<<" REP "<<endl;}
#line 2418 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 741 "robython.y" /* yacc.c:1646  */
    {halfCode<<" MI "<<endl;}
#line 2424 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 742 "robython.y" /* yacc.c:1646  */
    {halfCode<<" MIP "<<endl;}
#line 2430 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 743 "robython.y" /* yacc.c:1646  */
    {halfCode<<" FA "<<endl;}
#line 2436 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 744 "robython.y" /* yacc.c:1646  */
    {halfCode<<" FAP "<<endl;}
#line 2442 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 745 "robython.y" /* yacc.c:1646  */
    {halfCode<<" SOL "<<endl;}
#line 2448 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 746 "robython.y" /* yacc.c:1646  */
    {halfCode<<" SOLP "<<endl;}
#line 2454 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 747 "robython.y" /* yacc.c:1646  */
    {halfCode<<" LA "<<endl;}
#line 2460 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 748 "robython.y" /* yacc.c:1646  */
    {halfCode<<" LAP "<<endl;}
#line 2466 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 749 "robython.y" /* yacc.c:1646  */
    {halfCode<<" SI "<<endl;}
#line 2472 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 750 "robython.y" /* yacc.c:1646  */
    {halfCode<<" SI "<<endl;}
#line 2478 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 754 "robython.y" /* yacc.c:1646  */
    {nameFunct=(yyvsp[0].sval);
				if (validateExistFunc(nameFunct) != true) 
				 {
				 	cout << "ERROR FATAL: NO ES POSIBLE UTILIZAR FUNCIONES QUE NO HAN SIDO DECLARADAS" << endl;
				 	error=true; return -1; 	
				 }
				 else 
				 {
				 	halfCode <<(yyvsp[0].sval); 
				 	contadorParametros=0; 
					cantPar=numParFunc(nameFunct);
				 }
			}
#line 2496 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 766 "robython.y" /* yacc.c:1646  */
    {halfCode<<" ( ";}
#line 2502 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 766 "robython.y" /* yacc.c:1646  */
    {halfCode<<" ) ;"<<endl;}
#line 2508 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 769 "robython.y" /* yacc.c:1646  */
    {nameID=(yyvsp[0].sval); 
		   if (contadorParametros < cantPar)
		   {
			   	if (validateExistVar(nameID))
			   	{
			   		halfCode<<(yyvsp[0].sval);
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
		   
		}
#line 2545 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 800 "robython.y" /* yacc.c:1646  */
    {if (contadorParametros == (cantPar)) halfCode<<" "; else halfCode<< " , ";}
#line 2551 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 801 "robython.y" /* yacc.c:1646  */
    {if (contadorParametros < (cantPar))
			{
				cout << "ERROR FATAL: EL NÚMERO DE PARÁMETROS EN LA LLAMADA SON MENOS QUE EN LA DECLARACIÓN DE LA FUNCIÓN" << endl;
				error=true; return -1; 
			} 
		}
#line 2562 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 810 "robython.y" /* yacc.c:1646  */
    {halfCode<<" while ( ";}
#line 2568 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 810 "robython.y" /* yacc.c:1646  */
    {halfCode<<" ) endWhileParam ";}
#line 2574 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 811 "robython.y" /* yacc.c:1646  */
    {halfCode<<" for ( ";}
#line 2580 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 815 "robython.y" /* yacc.c:1646  */
    {halfCode<<(yyvsp[-2].sval)<<"="<<(yyvsp[0].sval)<<"; ";}
#line 2586 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 819 "robython.y" /* yacc.c:1646  */
    {halfCode<<"; ";}
#line 2592 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 823 "robython.y" /* yacc.c:1646  */
    {halfCode<<(yyvsp[-2].sval)<<"="<<(yyvsp[0].sval)<<" ";}
#line 2598 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 823 "robython.y" /* yacc.c:1646  */
    {halfCode<<(yyvsp[0].sval)<<" )"<<endl;}
#line 2604 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 827 "robython.y" /* yacc.c:1646  */
    {halfCode<<'+';}
#line 2610 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 828 "robython.y" /* yacc.c:1646  */
    {halfCode<<'-';}
#line 2616 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 829 "robython.y" /* yacc.c:1646  */
    {halfCode<<'*';}
#line 2622 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 830 "robython.y" /* yacc.c:1646  */
    {halfCode<<'/';}
#line 2628 "robython.tab.c" /* yacc.c:1646  */
    break;


#line 2632 "robython.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 833 "robython.y" /* yacc.c:1906  */


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
